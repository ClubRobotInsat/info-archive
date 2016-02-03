#!/bin/bash

dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

oldpwd="$OLDPWD"

cd "$dir"

git submodule update --init
petri/bootstrap.sh
make -C petri clean
make -C petri editor lib
mkdir -p robot/Principal/petri/src

cd "$oldpwd"
