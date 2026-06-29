#ifndef PositionFinder_h
#define PositionFinder_h 1

#include "G4VSensitiveDetector.hh"
#include <map>


struct TrackerHitRecord
{
    G4double phi;
    G4double theta;
    G4double energyDeposit;
    G4double pathLength;
};


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
    std::map<G4int, TrackerHitRecord> m_trackHits; // Map to store the hit records for each track ID
};

#endif
