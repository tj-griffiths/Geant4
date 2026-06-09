#!/bin/bash

# Local Mac setup - point to your Geant4 installation
source /Users/thomasgriffiths/geant4-install/bin/geant4.sh

# Check if HepMC already exists
if [ -d "HEPmc/HepMC-2.06.11" ]; then
    echo "HepMC already installed"
else
    echo "Installing HepMC... this will take a while"
    
    # Create directory
    mkdir -p HEPmc
    cd HEPmc
    
    # Download using curl (macOS default) instead of wget
    curl -O http://hepmc.web.cern.ch/hepmc/releases/hepmc2.06.11.tgz
    
    # Extract
    tar zxf hepmc2.06.11.tgz
    cd HepMC-2.06.11
    
    # Configure and build
    ./configure --prefix=$PWD/../../HEPmc/HepMC-2.06.11
    make
    make check
    make install
    
    cd ../..
fi

# Set HepMC environment
export HEPMC_DIR=$PWD/HEPmc/HepMC-2.06.11
