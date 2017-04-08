#!/bin/sh
raspivid -o - -n -t 0 -w 640 -h 360 -fps 12 | cvlc -vvv stream:///dev/stdin --sout '#standard{access=http,mux=ts,dst=:8090}' :demux=h264