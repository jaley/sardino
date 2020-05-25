(ns sonos-proxy.api
    (:require [compojure.core           :refer [routes POST GET]]
              [compojure.route          :refer [not-found]]
              [ring.middleware.defaults :refer [wrap-defaults site-defaults]]
              [taoensso.timbre          :as log]
              
              [sonos-proxy.auth         :as auth]))

(defn secure-routes
    "Routes requiring authentication"
    []
    (auth/wrap-auth
        (routes
            (GET "/main" [] "Logged in!"))))

(defn open-routes
    "Routes that won't be behind OAuth login"
    []
    (routes
        (GET "/" [] "All is very good")))

(defn build-routes
    "Compose routes and apply global settings"
    []
    (let [settings (-> site-defaults (assoc-in [:session :cookie-attrs :same-site] :lax))]
        (-> (routes
                (open-routes)
                (secure-routes)
                (not-found "Not Found"))
            (wrap-defaults settings))))
