# Note
Future work includes using the ACE+TAO library as a submodule. However, at this point in time, it needs to be installed separately.     
Please see the instructions below for the Ubuntu platform. This should also work on other Linux/Unix (Mac OS) systems.    
Windows systems should be able to install based on changing the OS-specific parameters (refer to the developer's guide PDF).    

# Install ACE+TAO on Ubuntu
1. Download the latest minor relase `Full` from https://download.dre.vanderbilt.edu/
1. Unzip the downloaded file into the `extern` directory.
1. Create a build tree in `extern/ACE_wrappers` by running `MPC/clone_build_tree.pl default`
1. Set environment variables `ACE_ROOT` as the current directory e.g. `ACE_ROOT=/usr/local/ACE_wrappers/build/default; export ACE_ROOT`
1. Set `TAO_ROOT` with ` TAO_ROOT=$ACE_ROOT/TAO; export TAO_ROOT`
1. Set `LD_LIBRARY_PATH` with `LD_LIBRARY_PATH=$ACE_ROOT/lib:$LD_LIBRARY_PATH; export LD_LIBRARY_PATH`
    - *Create a `.env` file of the above and run `source .env` with every new terminal. Test binaries need updated `LD_LIBRARY_PATH` to run.*
1. Create text file `$ACE_ROOT/ace/config.h` with `#include "config-linux.h"` 
1. Create text file `$ACE_ROOT/include/makeinclude/platform_macros.GNU` with `$(ACE_ROOT)/include/makeinclude/platform_linux.GNU`
1. Generate Makefiles - `cd $ACE_ROOT; bin/mwc.pl -type gnuace -recurse`
1. Make and build files `cd $TAO_ROOT; make`

The build will take some time as this builds the full version. 

**TAO Developer's Guide:** https://objectcomputing.com/files/3415/3090/8836/TAO_Dev_Guide_2_2a_p7.pdf    
Instructions taken from Chapter 2 and Appendix C, specifically:
- p. 1106-7 -- Create a build tree, set environment variables
- p. 66 -- Content for `config.h` and `platform_macros.GNU`
- p. 1109-10 -- Generate Makefiles and build instruction

