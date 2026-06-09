#include "PositionFinder.h"

#include "G4RunManager.hh"
#include "G4CsvAnalysisManager.hh"
using G4AnalysisManager = G4CsvAnalysisManager;

PositionFinder::PositionFinder( const G4String& name, const G4int id )
  : G4VSensitiveDetector( name ) // Run the constructor of the parent class
{
  // Set which ntuple to use
  m_ID = id;
}

PositionFinder::~PositionFinder()
{
}

void PositionFinder::Initialize( G4HCofThisEvent* )
{
  m_recordedTracks.clear(); // clears the set of recorded tracks from previous event
}

// Analyze anything that hit the detector
G4bool PositionFinder::ProcessHits( G4Step* step, G4TouchableHistory* )
{
  // Tracking detectors only sense charged particles
  if ( step->GetTrack()->GetParticleDefinition()->GetPDGCharge() != 0.0 )
  {
    G4int trackID = step->GetTrack()->GetTrackID();
    if (m_recordedTracks.count(trackID)) return true;
    m_recordedTracks.insert(trackID);

    // Get the analysis manager
    auto analysisManager = G4AnalysisManager::Instance();

    // Fill ntuple with my ID number
    analysisManager->FillNtupleIColumn( m_ID, 0, G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID() ); // Column 0 - event number
    analysisManager->FillNtupleDColumn( m_ID, 1, step->GetTrack()->GetPosition().phi() );                          // Column 1 - phi coordinate of hit
    analysisManager->FillNtupleDColumn( m_ID, 2, step->GetTrack()->GetPosition().theta() );                        // Column 2 - theta coordinate of hit
    analysisManager->AddNtupleRow( m_ID ); // Row complete
  }

  return true;
}

void PositionFinder::EndOfEvent( G4HCofThisEvent* )
{
}
