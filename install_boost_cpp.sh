#!/bin/sh
#
# Boost setup, loosely inspired by 'cpp-netlib/install-boost.sh'

here=$( cd `dirname "$0"` && pwd )

echo "+-- $0"

boost_modular_dir="$here/misc/boost"
if ! cd "$boost_modular_dir" && test -e .git && git describe ; then
	echo "+- $0: Couldn't get into \$boost_modular_dir=\"$boost_modular_dir\", failed."
	exit 127
fi

if [ ! -x bootstrap.sh ]; then
	echo "+- Couldn't find the bootstrap.sh script, failed."
	exit 126
fi

boost_last_versions=( $(git tag | grep -P '^boost-(\d\.?)*$' | sort -Vr | head -n5) )
boost_most_recent_version=${boost_last_versions[0]}
boost_cpp_version=$(git describe)

echo "|"
echo "| FYI: Last Boost C++ version tags: ${boost_last_versions[*]}"
echo "| FYI: Last Boost C++ version would be: $boost_most_recent_version"
echo "| FYI: “ locally checked-out version ” : $boost_cpp_version"
echo "|"

echo "| Git show-branch between locally checked-out code and the “ most recent version ” :"
git show-branch HEAD refs/tags/$boost_most_recent_version | sed -e 's/^/|  &/'

echo "|"
echo "| Git work-tree status :"
git status | sed -e 's/^/|  &/'

#boost_install_target_dir="${HOME}/${boost_cpp_version}-${CC}"
boost_install_target_dir="${here}/local/${boost_cpp_version}-${CC}"

echo "|"
echo "| \$CC = $CC"
echo "| \$CXX = $CXX"
echo "|"
echo "| Installation target directory (--prefix=...) will be: $boost_install_target_dir"

if [ -d "$boost_install_target_dir" ]; then
	read -p "| Target install. dir. exists, about to remove it, proceed ? (Ctrl-C to abort)"
	rm -rf "$boost_install_target_dir"
fi

echo "|"
read -p "| Ok, we're good to go, proceed ? (hit any key, or Ctrl-C to abort)"
echo "|"

if [ -x ./b2 ]; then
    echo "|"
    echo "| Found ./b2 : running ./b2 --clean"
    echo "|"

    if ! ./b2 --clean ; then
        retv=$?
        echo "|"
        echo "+- ./b2 --clean exited with status $retv, failed."
        exit $retv
    fi
fi

echo "|"
echo "| Running the ./bootstrap.sh script..."
echo "|"

if ! ./bootstrap.sh --prefix="$boost_install_target_dir" --with-toolset="${CC}";
then
	retv=$?
	echo "|"
	echo "+- ./bootstrap.sh ... exit status ain't ok: $retv, failed."
	exit $retv
fi

let processor_count=$( cat /proc/cpuinfo | grep -P '^processor\s*:\s*\d+' | wc -l )
let jobs_count="$processor_count - 1"
if [ "0$jobs_count" -lt 1 ]; then
	jobs_count=1
fi

echo "|"
echo "| Found out we have $processor_count CPUs ;"
echo "| we'll set -j$jobs_count (max. simultaneous jobs run by ./b2)."
echo "|"

echo "|"
echo "| Ok, proceeding with the ./b2 “thing” (compilation & installation)..."
echo "|"

time \
	./b2 -j$jobs_count --stagedir=. --layout=tagged variant=debug,release link=shared threading=multi address-model=64 cxxflags='-std=c++11' install

retv=$?
if [ $retv -gt 0 ]; then
	echo "|"
	echo "+- ./b2 ... exited with status $retv, failed."
	exit $retv
fi

echo "|"
echo "+--- $0: FINISHED ;-"
echo
echo

exit
