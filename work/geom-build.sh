#!/usr/bin/env bash
# This script rebuilds the STAR Geometry with an updated FstmGeo
# must be run from inside the docker container

# copy over the modified XML from working dir
cp /tmp/work/FstmGeo.xml /tmp/star-cvs/StarVMC/Geometry/FstmGeo/FstmGeo.xml
cp /tmp/work/StarGeo.xml /tmp/star-cvs/StarVMC/Geometry/StarGeo.xml
cd /tmp/star-build

rm -f StarVMC/StarGeometry/*
rm -f StarVMC/xgeometry/*

STAR_PATCH=""
STAR_BUILD_TYPE=Release
cmake /tmp/star-sw -DSTAR_SRC=/tmp/star-cvs \
    -DSTAR_PATCH=${STAR_PATCH} -DCMAKE_INSTALL_PREFIX=/tmp/star-install \
    -DCERNLIB_ROOT=/cern/2006 -DCMAKE_BUILD_TYPE=${STAR_BUILD_TYPE}

make StarGeometry
make xgeometry
make install
# manually instal the new lib
# cp StarVMC/xgeometry/libxgeometry.so /tmp/star-install/lib/libxgeometry.so

cd /tmp/work