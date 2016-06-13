#!/bin/bash

cd misc/boost

git describe

./bootstrap.sh --prefix="${HOME}/$(git describe --abbrev=0)-${CC}" --with-toolset=${CC}

time ./b2 --stagedir=. --layout=tagged variant=debug,release link=shared threading=multi address-model=64 cxxflags='-std=c++14' install      

##

cd misc/cpp-netlib/

git describe

mkcd build

# Note that having the env. var. BOOST_ROOT won't do it, we have to pass the arg. -DBOOST_ROOT=... (unfortunately).
# Note: see also their build.sh shell script.
cmake -DCMAKE_BUILD_TYPE=Debug -DBOOST_ROOT=$BOOST_ROOT ..

