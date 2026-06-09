#ifndef GeneratorAction_h
#define GeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4Event.hh"
#include "HepMC/IO_GenEvent.h"

class GeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    GeneratorAction();
    ~GeneratorAction() override;

    void GeneratePrimaries( G4Event* ) override;

  private:
    HepMC::IO_GenEvent* m_asciiInput;
};

#endif