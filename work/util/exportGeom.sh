

root -l -b /tmp/star-cvs/StarVMC/Geometry/macros/loadAgML.C  <<EEE
construct( "dev2021" );
gGeoManager->Export( "dev2021.C" );
.q
EEE
