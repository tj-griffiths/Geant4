#apt install -y wget
wget http://hepmc.web.cern.ch/hepmc/releases/hepmc2.06.11.tgz
tar zxvf hepmc2.06.11.tgz
mkdir build install
cd build
../HepMC-2.06.11/configure --prefix="$PWD/../install" --with-momentum=MEV --with-length=MM
make
make check
make install
cd ..
