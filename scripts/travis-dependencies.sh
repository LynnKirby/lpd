#!/bin/bash

if [[ $TRAVIS_OS_NAME == 'osx' ]]; then
    HOMEBREW_NO_AUTO_UPDATE=1
    brew update
    brew install ninja pipenv
    pipenv install
else
    sudo apt-get update -q
    sudo apt-get install -q ninja-build python-pip
    pip install -U pip
    pip install pipenv --user
    pipenv install
    pipenv run python -c "import pkg_resources; print(list(pkg_resources.iter_entry_points('bfg9000.backends')))"
fi
