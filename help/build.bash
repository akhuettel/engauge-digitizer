# This bash script creates .qch and .qhc output files. Optional argument is target directory.
# Since bash has different locations depending on the operating system, no #! line is included.
#
# Usage: .../build_qt5_12_0.bash [destdir]
# where:
#   [destdir]   Optional destination directory relative to the directory containing this script.
#               Default is ../bin/documentation
#
# This version is for qt 5.12.0 or newer, for which qcollectiongenerator is deprecated in favor of
# qhelpgenerator. Earlier versions of qcollectiongenerator fail since qch file is NOT generated
VER=`qmake6 -query QT_VERSION`
MAJOR=$(echo $VER | cut -f1 -d. )
MINOR=$(echo $VER | cut -f2 -d. )
SUBMINOR=$(echo $VER | cut -f3 -d. )
MAJORMIN=5
MINORMIN=12
SUBMINORMIN=0
SUCCESS=1
if (( MAJOR < MAJORMIN )); then
    SUCCESS=0
elif (( MAJOR == MAJORMIN )); then
    if (( MINOR < MINORMIN )); then
	SUCCESS=0
    elif (( MINOR == MINORMIN )); then
	if (( SUBMINOR < SUBMINORMIN )); then
	    SUCCESS=0
	fi
    fi
fi    
if (( SUCCESS == 0 )); then
    echo "Qt version must be $MAJORMIN.$MINORMIN.$SUBMINORMIN or newer. Use another build script"
    exit
fi    
# Run this script in the 'help' subdirectory to prevent confusion about $1 argument being relative
# script directory or current working directory. Flathub build script likes the 'cd' command in
# this script rather than outside
SCRIPTDIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"
WORKPATH=`pwd`
WORKDIR=`basename $WORKPATH`
if [[ $WORKDIR != "help" ]]; then
    # Changing to script directory
    cd $SCRIPTDIR
fi

if [ $# -eq 0 ]; then
    DESTDIR=../bin/documentation
else
    DESTDIR=$1
fi

# Specifying ../bin/documentation directory here creates files in this directory
# AND ../bin/documentation (with different sizes) so we build into the local directory.
# Outputs:
#   engauge.qch 3MB binary file
#   engauge.qhc 32K SQLite file

# qhelpgenerator is not always on PATH: Debian and Ubuntu install it into the Qt
# libexec directory (qt6-documentation-tools provides
# /usr/lib/qt6/libexec/qhelpgenerator). Without this the call below just printed
# 'command not found', the following mv failed as well, and the script still
# exited 0, so the help collection was reported as built while no .qch/.qhc file
# existed at all. Resolve the tool explicitly and stop with an actionable
# message when it is really missing.
QHELPGENERATOR=`command -v qhelpgenerator`
if [ -z "$QHELPGENERATOR" ]; then
    QHELPGENERATOR=`command -v qhelpgenerator6`
fi
if [ -z "$QHELPGENERATOR" ]; then
    QTLIBEXECS=`qmake6 -query QT_INSTALL_LIBEXECS 2>/dev/null`
    for candidate in "$QTLIBEXECS/qhelpgenerator" \
                     "$QTLIBEXECS/qhelpgenerator6" \
                     "/usr/lib/qt6/libexec/qhelpgenerator" \
                     "/usr/lib/x86_64-linux-gnu/qt6/libexec/qhelpgenerator"; do
        if [ -x "$candidate" ]; then
            QHELPGENERATOR=$candidate
            break
        fi
    done
fi
if [ -z "$QHELPGENERATOR" ]; then
    echo "qhelpgenerator was not found. Install the Qt documentation tools (Ubuntu: qt6-documentation-tools)"
    exit 1
fi

"$QHELPGENERATOR" engauge.qhp || exit 1

# Move to target directory which is relative to the help subdirectory. OSX does not use qhc file
mkdir -p $DESTDIR
mv engauge.qch $DESTDIR || exit 1
if [ -f engauge.qhc ]; then
    mv engauge.qhc $DESTDIR || exit 1
fi

