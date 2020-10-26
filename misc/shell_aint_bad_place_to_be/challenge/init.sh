#! /bin/bash
service xinetd restart
while :
do
    /bin/sleep 600
    cd /home/misc/jail && ls | grep -v src.c |xargs rm -rf;
    cd /home/misc && ls |grep -v -e redir.sh -e jail -e flag.txt -e chall |xargs rm -rf;
done
