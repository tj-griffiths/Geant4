#ifndef MuonSD_h
#define MuonSD_h 1

#include "G4VSensitiveDetector.hh"
#include "G4Step.hh"

// MuonSD records the phi and theta of the charged particles that reach
// the muon detector. Typically those would only be muons, so they'll be tagged as such

class MuonSD: public G4VSensitiveDetector
{
    // The constructor takes the name of the sensitive detector and the ID of the ntuple to save to (5 for muon detector)
  public:
    MuonSD( const G4String& name, G4int ntupleID );
    ~MuonSD() override;

    // Similar to Calorimeter, we initialize the map from layer number to energy deposited at the start of each event,
    //  and then fill it for every step in the volume, and save it to the ntuple at the end of the event
    void Initialize( G4HCofThisEvent* ) override; 
    G4bool ProcessHits( G4Step* step, G4TouchableHistory* ) override;
    void EndOfEvent( G4HCofThisEvent* ) override;

  private:
    G4int m_ntupleID; // ID of the ntuple to save to (5 for muon detector)
    std::set<G4int> m_recordedTracks; // prevent multiple muon recordings
};

#endif 