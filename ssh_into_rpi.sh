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
RST="\x1B[0m"

ip_ssh=`arp -a | grep "enp4s0\|eth0" | cut -d ' ' -f 2 | cut -c 2-12`

# Verification de l'adresse IP
validateIP $ip_ssh
if [[ $? -ne 0 ]]
then 
	echo -e "$RED Problème de connexion au raspi, vérifier le câble ethernet. $RST"
	exit 1
fi

echo -e "IP du raspberry PI : $GREEN $ip_ssh $RST"
echo "Le mot de passe est robot."

ssh pi@$ip_ssh
