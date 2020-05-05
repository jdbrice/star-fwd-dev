#!/usr/bin/env bash
set -euo pipefail

############################################
## dev.sh 
############################################
# this script runs a docker container for development / running fwd tracking code
# it mounts the subdirectories of this project into the container for inter op
#

echo docker run --rm -ti $@ -w /tmp/work -v `pwd`/work:/tmp/work -v `pwd`/star-sw/StRoot:/tmp/star-sw-master/StRoot jdbburg/star-sw-buil:v041820d bash
docker run --rm -ti $@ -w /tmp/work -v `pwd`/work:/tmp/work -v `pwd`/star-sw/StRoot:/tmp/star-sw-master/StRoot jdbburg/star-sw-build:v041820 bash 
