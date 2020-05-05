#!/usr/bin/env bash
set -euo pipefail

############################################
## dev.sh 
############################################
# this script runs a docker container for development / running fwd tracking code
#
# it mounts the subdirectories of this project into the container for inter op
#
# 
# cmake /tmp/star-sw-master -DSTAR_SRC=/tmp/star-cvs-master -DCMAKE_INSTALL_PREFIX=/tmp/star-install -DCERNLIB_ROOT=/cern/2006 -DCMAKE_BUILD_TYPE=Release 

# STAR_PATCH=""
# STAR_BUILD_TYPE=Release
# cmake /tmp/star-sw -DSTAR_SRC=/tmp/star-cvs \
#     -DSTAR_PATCH=${STAR_PATCH} -DCMAKE_INSTALL_PREFIX=/tmp/star-install \
#     -DCERNLIB_ROOT=/cern/2006 -DCMAKE_BUILD_TYPE=${STAR_BUILD_TYPE}

echo docker run --rm -ti $@ -w /tmp/work -v `pwd`/work:/tmp/work -v `pwd`/star-sw/StRoot:/tmp/star-sw-master/StRoot starbnl/star-sw:latest-build bash
docker run --rm -ti $@ -w /tmp/work -v `pwd`/work:/tmp/work -v `pwd`/star-sw/StRoot:/tmp/star-sw-master/StRoot starbnl/star-sw:latest-build bash 


# docker run --rm -ti $@ -w /tmp/work -v `pwd`/work:/tmp/work -v `pwd`/star-sw/StRoot:/tmp/star-sw-master/StRoot starbnl/star-sw:latest-build bash

# if [ -f ./star-cvs/StarVMC/Geometry/StarGeo.xml ]; then
# 	mount_cvs_geom="-v `pwd`/star-cvs/StarVMC:/tmp/star-cvs-master/StarVMC"
# else
# 	mount_cvs_geom=""
# fi

# echo docker run --rm -ti ${mount_cvs_geom} starbnl/star-sw:latest-build bash
# docker run --rm -ti ${mount_cvs_geom} starbnl/star-sw:latest-build bash
