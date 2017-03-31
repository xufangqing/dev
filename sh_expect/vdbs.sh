#!/bin/bash


HOMESHARE=/home/share
IMAGE_NAME=itran/vdbs
AS_IP=172.16.0.1
CNAME=vdbs
docker rm -f "${CNAME}"
docker run -it -d --privileged="true" -e DBCONF="ip=${AS_IP}:3000" --name="${CNAME}" --hostname="${CNAME}" -v ${HOMESHARE}:/home/share ${IMAGE_NAME}


