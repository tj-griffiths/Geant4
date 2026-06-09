#ifndef PositionFinder_h
#define PositionFinder_h 1

#include "G4VSensitiveDetector.hh"


class PositionFinder : public G4VSensitiveDetector
{
  public:
    PositionFinder( const G4String& name, const G4int id );
    ~PositionFinder() override;

    void Initialize( G4HCofThisEvent* ) override;
    G4bool ProcessHits( G4Step* step, G4TouchableHistory* ) override; //  check if anything hit it at each step
    void EndOfEvent( G4HCofThisEvent* ) override;

  private:
    G4int m_ID;
    std::set<G4int> m_recordedTracks; // Keep track of which tracks have already been recorded, so we only record one hit per track
};

#endif
