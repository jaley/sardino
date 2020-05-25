(ns sonos-proxy.graph
    (:import [com.microsoft.graph.authentication IAuthenticationProvider]
             [com.microsoft.graph.requests.extensions GraphServiceClient])
             
    (:require [taoensso.timbre :as log]))

;; TODO: Handle expiry / refresh properly

(defn auth-provider
    "Build an auth provider that uses the token obtained in OAuth flow"
    [token]
    (reify IAuthenticationProvider
        (authenticateRequest [this req]
            (.addHeader req "Authorization" (str "Bearer " token)))))

(defn graph-client
    "Build an MS graph client that uses provided access token"
    [token]
    (.. (GraphServiceClient/builder) 
        (authenticationProvider (auth-provider token))
        buildClient))

(defn get-user-id
    "Retrieve User.Id for current user from MS Graph API"
    [client]
    (try
        (.. client me buildRequest get id)
        (catch Exception e
            (log/error e "Error getting user ID"))))

(defn validate
    "Validate the MSA session state (token) and return authenticated user ID"
    [{:keys [token expires] :as msa}]
    (-> token graph-client get-user-id))
