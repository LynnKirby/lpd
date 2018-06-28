#!/bin/bash

if [[ $TRAVIS_OS_NAME == 'osx' ]]; then
    HOMEBREW_NO_AUTO_UPDATE=1
    brew update
    brew install ninja pipenv
    pipenv install
else
    sudo apt-get update -q
    sudo apt-get install -q ninja-build python-pip
    pip install pipenv --user
    NO_PATCHELF=1 pipenv install
    pipenv graph
fi
