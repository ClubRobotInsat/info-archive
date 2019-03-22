#!/bin/bash

# Validation d'une adresse IP
# Proviens de https://unix.stackexchange.com/a/111846
function validateIP()
{
	local ip=$1
	local stat=1
	if [[ $ip =~ ^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$ ]]; then
		OIFS=$IFS
		IFS='.'
		ip=($ip)
		IFS=$OIFS
		[[ ${ip[0]} -le 255 && ${ip[1]} -le 255 \
			&& ${ip[2]} -le 255 && ${ip[3]} -le 255 ]]
		stat=$?
	fi
	return $stat
}
RED="\x1B[31m"
GREEN="\x1B[32m"
BLUE="\x1B[34m"
RST="\x1B[0m"

# Ligne à modifier si on a une interface Internet avec un nom bizarre (dans 'send_file_to_rpi' aussi)
ip_ssh=$(arp -a | grep "enp3s0\|enp4s0\|eth0\|enx00e02f7000b5" | cut -d ' ' -f 2 | cut -d '(' -f 2 | cut -d ')' -f 1)

# Verification de l'adresse IP
validateIP $ip_ssh
if [ $? -ne 0 ]; then
	echo -e "$RED Problème de connexion au raspi, vérifier le câble ethernet. $RST"
	exit 1
fi

echo -e "IP du raspberry PI : $GREEN $ip_ssh $RST"

ssh pi@$ip_ssh
