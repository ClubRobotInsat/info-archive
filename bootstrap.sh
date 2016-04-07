#!/bin/bash

dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

oldpwd="$OLDPWD"

cd "$dir"

git submodule update --init
petri/bootstrap.sh
make -C petri editor lib -j8
mkdir -p robot/Principal/petri/src

cd "$oldpwd"
