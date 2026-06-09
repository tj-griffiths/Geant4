#include "EventAction.h"
#include "G4Step.hh"
#include "G4VProcess.hh"

#include "G4CsvAnalysisManager.hh"
using G4AnalysisManager = G4CsvAnalysisManager;

EventAction::EventAction() : G4UserEventAction()
{
}

EventAction::~EventAction()
{
}

void EventAction::BeginOfEventAction( const G4Event* )
{
  // clears all data structures at the beginning of each event
  m_recordedTracks.clear();
  m_allParticles.clear();
  m_bremPhotons.clear();
  m_electrons.clear();
  m_positrons.clear();
  m_muons.clear();
  photons.clear();
  m_graphEdges.clear();

}

void EventAction::EndOfEventAction( const G4Event* )
{
  G4cout << "Total particles in the event: " << m_allParticles.size() << G4endl;
  // had other print statements to check particle counts per event (like electron, positron, photons, but became way too cluttered when began doing the 1000 run simulations)
  // thats why there were so many m_{particles}
}

void EventAction::RecordParticleData( const G4Step* step )
{
  G4Track * track = step->GetTrack();
  std::string particle = track->GetParticleDefinition()->GetParticleName(); // gets the particle name from the track
  G4int trackID = track->GetTrackID();

  if ( track->GetCreatorProcess() )
  {
    std::string creator = track->GetCreatorProcess()->GetProcessName();

    if ( creator == "eBrem" && particle == "gamma" )
    {
      m_bremPhotons.insert( track->GetTrackID() );
    }
  }

  m_allParticles.insert( track->GetTrackID() );

  if ( particle == "e-" ) m_electrons.insert( track->GetTrackID() );
  else if ( particle == "e+" ) m_positrons.insert( track->GetTrackID() );
  else if ( particle == "mu+" || particle == "mu-" ) m_muons.insert( track->GetTrackID() );
  
  // Make a graph edge connecting a new track ID to its parent
  std::pair< int, int > edge = std::make_pair( track->GetParentID(), track->GetTrackID() );
  m_graphEdges.insert( edge );
}