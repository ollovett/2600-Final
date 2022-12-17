#!/bin/bash
# chkconfig: 345 99 10
# Description: auto start
#
case "$1" in
 'start')
    ./autoplay.sh
 'stop')
   echo "stop";;
esac