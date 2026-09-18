#!/bin/bash

PROXY="localhost:15213"
URL="http://neverssl.com/"

echo "Starting 20 concurrent requests..."

for i in {1..20}; do
    curl -s -x "$PROXY" "$URL" > /dev/null &
done

wait

echo "All requests finished!"
