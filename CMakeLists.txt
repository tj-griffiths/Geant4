#----------------------------------------------------------------------------
# Setup the project
#
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(Project3)

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
#
include(${Geant4_USE_FILE})

#----------------------------------------------------------------------------
# Find HepMC (required package)
#
find_package(HepMC REQUIRED)

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
include_directories(${PROJECT_SOURCE_DIR}/include 
                    ${Geant4_INCLUDE_DIR}
                    ${HEPMC_INCLUDE_DIR})
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.h)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 and HEPMC libraries
#
add_executable(MyProgram ${sources} ${headers})
target_link_libraries(MyProgram ${Geant4_LIBRARIES} ${HEPMC_LIBRARIES})

# C++17 complaining about std::iterator being deprecated
# Just hide it to avoid the noise (need to move to HEPMC3 eventually)
target_compile_options(MyProgram PRIVATE -Wno-deprecated-declarations)
