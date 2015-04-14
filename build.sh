#!/bin/bash

cd ./bin

cmake -GNinja $@ ../

ninja

cd ..
