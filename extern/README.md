# External Modules
This project has three external dependencies. GLFW and GLM are included through git submodules and should be installed with the git init recursive command. The third library, TAO+ACE, needs to be installed manually. 

Future work includes using the ACE+TAO library as a submodule. However, at this point in time, it needs to be installed separately.     
Please see the instructions below for the Ubuntu platform. This should also work on other Linux/Unix (Mac OS) systems.    
Windows systems should be able to install based on changing the OS-specific parameters (refer to the developer's guide PDF).    

# Install ACE+TAO on Ubuntu
1. Download the latest micro relase `Full` from https://download.dre.vanderbilt.edu/.
    - *As of this writing, the latest micro release is ACE 7.0.4 and TAO 3.0.4 (ACE+TAO x.0.4).*
1. Unzip the downloaded file into the `extern` directory.
1. Create a build tree in `extern/ACE_wrappers` by running `MPC/clone_build_tree.pl default`
1. Set environment variables `ACE_ROOT` as the current directory e.g. `ACE_ROOT=/home/user/svrcubeworld/extern/ACE_wrappers/build/default; export ACE_ROOT`
1. Set `TAO_ROOT` with ` TAO_ROOT=$ACE_ROOT/TAO; export TAO_ROOT`
1. Set `LD_LIBRARY_PATH` with `LD_LIBRARY_PATH=$ACE_ROOT/lib:$LD_LIBRARY_PATH; export LD_LIBRARY_PATH`
1. Create text file `$ACE_ROOT/ace/config.h` with `#include "config-linux.h"` 
1. Create text file `$ACE_ROOT/include/makeinclude/platform_macros.GNU` with `$(ACE_ROOT)/include/makeinclude/platform_linux.GNU`
1. Generate Makefiles with `cd $ACE_ROOT; bin/mwc.pl -type gnuace -recurse`
1. Make and build files with `cd $TAO_ROOT; make`

The build will take some time as this builds the full version. 

**TAO Developer's Guide:** https://objectcomputing.com/files/3415/3090/8836/TAO_Dev_Guide_2_2a_p7.pdf    
Instructions taken from Chapter 2 and Appendix C, specifically:
- p. 1106-7 -- Create a build tree, set environment variables
- p. 66 -- Content for `config.h` and `platform_macros.GNU`
- p. 1109-10 -- Generate Makefiles and build instruction

