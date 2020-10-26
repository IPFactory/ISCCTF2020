FROM ubuntu:18.04

ENV DEBIAN_FRONTEND noninteractive

RUN apt-get -y update --fix-missing && apt-get -y upgrade
RUN apt-get -y install xinetd libcap2-bin
RUN groupadd -r misc && useradd -r -g misc misc

ADD challenge/misc.xinetd /etc/xinetd.d/misc
ADD challenge/init.sh    /etc/init.sh
ADD challenge/redir.sh   /home/misc/redir.sh
RUN chmod 550        /home/misc/redir.sh
RUN chmod 700        /etc/init.sh
RUN chmod 1733 /tmp /var/tmp /dev/shm

ADD challenge/flag.txt   /home/misc
ADD challenge/chall      /home/misc
RUN chmod 440 /home/misc/flag.txt
RUN chmod 550 /home/misc/chall
RUN mkdir /home/misc/jail
ADD challenge/src.c      /home/misc/jail/

RUN setcap cap_sys_chroot+ep /home/misc/chall

RUN chown -R root:misc /home/misc

RUN ls /home/misc -lh

RUN service xinetd restart
