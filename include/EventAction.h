#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "G4Event.hh"
#include "G4Step.hh"
#include "G4RunManager.hh"
#include "G4SystemofUnits.hh"
#include "G4CsvAnalysisManager.hh"

#include <set>
#include <utility>

class EventAction : public G4UserEventAction
{
  public:
    EventAction();
    ~EventAction() override;

    void BeginOfEventAction( const G4Event* ) override;
    void EndOfEventAction( const G4Event* ) override;

    // Called by SteppingAction every step
    void RecordParticleData( const G4Step* step );

    private:
    std::set< std::pair< int, int>> m_graphEdges; // Set of edges in the track history graph (parent track ID, child track ID)
    std::set< int > m_bremPhotons;
    std::set< int > m_allParticles;
    std::set< int > m_electrons;
    std::set< int > m_positrons;
    std::set< int > m_muons;
    std::set< int > photons;

    // Tracks 
    std::set< int > m_recordedTracks;
};

#endif