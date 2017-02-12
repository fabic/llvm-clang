#!/bin/bash
#
# Boost setup, loosely inspired by 'cpp-netlib/install-boost.sh'

here=$( cd `dirname "$0"`/.. && pwd )

. "$here/functions.sh"

echo "+-- $0"

boost_most_recent_version="1.63.0"

arg1="${1:-$boost_most_recent_version}"

# 2nd arg. may be the target install dir.
# if not specified => it will be inferred later on as local/boost_.../
boost_install_target_dir="${2:-}"

# 1st argument may be a boost version.
if [ "X${arg1/?.??.*/YYYY}X" == "XYYYYX" ]; then
    boost_cpp_version="$arg1"
    boost_version_blankee="${boost_cpp_version//./_}"
    boost_tarball_filename="boost_$boost_version_blankee.tar.bz2"
    boost_tarball_url="https://downloads.sourceforge.net/project/boost/boost/$boost_cpp_version/$boost_tarball_filename"
    boost_sources_dir="$here/misc/boost_$boost_version_blankee/"

    echo "+- Asked for Boost C++ version $boost_cpp_version ($boost_version_blankee)"

    # If tarball not yet extracted into misc/
    if [ ! -d "$boost_sources_dir" ]; then
        # Find out if we have a tarball into tmp/
        if [ ! -f "$here/tmp/$boost_tarball_filename" ]; then
            # Fetch tarball
            if ! wget "$boost_tarball_url" -C "$here/tmp" ; then
                echo "| ERROR: wget exited with status code $?"
                echo "|        failed to download the Boost C++ tarball '$boost_tarball_url'."
                echo "|        (you may retrieve it yourself and extract it under misc/ )"
                echo "|"
                echo "+- $0: failed to download the Boost C++ tarball '$boost_tarball_url'."
                echo
                exit 1
            fi
        else
            echo "| Found existing tarball $here/tmp/$boost_tarball_filename, ok"
        fi

        echo "| Extractind sources tarball..."

        # Extract tarball
        if ! tar -xf "$here/tmp/$boost_tarball_filename" -C "$here/misc/" ; then
            echo "| ERROR: tarball extraction failed (exit status: $?)"
            echo "|"
            echo "+- $0: failed -_-"
            exit 2
        fi
    else
        echo "| Found Boost C++ sources dir. '$boost_sources_dir'"
    fi
#
## Build boost modular ???
#
elif [ "$arg1" == "modular" ];
then
    boost_modular_dir="$here/misc/boost"
    boost_sources_dir="$boost_modular_dir"

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
#
## Arg. “ any ” searches for untar-ed sources under misc/boost_X_YY_ZZ/
#
elif [ "x$arg1" == "xany" ]; then
    boost_versions=( $(ls -1d misc/boost_?_??_* | sort -r ) )
    boost_cpp_version="${boost_versions[0]}"

    if [ -z "$boost_cpp_version" ]; then
        echo "| FAIL: Couldn't find boost sources..."
        exit 5
    fi

    # TODO: UNFINISHED...

    echo "|"
    echo "| FYI: Found the following Boost C++ sources : ${boost_versions[*]}"

    echo " FIXME: UNFINISHED..."
    exit 127
#
## 3rd possibility: first command line argument as the path to the untar-ed Boost sources
#
else
    boost_sources_dir="$arg1"

    if [ ! -d "$boost_sources_dir" ]; then
        echo "| FAIL: \$boost_sources_dir = '$boost_sources_dir' is not a directory."
        exit 127
    fi

    echo "| Ok, $boost_sources_dir exists."
    echo "| Guessing Boost C++ version from the directory name."

    boost_cpp_version="$( basename "$boost_sources_dir" )"
    boost_cpp_version="${boost_cpp_version#boost_}"
    boost_cpp_version="${boost_cpp_version//_/.}"

    if [ -z "$boost_cpp_version" ]; then
        echo "| FAIL: Couldn't determine the Boost version at dir. '$boost_sources_dir'."
        exit 126
    fi

    echo "|"
    echo "| Ok, guessed Boost C++ version: $boost_cpp_version"
    echo "|"

fi

echo "|"
echo "| Ch. dir. into '$boost_sources_dir/'"
cd "$boost_sources_dir" \
  || exit 125
echo "| Ok, we're in..."

#boost_install_target_dir="${HOME}/${boost_cpp_version}-${CC}"
boost_install_target_dir="${boost_install_target_dir:-$here/local/boost-$boost_cpp_version}"

# Suffix with compiler 
if [ ! -z "$CC" ]; then
    boost_install_target_dir="${boost_install_target_dir}-`basename "$CC"`"
else
    read -p "| WARNING: \$CC is not defined or empty, hence we're _not_ suffixing the target installation dir. with it (CONTINUE ?)"
fi

echo "|"
echo "| \$CC = $CC"
echo "| \$CXX = $CXX"
echo "| \$CFLAGS   = $CFLAGS"
echo "| \$CXXFLAGS = $CXXFLAGS"
echo "| \$CPATH = $CPATH"
echo "| \$CPLUS_INCLUDE_PATH = $CPLUS_INCLUDE_PATH"
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

#--stagedir=.
#--build-dir=

time \
    ./b2 -j$jobs_count --layout=tagged --build-type=complete \
        variant=debug,release \
        link=shared,static \
        threading=single,multi \
        runtime-link=shared \
        address-model=64 \
        cxxflags='-std=c++1z -stdlib=libc++' \
        linkflags='-stdlib=libc++' \
            install

retv=$?
if [ $retv -gt 0 ]; then
    echo "|"
    echo "| Ouch! build failed : |  note that if only a few targets failed, you may"
    echo "| go into $boost_install_target_dir/ and retry to run ./b2 && ./b2 install"
    echo "|"
    echo "+- ./b2 ... exited with status $retv, failed."
    exit $retv
fi

echo "|"
echo "+--- $0: FINISHED ;-"
echo
echo

exit
