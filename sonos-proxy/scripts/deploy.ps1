$repo = "jaley.azurecr.io/sonos-proxy"
$tag = "latest-stable"

$image = @($repo, $tag) -join ':'

lein uberjar

docker build `
    -t $image `
    .

docker push `
    $image
