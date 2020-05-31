(ns sonos-proxy.whitelist
    (:require [clojure.string      :as str]
              [taoensso.timbre     :as log]

              [sonos-proxy.secrets :as secrets]
              [sonos-proxy.graph   :as graph]
              [sonos-proxy.util    :as util]))

(defn get-whitelist
    "Returns a set of whitelisted emails from environment config"
    []
    (-> :msa-user-id-whitelist
        secrets/get-secret
        (str/split #";")
        set))

(defn wrap-whitelist
    "Decorate a ring handler to check OAuth sessions for an MSA token, validate
     that token and ensure the user ID is whitelisted."
    [handler]
    (let [whitelisted? (get-whitelist)]
        (fn [req]
            (if-let [session (get-in req [:oauth2/access-tokens :msa])]
                (if (-> session graph/validate whitelisted?)
                    (handler req)
                    (util/forbidden "Couldn't validate MSA ID against whitelist"))
                (util/unauthorized "MSA sign-in required for this action")))))
