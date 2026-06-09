# Install script for directory: /Users/thomasgriffiths/Documents/Projects/Physics/UE-Phys-DA&ML/S2/GeantExample3/HEPmc/HepMC-2.06.11/HepMC

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/Users/thomasgriffiths/Documents/Projects/Physics/UE-Phys-DA&ML/S2/GeantExample3/HEPmc")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "RelWithDebInfo")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set path to fallback-tool for dependency-resolution.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/HepMC" TYPE FILE FILES
    "/Users/thomasgriffiths/Documents/Projects/Physics/UE-Phys-DA&ML/S2/GeantExample3/HEPmc/HepMC-2.06.11/HepMC/CompareGenEvent.h"
    "/Users/thomasgriffiths/Documents/Projects/Physics/UE-Phys-DA&ML/S2/GeantExample3/HEPmc/HepMC-2.06.11/HepMC/Flow.h"
    "/Users/thomasgriffiths/Documents/Projects/Physics/UE-Phys-DA&ML/S2/GeantExample3/HEPmc/HepMC-2.06.11/HepMC/GenEvent.h"
    "/Users/thomasgriffiths/Documents/Projects/Physics/UE-Phys-DA&ML/S2/GeantExample3/HEPmc/HepMC-2.06.11/HepMC/GenParticle.h"
    "/Users/thomasgriffiths/Documents/Projects/Physics/UE-Phys-DA&ML/S2/GeantExample3/HEPmc/HepMC-2.06.11/HepMC/GenVertex.h"
    "/Users/thomasgriffiths/Documents/Projects/Physics/UE-Phys-DA&ML/S2/GeantExample3/HEPmc/HepMC-2.06.11/HepMC/GenCrossSection.h"
    "/Users/thomasgriffiths/Documents/Projects/Physics/UE-Phys-DA&ML/S2/GeantExample3/HEPmc/HepMC-2.06.11/HepMC/GenRanges.h"
    "/Users/thomasgriffiths/Documents/Projects/Physics/UE-Phys-DA&ML/S2/GeantExample3/HEPmc/HepMC-2.06.11/HepMC/HeavyIon.h"
    "/Users/thomasgriffiths/Documents/Projects/Physics/UE-Phys-DA&ML/S2/GeantExample3/HEPmc/HepMC-2.06.11/HepMC/HEPEVT_Wrapper.h"
    "/Users/thomasgriffiths/Documents/Projects/Physics/UE-Phys-DA&ML/S2/GeantExample3/HEPmc/HepMC-2.06.11/HepMC/HerwigWrapper.h"
    "/Users/thomasgriffiths/Documents/Projects/Physics/UE-Phys-DA&ML/S2/GeantExample3/HEPmc/HepMC-2.06.11/HepMC/IO_AsciiParticles.h"
    "/Users/thomasgriffiths/Documents/Projects/Physics/UE-Phys-DA&ML/S2/GeantExample3/HEPmc/HepMC-2.06.11/HepMC/IO_BaseClass.h"
    "/Users/thomasgriffiths/Documents/Projects/Physics/UE-Phys-DA&ML/S2/GeantExample3/HEPmc/HepMC-2.06.11/HepMC/IO_Exception.h"
    "/Users/thomasgriffiths/Documents/Projects/Physics/UE-Phys-DA&ML/S2/GeantExample3/HEPmc/HepMC-2.06.11/HepMC/IO_GenEvent.h"
    "/Users/thomasgriffiths/Documents/Projects/Physics/UE-Phys-DA&ML/S2/GeantExample3/HEPmc/HepMC-2.06.11/HepMC/IO_HEPEVT.h"
    "/Users/thomasgriffiths/Documents/Projects/Physics/UE-Phys-DA&ML/S2/GeantExample3/HEPmc/HepMC-2.06.11/HepMC/IO_HERWIG.h"
    "/Users/thomasgriffiths/Documents/Projects/Physics/UE-Phys-DA&ML/S2/GeantExample3/HEPmc/HepMC-2.06.11/HepMC/IteratorRange.h"
    "/Users/thomasgriffiths/Documents/Projects/Physics/UE-Phys-DA&ML/S2/GeantExample3/HEPmc/HepMC-2.06.11/HepMC/PdfInfo.h"
    "/Users/thomasgriffiths/Documents/Projects/Physics/UE-Phys-DA&ML/S2/GeantExample3/HEPmc/HepMC-2.06.11/HepMC/Polarization.h"
    "/Users/thomasgriffiths/Documents/Projects/Physics/UE-Phys-DA&ML/S2/GeantExample3/HEPmc/HepMC-2.06.11/HepMC/PythiaWrapper6_4.h"
    "/Users/thomasgriffiths/Documents/Projects/Physics/UE-Phys-DA&ML/S2/GeantExample3/HEPmc/HepMC-2.06.11/HepMC/PythiaWrapper6_4_WIN32.h"
    "/Users/thomasgriffiths/Documents/Projects/Physics/UE-Phys-DA&ML/S2/GeantExample3/HEPmc/HepMC-2.06.11/HepMC/PythiaWrapper.h"
    "/Users/thomasgriffiths/Documents/Projects/Physics/UE-Phys-DA&ML/S2/GeantExample3/HEPmc/HepMC-2.06.11/HepMC/WeightContainer.h"
    "/Users/thomasgriffiths/Documents/Projects/Physics/UE-Phys-DA&ML/S2/GeantExample3/HEPmc/HepMC-2.06.11/HepMC/SearchVector.h"
    "/Users/thomasgriffiths/Documents/Projects/Physics/UE-Phys-DA&ML/S2/GeantExample3/HEPmc/HepMC-2.06.11/HepMC/SimpleVector.h"
    "/Users/thomasgriffiths/Documents/Projects/Physics/UE-Phys-DA&ML/S2/GeantExample3/HEPmc/HepMC-2.06.11/HepMC/SimpleVector.icc"
    "/Users/thomasgriffiths/Documents/Projects/Physics/UE-Phys-DA&ML/S2/GeantExample3/HEPmc/HepMC-2.06.11/HepMC/StreamHelpers.h"
    "/Users/thomasgriffiths/Documents/Projects/Physics/UE-Phys-DA&ML/S2/GeantExample3/HEPmc/HepMC-2.06.11/HepMC/StreamInfo.h"
    "/Users/thomasgriffiths/Documents/Projects/Physics/UE-Phys-DA&ML/S2/GeantExample3/HEPmc/HepMC-2.06.11/HepMC/enable_if.h"
    "/Users/thomasgriffiths/Documents/Projects/Physics/UE-Phys-DA&ML/S2/GeantExample3/HEPmc/HepMC-2.06.11/HepMC/is_arithmetic.h"
    "/Users/thomasgriffiths/Documents/Projects/Physics/UE-Phys-DA&ML/S2/GeantExample3/HEPmc/HepMC-2.06.11/HepMC/TempParticleMap.h"
    "/Users/thomasgriffiths/Documents/Projects/Physics/UE-Phys-DA&ML/S2/GeantExample3/HEPmc/HepMC-2.06.11/HepMC/Units.h"
    "/Users/thomasgriffiths/Documents/Projects/Physics/UE-Phys-DA&ML/S2/GeantExample3/HEPmc/HepMC-2.06.11/HepMC/Version.h"
    "/Users/thomasgriffiths/Documents/Projects/Physics/UE-Phys-DA&ML/S2/GeantExample3/HEPmc/HepMC-2.06.11/HepMC/HepMCDefs.h"
    )
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "/Users/thomasgriffiths/Documents/Projects/Physics/UE-Phys-DA&ML/S2/GeantExample3/HEPmc/HepMC-2.06.11/build/HepMC/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
