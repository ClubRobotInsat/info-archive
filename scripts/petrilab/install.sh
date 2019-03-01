#!/bin/bash

help() {
	echo "Petrilab is currently distributed for Ubuntu [14.04|17.04|18.04]."
	exit 1
}

RELEASE=$(lsb_release -si)
if [[ "$RELEASE" != "Ubuntu" ]]; then
	help
fi

VERSION=$(lsb_release -d | cut -f2 | cut -d' ' -f2)
VERSION="${VERSION:0:5}" # Cas où la version est "18.04.1" au lieu de "18.04"
if [[ "$VERSION" != "14.04" && "$VERSION" != "17.04" && "$VERSION" != "18.04" ]]; then
	help
fi

sudo apt-key add - < petrilab_pub.pgp
echo "deb $(gpg server.link.gpg > /dev/null && cat server.link) $(lsb_release -c | rev | cut -f1 | rev) main" | sudo tee /etc/apt/sources.list.d/petrilab_$(lsb_release -c | rev | cut -f1 | rev).list && sudo apt-get update && sudo apt-get install petrilab

