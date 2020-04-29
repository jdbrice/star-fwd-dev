cp /tmp/work/FstmGeo.xml /tmp/star-cvs/StarVMC/Geometry/FstmGeo/FstmGeo.xml
cd /tmp/star-build

STAR_PATCH=""
STAR_BUILD_TYPE=Release
cmake /tmp/star-sw -DSTAR_SRC=/tmp/star-cvs \
    -DSTAR_PATCH=${STAR_PATCH} -DCMAKE_INSTALL_PREFIX=/tmp/star-install \
    -DCERNLIB_ROOT=/cern/2006 -DCMAKE_BUILD_TYPE=${STAR_BUILD_TYPE}

make xgeometry
# manually instal the new lib
cp StarVMC/xgeometry/libxgeometry.so /tmp/star-install/lib/libxgeometry.so

cd /tmp/work