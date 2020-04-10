#!/bin/bash -x

# Add github version of CMake into PATH
touch env_pegase_engine
echo "PATH=$HOME/git/CMake/bin:$PATH" >> env_pegase_engine
source env_pegase_engine

cd thirdparty
if [ -z "$(ls)" ]; then 
  echo "Directory empty"
  # Download boost::property_tree
  #git clone https://github.com/boostorg/property_tree.git
  wget https://dl.bintray.com/boostorg/release/1.72.0/source/boost_1_72_0.tar.gz
  # Download and install jsoncpp
  git clone https://github.com/open-source-parsers/jsoncpp.git
  cd jsoncpp
  cmake .
  make install
fi

# Install BOOST
tar -xvzf boost_1_72_0.tar.gz
cd boost_1_72_0
./bootstrap.sh
./b2

