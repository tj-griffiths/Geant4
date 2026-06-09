#include "GeneratorAction.h"

#include "G4RunManager.hh"
#include "G4LorentzVector.hh"
#include "G4PrimaryParticle.hh"
#include "G4PrimaryVertex.hh"
#include "G4TransportationManager.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4CsvAnalysisManager.hh"
using G4AnalysisManager = G4CsvAnalysisManager;

// Simplified version of Geant4 example
// extended/eventgenerator/HepMC/HepMCEx01/src/HepMCG4Interface.cc
// My only addition is the truth information output at the end
GeneratorAction::GeneratorAction()
{
  // Load my input file 
  m_asciiInput = new HepMC::IO_GenEvent( "data/1000.mumu.dat", std::ios::in ); // Use this for muon pair production
  //m_asciiInput = new HepMC::IO_GenEvent( "data/1000.ee.dat", std::ios::in ); // Use this for electron pair production
}

GeneratorAction::~GeneratorAction()
{
  delete m_asciiInput;
}

void GeneratorAction::GeneratePrimaries( G4Event* anEvent )
{
  // Load next event from file
  HepMC::GenEvent* hepmcEvent = m_asciiInput->read_next_event();
  if ( !hepmcEvent )
  {
    G4cout << "HepMCInterface: no generated particles. Run terminated..." << G4endl; 
    G4RunManager::GetRunManager()->AbortRun();
    return;
  }

  // Loop over all event vertices
  for( auto vitr = hepmcEvent->vertices_begin(); vitr != hepmcEvent->vertices_end(); ++vitr )
  {
    // Check that the vertex has valid particles
    G4bool valid = false;
    for ( auto pitr= (*vitr)->particles_begin( HepMC::children ); pitr != (*vitr)->particles_end( HepMC::children ); ++pitr)
    {
      if ( !(*pitr)->end_vertex() && (*pitr)->status() == 1 )
      {
        valid = true;
        break;
      }
    }
    if ( !valid ) continue; // skip to next vertex in loop

    // Find vertex position 
    HepMC::FourVector pos = (*vitr)->position();
    G4LorentzVector xvtx( pos.x(), pos.y(), pos.z(), pos.t() );

    // Check the vertex is inside the world
    G4Navigator* navigator = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();
    G4VPhysicalVolume* world = navigator->GetWorldVolume();
    G4VSolid* solid = world->GetLogicalVolume()->GetSolid();
    EInside qinside = solid->Inside( xvtx.vect()*mm );
    if ( qinside != kInside ) continue; // skip to next vertex in loop

    // Make the vertex (input lengths in mm)
    G4PrimaryVertex* g4vtx = new G4PrimaryVertex( xvtx.x()*mm,
                                                  xvtx.y()*mm,
                                                  xvtx.z()*mm,
                                                  xvtx.t()*mm / c_light );

    // Loop over all particles for this vertex
    for ( auto vpitr = (*vitr)->particles_begin( HepMC::children ); vpitr != (*vitr)->particles_end( HepMC::children ); ++vpitr )
    {
      if ( (*vpitr)->status() != 1 ) continue; // skip to next particle in loop

      // Make the particle (input energies in MeV)
      G4int pdgcode = (*vpitr)->pdg_id();
      HepMC::FourVector mom = (*vpitr)->momentum();
      G4PrimaryParticle* g4prim = new G4PrimaryParticle( pdgcode, mom.px()*MeV, mom.py()*MeV, mom.pz()*MeV );

      // Output truth information for muons ( pdg 13 ), electrons ( pdg 11 ), photon (22), charged pion (211),  neutron ( 2112), and proton (2212)
      // The abs(pdgcode) is for both particles and antiparticles, since they have the same momentum and only differ in charge
      if (abs(pdgcode) == 13 || abs(pdgcode) == 11  ||  pdgcode == 22 ||  pdgcode == 2212 || pdgcode == 2112)
      
      {
        // Get analysis manager
        auto analysisManager = G4AnalysisManager::Instance();

        // Assign particle codes as integer:
        // 0 = electron, 1 = muon, 2 = photon, 3 = proton, 4 = neutron, and 99 = other
        int particleCode = 99;
        if (abs(pdgcode) == 11) particleCode = 0;
        if (abs(pdgcode) == 13) particleCode = 1;
        if (pdgcode == 22) particleCode = 2;
        if (pdgcode == 2212) particleCode = 3;
        if (pdgcode == 2112) particleCode = 4;

        // Fill ntuple
        analysisManager->FillNtupleIColumn( 0, 0, anEvent->GetEventID() );
        analysisManager->FillNtupleDColumn( 0, 1, g4prim->GetMomentum().phi() );
        analysisManager->FillNtupleDColumn( 0, 2, g4prim->GetMomentum().theta() );
        // Divide by the unit when outputting
        // see http://geant4.web.cern.ch/sites/geant4.web.cern.ch/files/geant4/collaboration/working_groups/electromagnetic/gallery/units/SystemOfUnits.html
        analysisManager->FillNtupleDColumn( 0, 3, g4prim->GetMomentum().mag()/GeV );
        analysisManager->FillNtupleIColumn( 0, 4, particleCode );
        analysisManager->AddNtupleRow( 0 );
      }

      // Add my particle to the vertex
      g4vtx->SetPrimary( g4prim );
    }

    // Add my vertex to the event
    anEvent->AddPrimaryVertex( g4vtx );
  }
}
