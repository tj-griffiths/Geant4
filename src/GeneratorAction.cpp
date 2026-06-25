#include "GeneratorAction.h"

#include "G4RunManager.hh"
#include "G4LorentzVector.hh"
#include "G4PrimaryParticle.hh"
#include "G4PrimaryVertex.hh"
#include "G4TransportationManager.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "Randomize.hh"
#include "G4CsvAnalysisManager.hh"
using G4AnalysisManager = G4CsvAnalysisManager;

#include <cmath>

// Simplified version of Geant4 example
// extended/eventgenerator/HepMC/HepMCEx01/src/HepMCG4Interface.cc
// My only addition is the truth information output at the end

// Source Mixing configuration
// Weights/paths/energy range are all in one place to retune
namespace {
  const char* kEEPath = "data/1000.ee.dat";
  const char* kMumuPath = "data/1000.mumu.dat";

  // Mixing Weights
  // {ee file, mumu file, photon gun, proton/antiproton gun, pion gun}
  // Must sum to ~ 1.0:
  const G4double kWeights[5] = {0.25, 0.25, 0.15, 0.15, 0.20};

  // Gun-fired particles sampled over this energy range (GeV)
  const G4double kGunEnergyMin = 0.1 * GeV;
  const G4double kGunEnergyMax = 20.0 * GeV;
}

GeneratorAction::GeneratorAction()
  : m_eeInput(nullptr), m_mumuInput(nullptr), m_gun(nullptr)
{
  // Load input file 
  m_eeInput = new HepMC::IO_GenEvent(kEEPath, std::ios::in);
  m_mumuInput = new HepMC::IO_GenEvent(kMumuPath, std::ios::in);

  // Particle/energy/direction are overwritten per-event from GenerateFromGun()
  m_gun = new G4ParticleGun(1); // 1 particle per event
}

GeneratorAction::~GeneratorAction()
{
  delete m_eeInput;
  delete m_mumuInput;
  delete m_gun;

}

// GeneratePrimaries() picks from one of five weighted sources
void GeneratorAction::GeneratePrimaries( G4Event* anEvent )
{
  G4double roll = G4UniformRand();
  G4int source = 4; //falls through to pion gun if all else fails
  G4double cumulative = 0.0;
  for (G4int i = 0; i < 5; ++i)
  {
    cumulative += kWeights[i];
    if (roll < cumulative) {source = i; break;}
  }

  switch (source)
  {
    case 0:
      GenerateFromHepMC(anEvent, m_eeInput, kEEPath);
      break;
    case 1:
      GenerateFromHepMC(anEvent, m_mumuInput, kMumuPath);
      break;
    case 2:
      GenerateFromGun(anEvent, "gamma", /*particleCode=*/2, /*charge=*/0);
      break;
    case 3:
      if (G4UniformRand() < 0.5)
        GenerateFromGun(anEvent, "proton", 3, +1);
      else
        GenerateFromGun(anEvent, "anti_proton", 3, -1);
      break;
    case 4:
      if (G4UniformRand() < 0.5)
        GenerateFromGun(anEvent, "pi+", 4, +1);
      else
        GenerateFromGun(anEvent, "pi-", 4, -1);
      break;
  }
}

