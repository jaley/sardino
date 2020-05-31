(ns sonos-proxy.util
    (:require [ring.util.json-response :refer [json-response]]
              [ring.util.response      :as resp]))

(defn forbidden
    "Returns a 403 Forbidden JSON respsonse"
    [msg]
    (-> {:reason msg}
        json-response
        (resp/status 403)))

(defn unauthorized
    "Returns a 401 Unauthorized JSON response"
    [msg]
    (-> {:reason msg}
        json-response
        (resp/status 401)))
