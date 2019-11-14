#!/bin/bash

docker run --rm -d --name test amirsamary/iris-crc-module-example:test 
sleep 3
docker exec -it test iris session iris -UAPP ^TEST
docker stop test