void GeneratorAction::GenerateFromHepMC(G4Event* anEvent, HepMC::IO_GenEvent*& reader, const char* path)
{
  HepMC::GenEvent* hepmcEvent = reader->read_next_event();

  if (!hepmcEvent)
  {
    delete reader;
    reader = new HepMC::IO_GenEvent(path, std::ios::in);
    hepmcEvent = reader->read_next_event();
    if (!hepmcEvent)
    {
      // Reopen genuienly failed (missing/corrupt file) -- real error and not just "run out of events"
      G4cerr << "GeneratorAction: failed to reopen HepMC file " << path << ". Run terminated..." << G4endl;
      G4RunManager::GetRunManager()->AbortRun();
      return;
    }
  }

  // Loop over all event vertices
  for (auto vitr = hepmcEvent->vertices_begin(); vitr != hepmcEvent->vertices_end(); ++vitr)
  {
    G4bool valid = false;
    for (auto pitr = (*vitr)->particles_begin(HepMC::children); pitr != (*vitr)->particles_end(HepMC::children); ++pitr)
    {
      if (!(*pitr)->end_vertex() && (*pitr)->status() == 1) {valid = true; break;}
    }
    if (!valid) continue; // skip to next vertex in loop

    HepMC::FourVector pos = (*vitr)->position();
    G4LorentzVector xvtx(pos.x(), pos.y(), pos.z(), pos.t()); 

    G4Navigator* navigator = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();
    G4VPhysicalVolume* world = navigator->GetWorldVolume();
    G4VSolid* solid = world->GetLogicalVolume()->GetSolid();
    EInside qinside = solid->Inside(xvtx.vect() * mm);
    if (qinside != kInside) continue; // skip to next vertex in loop

    G4PrimaryVertex* g4vtx = new G4PrimaryVertex(xvtx.x() * mm, xvtx.y() * mm, xvtx.z() * mm, xvtx.t() * mm / c_light);

    for (auto vpitr = (*vitr)->particles_begin(HepMC::children); vpitr != (*vitr)->particles_end(HepMC::children); ++vpitr)
    {
      if ((*vpitr)->status() !=1) continue; // skip to next particle in loop
      G4int pdgcode = (*vpitr)->pdg_id();
      HepMC::FourVector mom = (*vpitr)->momentum();
      G4PrimaryParticle* g4prim = new G4PrimaryParticle(pdgcode, mom.px() * MeV, mom.py() * MeV, mom.pz() * MeV);

      // Output truth information for muons ( pdg 13 ), electrons ( pdg 11 ), photon (22), charged pion (211), proton (2212), and neutron (2112)
      if (abs(pdgcode) == 13 || abs(pdgcode) == 11 || pdgcode == 22 || abs(pdgcode) == 2212 || abs(pdgcode) == 211 || pdgcode == 2112)
      {
        int particleCode = 99; // 0 = electron, 1 = muon, 2 = photon, 3 = proton, 4 = pion, 5 = neutron, 99 = other
        int charge = 0;
        if (abs(pdgcode) == 11) {particleCode = 0; charge = (pdgcode > 0)? -1 : +1;} // electron/positron
        if (abs(pdgcode) == 13) {particleCode = 1; charge = (pdgcode > 0)? -1 : +1;} // muon/antimuon
        if (pdgcode == 22) {particleCode = 2; charge = 0;} // photon
        if (abs(pdgcode) == 2212) {particleCode = 3; charge = (pdgcode > 0)? +1 : -1;} // proton/antiproton
        if (abs(pdgcode) == 211) {particleCode = 4; charge = (pdgcode > 0)? +1 : -1;} // charged pion
        if (pdgcode == 2112) {particleCode = 5; charge = 0;} // neutron

        WriteTruth(anEvent->GetEventID(), g4prim->GetMomentum().phi(), g4prim->GetMomentum().theta(), g4prim->GetMomentum().mag() / GeV, particleCode, charge);
      }
      g4vtx->SetPrimary(g4prim);
    }
   anEvent->AddPrimaryVertex(g4vtx);
  }
}

// Gun-based source for species without a HepMC pair sample.
// Energy is log-uniform
// Isotropic, fired from the world origin.
void GeneratorAction::GenerateFromGun(G4Event* anEvent, const G4String& particleName, G4int particleCode, G4int charge)

{
  G4ParticleDefinition* particle = G4ParticleTable::GetParticleTable()->FindParticle(particleName);
  if (!particle)
  {
    G4cerr << "GeneratorAction: Unkown Particle Name '" << particleName << "'. Run terminated..." << G4endl;
    return;
  }

  G4double logEMin = std::log(kGunEnergyMin);
  G4double logEMax = std::log(kGunEnergyMax);
  G4double energy = std::exp(logEMin + G4UniformRand() * (logEMax - logEMin));

  G4double cosTheta = 2.0 * G4UniformRand() - 1.0;
  G4double theta = std::acos(cosTheta);
  G4double phi = 2.0 * CLHEP::pi * G4UniformRand();
  G4double sinTheta = std::sin(theta);
  G4ThreeVector direction(sinTheta * std::cos(phi), sinTheta * std::sin(phi), cosTheta);

  m_gun->SetParticleDefinition(particle);
  m_gun->SetParticleEnergy(energy);
  m_gun->SetParticleMomentumDirection(direction);
  m_gun->SetParticlePosition(G4ThreeVector(0.0, 0.0, 0.0));
  m_gun->GeneratePrimaryVertex(anEvent); // builds vertex + primary, adds to event

  WriteTruth(anEvent->GetEventID(), phi, theta, energy / GeV, particleCode, charge);
}

void GeneratorAction::WriteTruth(G4int eventID, G4double phi, G4double theta, G4double momentumGeV, G4int particleCode, G4int charge)

{
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->FillNtupleIColumn(0, 0, eventID);
  analysisManager->FillNtupleDColumn(0, 1, phi);
  analysisManager->FillNtupleDColumn(0, 2, theta);
  analysisManager->FillNtupleDColumn(0, 3, momentumGeV);
  analysisManager->FillNtupleIColumn(0, 4, particleCode);
  analysisManager->FillNtupleIColumn(0, 5, charge);
  analysisManager->AddNtupleRow(0);
}