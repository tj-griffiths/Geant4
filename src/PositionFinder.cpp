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
  m_trackHits.clear(); // clears the set of recorded tracks from previous event
}

// Analyze anything that hit the detector
// Used claude to help set up recording energy and path length accumulation across each step in the volume
G4bool PositionFinder::ProcessHits( G4Step* step, G4TouchableHistory* )
{
  // Tracking detectors only sense charged particles
  if ( step->GetTrack()->GetParticleDefinition()->GetPDGCharge() != 0.0 )
  {
    // Get the track ID, energy deposited, and step length for this step
    G4int trackID = step->GetTrack()->GetTrackID();
    G4double stepEdep = step->GetTotalEnergyDeposit();
    G4double stepLength = step->GetStepLength();

    // Check if this track has already been recorded in the map
    auto it = m_trackHits.find(trackID);
    if (it == m_trackHits.end() )
    {
      // If not, create a new record for this track and add it to the map
      TrackerHitRecord rec;
      rec.phi = step->GetTrack()->GetPosition().phi();
      rec.theta = step->GetTrack()->GetPosition().theta();
      rec.energyDeposit = stepEdep;
      rec.pathLength = stepLength;
      m_trackHits[trackID] = rec;
    }
    else
    {
      // If the track has already been recorded, update the existing record with the new energy deposit and step length
      it->second.energyDeposit += stepEdep;
      it->second.pathLength += stepLength;
    }
  }
  return true;
}

// At the end of the event, fill the ntuple with the recorded data for each track
void PositionFinder::EndOfEvent( G4HCofThisEvent* )
{
  auto analysisManager = G4AnalysisManager::Instance();
  G4int eventID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();

  for (const auto& [trackID, rec] : m_trackHits)
  {
    analysisManager->FillNtupleIColumn(m_ID, 0, eventID);
    analysisManager->FillNtupleIColumn(m_ID, 1, trackID);
    analysisManager->FillNtupleDColumn(m_ID, 2, rec.phi);
    analysisManager->FillNtupleDColumn(m_ID, 3, rec.theta);
    analysisManager->FillNtupleDColumn(m_ID, 4, rec.energyDeposit);
    analysisManager->FillNtupleDColumn(m_ID, 5, rec.pathLength);
    analysisManager->AddNtupleRow(m_ID);
  }
}
