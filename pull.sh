#!/bin/bash

dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
oldpwd=$(pwd)

cd "$dir"

git pull

echo "Updating submodules and compiling petri…"
"$dir"/bootstrap.sh

cd ../bin

echo "Compiling the code…"

if [ -f "Makefile" ]; then
    make
elif [ -f "build.ninja" ]; then
    ninja
else
    echo "Wrong build system…"
fi

cd "$oldpwd"

