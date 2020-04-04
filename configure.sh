#!/bin/bash -x

# Add github version of CMake into PATH
export PATH=/Users/aurele/git/CMake/bin:$PATH

cd thirdparty
rm -rf *
git clone https://github.com/open-source-parsers/jsoncpp.git
