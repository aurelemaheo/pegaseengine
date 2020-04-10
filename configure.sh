#!/bin/bash -x

# Add github version of CMake into PATH
touch env_pegase_engine
echo "PATH=$HOME/git/CMake/bin:$PATH" >> env_pegase_engine
source env_pegase_engine

cd thirdparty
rm -rf *
# Download boost::property_tree
git clone https://github.com/boostorg/property_tree.git

# Download and install jsoncpp
git clone https://github.com/open-source-parsers/jsoncpp.git
cd jsoncpp
cmake .
make install
