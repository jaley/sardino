(ns sonos-proxy.api
    (:require [compojure.core           :refer [routes POST GET]]
              [compojure.route          :refer [not-found resources]]
              [ring.middleware.defaults :refer [wrap-defaults site-defaults]]
              [ring.util.response       :as resp]
              [ring.util.json-response  :as json]
              [cheshire.generate        :as gen]
              [taoensso.timbre          :as log]
              
              [sonos-proxy.auth         :as auth]
              [sonos-proxy.sonos        :as sonos])
    (:import  [org.joda.time DateTime]))

;; Token expiries are DateTime objects, which would be useful
;; to pass over APIs sometimes
(extend-protocol gen/JSONable
    DateTime
    (to-json [dt gen]
        (gen/write-string gen (str dt))))

(defn static
    "Return a static page from resources"
    [html]
    (-> html
        (resp/resource-response {:root "html"})
        (resp/content-type "text/html")))

(defn build-routes
    "Compose routes and apply global settings"
    []
    (let [settings (-> site-defaults (assoc-in [:session :cookie-attrs :same-site] :lax))]
        (-> (routes
                (GET "/" [] (static "main.html"))
                (resources "/css" {:root "css"})
                (resources "/js" {:root "js"})
                (GET "/auth/msa/logout" []
                    (auth/msa-logout-response))
                (GET "/auth/sonos/logout" []
                    (auth/sonos-logout-response))
                (auth/wrap-shared-secret
                    (routes
                        (GET "/api/groups" []
                            (json/json-response
                                (sonos/groups)))
                        (GET "/api/:group-id/volume" [group-id]
                            (json/json-response
                                (sonos/get-volume group-id)))
                        (POST "/api/:group-id/volume" [group-id volume]
                            (json/json-response
                                (sonos/set-volume group-id volume)))))
                (auth/wrap-oauth
                    (routes
                        (GET "/api/tokens" req
                            (json/json-response (:oauth2/access-tokens req)))
                        (GET "/api/sonos-init" req
                            (sonos/init! (get-in req [:oauth2/access-tokens :sonos]))
                            (resp/redirect "/"))))
                (not-found "Not Found"))
            (wrap-defaults settings))))
