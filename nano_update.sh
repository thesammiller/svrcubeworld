git pull
cp ../CMakeLists.use ./CMakeLists.txt
source ./extern/ACE_wrappers/build/default/.env
./extern/ACE_wrappers/build/default/TAO/TAO_IDL/tao_idl test.idl
./build.sh
