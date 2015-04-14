#!/bin/bash

cd ./bin

cmake -GNinja $@ ../

/usr/local/bin/ninja

cd ..
