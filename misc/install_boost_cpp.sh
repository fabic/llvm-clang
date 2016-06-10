#!/bin/bash

./bootstrap.sh --prefix="${HOME}/$(git describe --abbrev=0)-${CC}" --with-toolset=${CC}


time ./b2 --stagedir=. --layout=tagged variant=debug,release link=shared threading=multi address-model=64 cxxflags='-std=c++14' install      

