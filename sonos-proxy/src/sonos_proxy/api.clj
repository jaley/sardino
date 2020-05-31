(ns sonos-proxy.api
    (:require [compojure.core           :refer [routes POST GET]]
              [compojure.route          :refer [not-found resources]]
              [ring.middleware.defaults :refer [wrap-defaults site-defaults]]
              [ring.util.response       :as resp]
              [ring.util.json-response  :as json]
              [cheshire.generate        :as gen]
              [taoensso.timbre          :as log]
              
              [sonos-proxy.auth         :as auth])
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
                (GET "/auth/msa/logout" [] (not-found "[not implemented] msa logout"))
                (GET "/auth/sonos/logout" [] (not-found "[not implemented] sonos logout"))
                (auth/wrap-auth
                    (routes
                        (GET "/api/tokens" req
                            (json/json-response (:oauth2/access-tokens req)))))
                (not-found "Not Found"))
            (wrap-defaults settings))))
