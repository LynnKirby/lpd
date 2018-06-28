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
    # FIXME: for some reason patchelf-wrapper fails to install on Travis
    NO_PATCHELF=1 pipenv install
fi
