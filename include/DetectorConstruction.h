#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "G4GlobalMagFieldMessenger.hh"
#include "G4LogicalVolume.hh"

#include <vector>

// Define the experiment to be simulated
class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    DetectorConstruction();
    ~DetectorConstruction() override;

    G4VPhysicalVolume* Construct() override;
    void ConstructSDandField() override;

  private:
    // Global magnetic field messenger
    static G4ThreadLocal G4GlobalMagFieldMessenger* m_magneticFieldMessenger;

    // The commented out lines were from an incorrect method of only setting the last layer as sensitive material
    // G4LogicalVolume* m_emCalLV = nullptr; // EM Cal. 
    std::vector<G4LogicalVolume*> m_emCalLVs;
    // G4LogicalVolume* m_hCalLV = nullptr; // Hadron Cal.
    std::vector<G4LogicalVolume*> m_hCalLVs;
    G4LogicalVolume* m_muonLV = nullptr; // Muon Detector
};

#endif
