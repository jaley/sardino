FROM alpine:3.11

RUN apk --update add openjdk8-jre
ADD target/sonos-proxy-0.1.0-SNAPSHOT-standalone.jar /opt/sonos-proxy/sonos-proxy-standalone.jar

EXPOSE 9090

ENTRYPOINT ["/usr/bin/java", "-jar", "/opt/sonos-proxy/sonos-proxy-standalone.jar"]