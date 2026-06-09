#ifndef CalorimeterSD_h
#define CalorimeterSD_h 1

#include "G4VSensitiveDetector.hh"
#include "G4Step.hh"
#include <map>

// CalorimeterSD accumulates energy deposited in the calorimeter
// layers (LAr for EMCal, and scintillator for HCal) and saves it in an ntuple
// one row to the output ntuple at the end of each event

class CalorimeterSD : public G4VSensitiveDetector
{
  public:

  // The constructor takes the name of the sensitive detector and the ID of the ntuple to save to (3 for EMCal, 4 for HCal)
    CalorimeterSD( const G4String& name, G4int ntupleID );
    ~CalorimeterSD() override;

    // We Initialize the map from layer number to energy deposited at the start of each event
    void Initialize(G4HCofThisEvent*) override; 
    // This method is called by Geant4 for every step in the volume
    G4bool ProcessHits( G4Step* step, G4TouchableHistory*) override;
    void EndOfEvent( G4HCofThisEvent* ) override;
    // Then end of the event, we save the total energy deposited in each layer to the ntuple and reset the map for the next event

  private:
    G4int m_ntupleID; // ID of the ntuple to save to (3 for EMCal, 4 for HCal)
    std::map<G4int, std::map<G4int, G4double>> m_edep; // eventID -> layerID -> energy deposited
};

#endif
