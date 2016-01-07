#!/bin/bash

dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

oldpwd="$OLDPWD"

cd "$dir"

git submodule update --init
petri/bootstrap.sh
#petri/Editor/build.sh
xbuild petri/Editor/Projects/Petri.csproj
mkdir -p robot/Principal/petri/src

cd "$oldpwd"
