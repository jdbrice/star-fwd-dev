#/bin/bash
source star_env

nevents=${1:-1000}
filename=${2:-"testg.fzd"}
config=${3:-"config.xml"}
echo root4star -b -q -l 'simple.C( '"$nevents"', "'"$filename"'", "'"$config"'" )'
root4star -b -q -l 'simple.C( '"$nevents"', "'"$filename"'", "'"$config"'" )'