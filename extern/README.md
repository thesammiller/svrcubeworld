# Note
Future work includes using the ACE+TAO library as a submodule. 
However, at this point in time, it needs to be installed separately. 
Please see the instructions below for the Ubuntu platform. This should also work on other Linux/Unix (Mac OS) systems.
Windows systems should be able to install based on changing the OS-specific parameters (refer to the developer's guide PDF).
# Install ACE+TAO on Ubuntu
- Instructions page references: https://objectcomputing.com/files/3415/3090/8836/TAO_Dev_Guide_2_2a_p7.pdf
- Download the latest minor relase `Full` from https://download.dre.vanderbilt.edu/
- Unzip the downloaded file into the extern directory.
- p. 1106 - Create a build tree -- in the unzipped directory `ACE_wrappers` run `MPC/clone_build_tree.pl default`
- p. 1106 - Set environment variables `ACE_ROOT` as the current directory e.g. `ACE_ROOT=/usr/local/ACE_wrappers/build/default; export ACE_ROOT`
- p. 1106 - Set `TAO_ROOT` with ` TAO_ROOT=$ACE_ROOT/TAO; export TAO_ROOT`
- p. 1007 - Set `LD_LIBRARY_PATH` with `LD_LIBRARY_PATH=$ACE_ROOT/lib:$LD_LIBRARY_PATH; export LD_LIBRARY_PATH`
- Recommend to make a `.env` file of the above so that you can `source .env` with every new terminal. Test binaries need updated `LD_LIBRARY_PATH` to run.
- p. 66 - Create text file `$ACE_ROOT/ace/config.h` with `#include "config-linux.h"` 
- p. 66 - Create text file `$ACE_ROOT/include/makeinclude/platform_macros.GNU` with `$(ACE_ROOT)/include/makeinclude/platform_linux.GNU`
- p. 1109 - Generate Makefiles - `cd $ACE_ROOT; bin/mwc.pl -type gnuace -recurse`
- p. 1110 - Make and build files `cd $TAO_ROOT; make`

The build will take some time as this builds the full version. 

