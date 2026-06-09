#include "MuonSD.h"

#include "G4Step.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4SystemOfUnits.hh"
#include "G4CsvAnalysisManager.hh"

using G4AnalysisManager = G4CsvAnalysisManager; // Use the CSV analysis manager

// Constructor (passes name and ID of the ntuple to save to)
MuonSD::MuonSD( const G4String& name, G4int ntupleID ) 
  : G4VSensitiveDetector( name ), m_ntupleID( ntupleID )
{
}

// Destructor
MuonSD::~MuonSD()
{
}


void MuonSD::Initialize( G4HCofThisEvent* )
{
  m_recordedTracks.clear(); // reset each event
}

// This method is called every time a particle takes a step in the sensitive volume
G4bool MuonSD::ProcessHits( G4Step* step, G4TouchableHistory* )
{
    // Only accept muons
    G4String particleName = step->GetTrack()->GetParticleDefinition()->GetParticleName();
    if (particleName != "mu+" && particleName != "mu-") return false;

    /// Get the track for the event
    G4int eventID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
    G4int trackID = step->GetTrack()->GetTrackID();

    // Only record each muon track once per event
    if (m_recordedTracks.count(trackID)) return false;
    m_recordedTracks.insert(trackID);
    G4ThreeVector pos = step->GetTrack()->GetPosition();

    // Add it to the list of phi and theta for this event
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->FillNtupleIColumn( m_ntupleID, 0, eventID );
    analysisManager->FillNtupleDColumn( m_ntupleID, 1, pos.phi() );
    analysisManager->FillNtupleDColumn( m_ntupleID, 2, pos.theta() );
    analysisManager->AddNtupleRow( m_ntupleID );

    return true;
}

void MuonSD::EndOfEvent( G4HCofThisEvent* )
{
}

