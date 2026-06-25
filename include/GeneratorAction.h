#ifndef GeneratorAction_h
#define GeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "G4Event.hh"
#include "HepMC/IO_GenEvent.h"
#include "globals.hh"

class GeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    GeneratorAction();
    ~GeneratorAction() override;

    void GeneratePrimaries( G4Event* ) override;

  private:
  // Pair-production sources, reads data/1000.ee.dat and data/1000.mumu.dat
    HepMC::IO_GenEvent* m_eeInput;
    HepMC::IO_GenEvent* m_mumuInput;

    void GenerateFromHepMC( G4Event* event, HepMC::IO_GenEvent*& reader, const char* path);

    // Singe-particle source for species taht aren't naturally pair-produced
    G4ParticleGun* m_gun;
    void GenerateFromGun(G4Event* anEvent, const G4String& particleName,
                         G4int particleCode, G4int charge);
    
    // Shared truth-ntuple writer used by both source types, so the Truth CSV has one consistent schema
    void WriteTruth(G4int eventID, G4double phi, G4double theta, G4double momentumGeV, G4int particleCode, G4int charge);
};

#endif