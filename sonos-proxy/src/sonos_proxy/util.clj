(ns sonos-proxy.util)

(defn forbidden
    []
    {:status 403
     :headers {"Content-Type" "text/html; charset=utf-8"}
     :body "Forbidden"})

(defn unauthorized
    []
    {:status 401
     :headers {"Content-Type" "text/html; charset=utf-8"}
     :body "Unauthorized"})
