
HERE=$( cd `dirname "${BASH_SOURCE[0]}"` && pwd )

. $HERE/functions.sh


CC=gcc
CXX=g++
export CC CXX

BOOST_ROOT="$HOME/boost-1.61.0-gcc" 
export BOOST_ROOT

#BOOST_INCLUDE_DIRS="${BOOST_ROOT}/include"
#BOOST_LIBRARY_DIRS="${BOOST_ROOT}/lib"
#export BOOST_INCLUDE_DIRS BOOST_LIBRARY_DIRS

#INCLUDE_PATH="$BOOST_ROOT/include:$INCLUDE_PATH"
#export INCLUDE_PATH

fhs_path_setup_for "$BOOST_ROOT"

export CPATH LIBRARY_PATH

