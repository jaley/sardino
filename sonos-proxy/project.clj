(defproject sonos-proxy "0.1.0-SNAPSHOT"
  :description "Web App to proxy Sonos Control API calls"
  :license {:name "EPL-2.0 OR GPL-2.0-or-later WITH Classpath-exception-2.0"
            :url "https://www.eclipse.org/legal/epl-2.0/"}
  :dependencies [[org.clojure/clojure "1.10.1"]
                 [com.taoensso/timbre "4.10.0"]
                 [http-kit "2.3.0"]
                 [compojure "1.6.1"]
                 [ring-oauth2 "0.1.5"]
                 [ring/ring-defaults "0.3.2"]
                 [ring-logger-timbre "0.7.6"]
                 [com.microsoft.graph/microsoft-graph "1.7.1"]]
  :repl-options {:init-ns sonos-proxy.core}
  
  :uberjar
   {:aot  :all}
   
   :main sonos-proxy.core)
