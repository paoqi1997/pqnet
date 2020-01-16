#!/bin/sh

pname='daemon_test'
pid=`ps -ef|grep $pname|grep -v grep|awk '{print $2}'`

case $1 in
    'start')
        if [ -z $pid ]; then
            ./$pname
        fi
        ;;
    'stop')
        if [ -n "$pid" ]; then
            kill $pid
            rm daemon.log
        fi
        ;;
    'status')
        if [ -n "$pid" ]; then
            echo 'Running!'
        else
            echo 'Stopped!'
        fi
        ;;
    *)
        echo 'Usage:'
        echo '  sh main.sh [command]'
        echo 'Commands:'
        echo "  start   start $pname"
        echo "  stop    stop $pname"
        echo '  status  view the status'
        ;;
esac
