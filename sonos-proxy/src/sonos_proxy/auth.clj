(ns sonos-proxy.auth
    (:require [ring.middleware.oauth2 :refer [wrap-oauth2]]
              [ring.middleware.params :refer [wrap-params]]

              [sonos-proxy.secrets    :as secrets]
              [sonos-proxy.whitelist  :as whitelist]))

(defn msa-oauth2-profile
    "Build a config profile for MSA OAuth 2.0"
    []
    {:msa
        {:authorize-uri    "https://login.microsoftonline.com/common/oauth2/v2.0/authorize"
         :access-token-uri "https://login.microsoftonline.com/common/oauth2/v2.0/token"
         :client-id        (secrets/get-secret :msa-client-id)
         :client-secret    (secrets/get-secret :msa-client-secret)
         :scopes           ["https://graph.microsoft.com/user.read"]
         :launch-uri       "/auth/msa/login"
         :redirect-uri     (str (secrets/get-secret :base-uri) "/auth/msa/callback")
         :landing-uri      "/main"}})

(defn wrap-auth
    "Wrap API handler with OAuth 2.0 decorator and Email whitelist"
    [handler]
    (-> handler
        whitelist/wrap-whitelist
        (wrap-oauth2 (msa-oauth2-profile))
        wrap-params))
