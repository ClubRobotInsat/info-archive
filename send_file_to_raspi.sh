#!/bin/sh

file=$1
ip_ssh=`arp -a | grep enp4s0 | cut -d ' ' -f 2 | cut -c 2-12 `

echo "Rsync invocation: rsync" $file "pi@" $ip_ssh ":~/Desktop"
echo "The password is : robot"
rsync $file pi@$ip_ssh:~/Desktop
