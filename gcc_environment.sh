
HERE=$( cd `dirname "${BASH_SOURCE[0]}"` && pwd )

. $HERE/functions.sh


CC=gcc
CXX=g++
export CC CXX

BOOST_ROOT="$HOME/boost-1.61.0-gcc" 

if [ -d "$BOOST_ROOT/include" ];
then
	echo "FYI: Found Boost @ $BOOST_ROOT"

	export BOOST_ROOT

	BOOST_INCLUDE_DIRS="${BOOST_ROOT}/include"
	BOOST_LIBRARY_DIRS="${BOOST_ROOT}/lib"
	export BOOST_INCLUDE_DIRS BOOST_LIBRARY_DIRS

	pathprepend "$BOOST_INCLUDE_DIRS" CPLUS_INCLUDE_PATH
	pathprepend "$BOOST_LIBRARY_DIRS" LD_RUN_PATH


	#INCLUDE_PATH="$BOOST_ROOT/include:$INCLUDE_PATH"
	#export INCLUDE_PATH

	#fhs_path_setup_for "$BOOST_ROOT"

	export CPLUS_INCLUDE_PATH LD_RUN_PATH
else
    echo "FYI: couldn't find Boost @ \$BOOST_ROOT=$BOOST_ROOT  (unsetting env. var.)."
    unset BOOST_ROOT
fi

