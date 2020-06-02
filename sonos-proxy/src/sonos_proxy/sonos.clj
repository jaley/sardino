(ns sonos-proxy.sonos
    (:require [clojure.set         :as set]
              [clj-http.client     :as http]
              [clj-http.conn-mgr   :as conn]
              [clj-time.core       :as time]
              [clj-time.format     :as tfmt]
              [cheshire.core       :as json]
              [taoensso.timbre     :as log]
              
              [sonos-proxy.secrets :as secrets]
              [sonos-proxy.util    :as util])

    (:import  [org.apache.http.impl.execchain HttpResponseProxy]
              [org.joda.time DateTime]))

;; Seems clj-http doesn't know how to handle HttpResponseProxy?
;; <3 them for using a multimethod
(defmethod conn/shutdown-manager
    HttpResponseProxy
    [^HttpResponseProxy proxy]
    (.close proxy))

(defn- sonos-base-uri
    "Add Sonos API base URI (used for login/auth requests only) to path"
    [path]
    (str "https://api.sonos.com" path))

(defn- sonos-control-uri
    "Add Base URI for Control API requests to path"
    [path]
    (str "https://api.ws.sonos.com/control/api/v1" path))

(def controller-state
    "Global controller state for incoming MQTT commands"
    (atom {}))

(defn logout!
    "Reset Sonos controller state on user logout request"
    []
    (log/info "Sonos API logout")
    (swap! controller-state (constantly {})))

(defmulti to-expiry-dt
    "Convert object to an expiry DateTime instant"
    class)

(defmethod to-expiry-dt String
    [expiry-str]
    (-> :date-time tfmt/formatters (tfmt/parse expiry-str)))

(defmethod to-expiry-dt Integer
    [duration-secs]
    (-> (time/now)
        (time/plus (time/seconds duration-secs))))

(defmethod to-expiry-dt DateTime
    [dt]
    dt)

(defn init!
    "Initialize global token state with given credentials"
    [{:keys [token expires refresh-token] :as creds}]
    (log/info "Sonos API initialized with new creds")
    (swap! controller-state
        (fn [state]
            (assoc state :creds
                {:token         token
                 :expires       (to-expiry-dt expires)
                 :refresh-token refresh-token}))))

(defn- auth-header
    "Generate an Authorization header for API calls"
    [creds]
    {:Authorization (str "Bearer " (:token creds))})

(defn- basic-auth-creds
    "Return vector of Sonos API creds for Basic Auth"
    []
    [(secrets/get-secret :sonos-client-id)
     (secrets/get-secret :sonos-client-secret)])

(defn- refresh!
    "Request a new token using the refresh token and update
    controller-state with fresh tokens"
    []
    (-> (http/post (sonos-base-uri "/login/v3/oauth/access")
            {:basic-auth (basic-auth-creds)
             :form-params {:grant_type    "refresh_token" 
                           :refresh_token (-> @controller-state :creds :refresh-token)}
                           :accept        :json 
                           :as            :json})
        :body
        (select-keys [:access_token :expires_in :refresh_token])
        (set/rename-keys {:access_token  :token
                          :expires_in    :expires
                          :refresh_token :refresh-token})
        (init!)))

(defn- with-refresh
    "Check for token expiry, refresh and run req-fn, which will be
    passed clj-http options map as params"
    [req-fn]
    (if (= @controller-state {})
        (do
            (log/error "Attempted to execute API command before init!")
            (util/forbidden "Sonos sign-in required"))
        (do
            (when (-> @controller-state :creds :expires 
                    (time/minus (time/hours 1))
                    (time/before? (time/now)))
                (refresh!))
            (req-fn {:headers (-> @controller-state :creds auth-header)}))))

(defn- control-api-get
    "Execute a GET request on the Sonos Control API"
    [path & [params]]
    (log/debug (str "Sonos API GET: " path "; Params: " params))
    (with-refresh
        (fn [opts]
            (:body
                (http/get (sonos-control-uri path)
                    (cond-> opts
                        true   (assoc :accept :json)
                        true   (assoc :as :json)
                        params (assoc :query-params params)))))))

(defn- control-api-post
    "Execute a POST request on the Sonos Control API"
    [path & [params]]
    (log/debug (str "Sonos API POST: " path "; Params: " params))
    (with-refresh
        (fn [opts]
            (:body
                (http/post (sonos-control-uri path)
                    (cond-> opts
                        params (assoc :body (json/encode params))
                        true   (assoc :content-type :json)
                        true   (assoc :accept :json)
                        true   (assoc :as :json)))))))

(defn- groups-in-household
    "Returns a sequence of groups within household"
    [household-id]
    (map (fn [group]
            {:household-id household-id
             :group-id     (:id group)
             :group-name   (:name group)})
         (-> (str "/households/" household-id "/groups")
             control-api-get
             :groups)))

(defn groups
    "Returns a sequence of group maps for all groups in all households
    registered to logged in user"
    []
    (->> (control-api-get "/households")
         :households
         (map :id)
         (mapcat groups-in-household)))

(defn get-volume
    "Returns the current volume (0-100) for the requested group.
    Return value is a map containing :volume, :muted and :fixed"
    [group-id]
    (control-api-get
        (str "/groups/" group-id "/groupVolume")))

(defn set-volume
    "Set the group volume (integer in 0-100) to given value"
    [group-id volume]
    (control-api-post 
        (str "/groups/" group-id "/groupVolume") 
        {:volume volume}))
