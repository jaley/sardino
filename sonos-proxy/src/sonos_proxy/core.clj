(ns sonos-proxy.core
  (:require [org.httpkit.server :as http]
            [taoensso.timbre    :as log]
            [ring.logger.timbre :as req-log]
            
            [sonos-proxy.api    :as api])
  (:gen-class))

(def config
  {:port       9090
   :ip         "0.0.0.0"
   :max-body   50000
   :thread     1
   :queue-size 20000 })

(defn start!
  "Start the proxy service running, using handler as routes"
  [handler]
  (http/run-server handler config))

(defn -main
  [& args]
  (log/info "Starting Sonos Proxy App")
  (start! (req-log/wrap-with-logger (api/build-routes)))
  (log/info "... App Running!"))
