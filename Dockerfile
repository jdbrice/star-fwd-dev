FROM starbnl/star-sw:latest-build

ARG STAR_SW_REF=fwd 
ARG STAR_CVS_REF=master
ARG STAR_BUILD_TYPE=Release

RUN yum install -y git
WORKDIR /tmp
RUN git clone https://github.com/jdbrice/star-sw.git star-sw-fwd
COPY StForwardTrackMaker/StRoot /tmp/star-sw-fwd/StRoot
RUN rm -rf /tmp/star-build && mkdir /tmp/star-build
WORKDIR /tmp/star-build

RUN cmake /tmp/star-sw-${STAR_SW_REF} -DSTAR_SRC=/tmp/star-cvs-${STAR_CVS_REF} \
    -DSTAR_PATCH=gcc540 -DCMAKE_INSTALL_PREFIX=/tmp/star-install \
    -DCERNLIB_ROOT=/cern/2006 -DCMAKE_BUILD_TYPE=${STAR_BUILD_TYPE} \
 && make StgMaker -j $(nproc) \
 && make install

