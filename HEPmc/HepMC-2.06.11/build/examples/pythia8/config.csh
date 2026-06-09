#!/bin/csh
if( ! $?LD_LIBRARY_PATH ) then
  setenv LD_LIBRARY_PATH /Users/thomasgriffiths/Documents/Projects/Physics/UE-Phys-DA&ML/S2/GeantExample3/HEPmc/lib
else
  setenv LD_LIBRARY_PATH ${LD_LIBRARY_PATH}:/Users/thomasgriffiths/Documents/Projects/Physics/UE-Phys-DA&ML/S2/GeantExample3/HEPmc/lib
endif
setenv PYTHIA8DATA ${PYTHIA8_HOME}/xmldoc
