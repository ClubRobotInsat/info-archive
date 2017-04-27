#!/bin/sh

ip_ssh=`arp -a | grep enp4s0 | cut -d ' ' -f 2 | cut -c 2-12 `

echo "ssh invocation: pi@" $ip_ssh
echo "The password is : robot"
ssh pi@$ip_ssh
