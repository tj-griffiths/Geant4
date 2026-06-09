# Install script for directory: /Users/thomasgriffiths/Documents/Projects/Physics/UE-Phys-DA&ML/S2/GeantExample3/HEPmc/HepMC-2.06.11/examples/fio

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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/HepMC/examples/fio" TYPE FILE FILES
    "/Users/thomasgriffiths/Documents/Projects/Physics/UE-Phys-DA&ML/S2/GeantExample3/HEPmc/HepMC-2.06.11/examples/fio/example_MyHerwig.cc"
    "/Users/thomasgriffiths/Documents/Projects/Physics/UE-Phys-DA&ML/S2/GeantExample3/HEPmc/HepMC-2.06.11/examples/fio/example_MyPythia.cc"
    "/Users/thomasgriffiths/Documents/Projects/Physics/UE-Phys-DA&ML/S2/GeantExample3/HEPmc/HepMC-2.06.11/examples/fio/example_MyPythiaOnlyToHepMC.cc"
    "/Users/thomasgriffiths/Documents/Projects/Physics/UE-Phys-DA&ML/S2/GeantExample3/HEPmc/HepMC-2.06.11/examples/fio/example_PythiaStreamIO.cc"
    "/Users/thomasgriffiths/Documents/Projects/Physics/UE-Phys-DA&ML/S2/GeantExample3/HEPmc/HepMC-2.06.11/examples/fio/initpydata.f"
    "/Users/thomasgriffiths/Documents/Projects/Physics/UE-Phys-DA&ML/S2/GeantExample3/HEPmc/HepMC-2.06.11/examples/fio/initPythia.cc"
    "/Users/thomasgriffiths/Documents/Projects/Physics/UE-Phys-DA&ML/S2/GeantExample3/HEPmc/HepMC-2.06.11/examples/fio/PythiaHelper.h"
    "/Users/thomasgriffiths/Documents/Projects/Physics/UE-Phys-DA&ML/S2/GeantExample3/HEPmc/HepMC-2.06.11/examples/fio/testHerwigCopies.cc"
    "/Users/thomasgriffiths/Documents/Projects/Physics/UE-Phys-DA&ML/S2/GeantExample3/HEPmc/HepMC-2.06.11/examples/fio/testPythiaCopies.cc"
    "/Users/thomasgriffiths/Documents/Projects/Physics/UE-Phys-DA&ML/S2/GeantExample3/HEPmc/HepMC-2.06.11/build/examples/fio/GNUmakefile"
    )
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "/Users/thomasgriffiths/Documents/Projects/Physics/UE-Phys-DA&ML/S2/GeantExample3/HEPmc/HepMC-2.06.11/build/examples/fio/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
