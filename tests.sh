#!/bin/sh

./out/build/ovr_test
./out/build/gl_test
#!/bin/sh

./out/build/svr_cubeworld &
sleep 5
./out/build/client
