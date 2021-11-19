# Oculus VR Mobile SDK - Cloud VR Server

This project implements a Cloud VR pipeline using the Oculus Mobile SDK sample project, "CubeWorld" and TAO, a CORBA service for distributed communication based on the ACE C++ framework.

Build instructions below.

# Original Copyright #
Created		:	March, 2015    
Authors		:	J.M.P. van Waveren    
Copyright	:	Copyright (c) Facebook Technologies, LLC and its affiliates. All rights reserved.    

# How To Build and Run the OVR Server
1. From a command line, run `./configure.sh`
1. Wait for CMake to complete
1. Run `./build.sh`
1. Wait for build to complete
1. Run `./run.sh`
OR
Run `./full.sh` to run through all steps and tests

# How to Build and Run the OVR Client
This is the client that runs on the Oculus Quest.
1. Download the Oculus Mobile SDK https://developer.oculus.com/downloads/package/oculus-mobile-sdk/
1. Replace the VrSamples directory with the one located in `extern/ovr_client/VrSamples`
1. Run `build.sh` in the `VrCubeWorld_Framework/Project/Android` folder
1. Project will build and install if an Oculus Quest is connected.

The OVR application client code is located in the folder `extern/ovr_client/VrSamples/VrCubeWorld_Framework/Src/`

