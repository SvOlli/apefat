#!/bin/sh

MXE=/opt/mxe
cd ${0%/*}
builddir=../../build-mxe
rm -rf ${builddir}
mkdir -p ${builddir}
cd ${builddir}

cmake ../src -DCMAKE_VERBOSE_MAKEFILE=true -DCMAKE_TOOLCHAIN_FILE=${MXE}/usr/i686-pc-mingw32/share/cmake/mxe-conf.cmake
make

