#!/bin/sh
#
# Boost setup, loosely inspired by 'cpp-netlib/install-boost.sh'

here=$( cd `dirname "$0"` && pwd )

boost_modular_dir="$here/boost"
if ! cd "$boost_modular_dir" && test -e .git && git describe ; then
	echo "$0: Couldn't get into \$boost_modular_dir=\"$boost_modular_dir\""
	exit 127
fi

if [ ! -x bootstrap.sh ] || [ ! -x b2 ]; then
	echo "Couldn't find either one of bootstrap.sh or b2"
	exit 126
fi

boost_last_versions=( $(git tag | grep -P '^boost-(\d\.?)*$' | sort -Vr | head -n5) )
boost_most_recent_version=${boost_last_versions[0]}
boost_cpp_version=$(git describe)

echo "FYI: Last Boost C++ version tags: ${boost_last_versions[*]}"
echo "FYI: Last Boost C++ version would be: $boost_most_recent_version"

echo "Git show-branch between locally checked-out code and the “ most recent version ” :"
git show-branch HEAD refs/tags/$boost_most_recent_version || exit 125

echo
echo "Git work-tree status :"
git status || exit 124

boost_install_target_dir="${HOME}/${boost_cpp_version}-${CC}"

echo
echo "Installation target directory (--prefix=...) will be: $boost_install_target_dir"
echo "\$CC=$CC"
echo "\$CXX=$CXX"

echo
read -p "Ok, we're good to go, proceed ? (hit any key, or Ctrl-C to abort)"
echo

echo "Running the ./bootstrap.sh script..."

if ! ./bootstrap.sh --prefix="$boost_install_target_dir" --with-toolset="${CC}";
then
	retv=$?
	echo "FAILED: ./bootstrap.sh ... exit status ain't ok: $retv"
	exit $retv
fi

echo
echo "Ok, proceeding with the ./b2 thing (compilation & installation)"
echo

time \
	./b2 --stagedir=. --layout=tagged variant=debug,release link=shared threading=multi address-model=64 cxxflags='-std=c++11' install

retv=$?
if [ $retv -gt 0 ]; then
	echo
	echo "FAILED: ./b2 ... exited with status $retv"
	exit $retv
fi

echo
echo "$0: FINISHED ;-"
echo

exit

cd misc/cpp-netlib/

git describe

mkcd build

# Note that having the env. var. BOOST_ROOT won't do it, we have to pass the arg. -DBOOST_ROOT=... (unfortunately).
# Note: see also their build.sh shell script.
cmake -DCMAKE_BUILD_TYPE=Debug -DBOOST_ROOT=$BOOST_ROOT ..

