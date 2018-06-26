#!/bin/bash

if [[ $TRAVIS_OS_NAME == 'osx' ]]; then
    brew install ninja
    pip2 install bfg9000
else
    sudo apt-get update -q
    sudo apt-get install -q ninja-build python-pip
    NO_PATCHELF=1 sudo pip install bfg9000
fi
