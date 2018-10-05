#!/bin/sh

interval=5
logfile=/home/mcadmin/Github/C++_Code/Posix/pool1/memory_monitor.txt

while [ 1 ]
do
	echo "-----------------------`date +%Y%m%d%H%M%S`-----------------------" >> $logfile
	free >> $logfile
	echo >> $logfile
	ps aux | sort -k6nr | head -n 100 >> $logfile
	echo >> $logfile
        processes=`ps -ef| awk '{ print $2,$8 }' |grep -v "\[" |grep -v grep |grep -v pmap|grep -v PID|uniq|cut -d" " -f1`
        for psid in $processes
        do
                 value=`pmap -d $psid |grep write |sed "s/.*private: //" |sed "s/K.*//"|awk '{ print $1 }' 2>>/var/tmp/error.log`
                 echo -e "pid: ${psid}\t${value}K" >> $logfile
        done
        sleep $interval
done
