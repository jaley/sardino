(ns sonos-proxy.auth
    (:require [ring.middleware.oauth2 :refer [wrap-oauth2]]
              [ring.middleware.params :refer [wrap-params]]
              [ring.util.response     :as resp]

              [sonos-proxy.secrets    :as secrets]
              [sonos-proxy.whitelist  :as whitelist]
              [sonos-proxy.sonos      :as sonos]))

(defn msa-oauth2-profile
    "Build a config profile for MSA OAuth 2.0"
    []
    {:authorize-uri    "https://login.microsoftonline.com/common/oauth2/v2.0/authorize"
     :access-token-uri "https://login.microsoftonline.com/common/oauth2/v2.0/token"
     :client-id        (secrets/get-secret :msa-client-id)
     :client-secret    (secrets/get-secret :msa-client-secret)
     :scopes           ["https://graph.microsoft.com/user.read"]
     :launch-uri       "/auth/msa/login"
     :redirect-uri     (str (secrets/get-secret :base-uri) "/auth/msa/callback")
     :landing-uri      "/"})

(defn sonos-oauth2-profile
    "Build a config profile for Sonos Control API OAuth 2.0"
    []
    {:authorize-uri    "https://api.sonos.com/login/v3/oauth"
     :access-token-uri "https://api.sonos.com/login/v3/oauth/access"
     :client-id        (secrets/get-secret :sonos-client-id)
     :client-secret    (secrets/get-secret :sonos-client-secret)
     :scopes           ["playback-control-all"]
     :launch-uri       "/auth/sonos/login"
     :redirect-uri     (str (secrets/get-secret :base-uri) "/auth/sonos/callback")
     :landing-uri      "/api/sonos-init"
     :basic-auth?      true})

(defn wrap-auth
    "Wrap API handler with OAuth 2.0 decorator and Email whitelist"
    [handler]
    (-> handler
        ;; note: wrapping Sonos OAuth within MSA and whitelist,
        ;; as we don't want to activate these routes outside auth
        (wrap-oauth2
            {:sonos (sonos-oauth2-profile)})
        whitelist/wrap-whitelist
        (wrap-oauth2 
            {:msa (msa-oauth2-profile)})
        wrap-params))

(defn msa-logout-response
    "Generate a response to log the user out of MSA"
    []
    (-> "https://login.microsoftonline.com/common/oauth2/v2.0/logout"
        (str "?post_logout_redirect_uri=" (secrets/get-secret :base-uri) "/")
        resp/redirect
        (update-in [:session :ring.middleware.oauth2/access-tokens] dissoc :msa)
        (update-in [:oauth2/access-tokens] dissoc :msa)))

(defn sonos-logout-response
    "Generate a response to erase Sonos session state"
    []
    (sonos/logout!)
    (-> (resp/redirect "/")
        (update-in [:session :ring.middleware.oauth2/access-tokens] dissoc :sonos)
        (update-in [:oauth2/access-tokens] dissoc :sonos)))
