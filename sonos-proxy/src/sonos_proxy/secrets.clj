(ns sonos-proxy.secrets
    "Secrets retrieval functions"
    (:require [clojure.string :as str]))

(def vault-keys
    "Key Vaullt keys for required secrets"
    {:msa-client-id         "sonosProxyMsaClientId"
     :msa-client-secret     "sonosProxyMsaClientSecret"
     :msa-user-id-whitelist "sonosProxyUserIdWhiteList" 
     :base-uri              "sonosProxyBaseUri" })

(defn error
    "Complain about problems retrieving secrets"
    [key reason]
    (throw
        (Exception. 
            (str "Error retrieving secret: " key ", "
                 "Reason: " reason))))

(defn get-secret
    "Relevant Key Vault secrets should be exposed to environment, so
    lookup secrets in env vars."
    [key]
    (if-let [vault-key (vault-keys key)]
        (or (System/getenv vault-key)
            (error key :env-var-not-set))
        (error key :invalid-vault-key)))