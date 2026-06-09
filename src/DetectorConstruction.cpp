#include "DetectorConstruction.h"
#include "PositionFinder.h"

#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4AutoDelete.hh"
#include "G4GeometryManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4SDManager.hh"
#include "CalorimeterSD.h"
#include "MuonSD.h"


G4ThreadLocal
G4GlobalMagFieldMessenger* DetectorConstruction::m_magneticFieldMessenger = 0;

DetectorConstruction::DetectorConstruction() : G4VUserDetectorConstruction()
{
}

DetectorConstruction::~DetectorConstruction()
{
}

// Here we define the actual experiment that we want to perform
G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // Materials
  // http://geant4-userdoc.web.cern.ch/geant4-userdoc/UsersGuides/ForApplicationDeveloper/html/Appendix/materialNames.html
  G4NistManager* nistManager = G4NistManager::Instance();
  G4Material* air = nistManager->FindOrBuildMaterial( "G4_AIR" );
  G4Material* silicon = nistManager->FindOrBuildMaterial( "G4_Si" ); // Silicon is a common tracker material
  G4Material* lead = nistManager->FindOrBuildMaterial( "G4_Pb" ); // Lead is a common EM calorimeter absorber
  G4Material* uranium = nistManager->FindOrBuildMaterial( "G4_U" ); // Could use Uranium for Hadron and EM calorimeter material with higher density and smaller radiation length, but less
  G4Material* iron = nistManager->FindOrBuildMaterial( "G4_Fe" ); // Iron is a common hadron calorimeter absorber
  G4Material* scint = nistManager->FindOrBuildMaterial( "G4_POLYSTYRENE" ); // Scintillator material
  G4Material* alumin = nistManager->FindOrBuildMaterial( "G4_Al" );
  G4Material* lAr = nistManager->FindOrBuildMaterial( "G4_lAr" ); // Liquid Argon is a common EM calorimeter active material

  // Sizes of the principal geometrical components (solids)
  G4double tracker1Length = 400.0*cm;
  G4double tracker1InnerRadius = 399.8*cm;
  G4double tracker1OuterRadius = 400.0*cm;
  G4double tracker2Length = 800.0*cm;
  G4double tracker2InnerRadius = 799.8*cm;
  G4double tracker2OuterRadius = 800.0*cm;
  G4double worldLength = 2000.0*cm; // Increased world size to fit larger calorimeters and muon detector originally 1000cm

  // EM Calorimeter: 15 layers of Pb and LAr (originally had 10 layers)
  // Starts Immediately after Tracker 2
  const G4int nEM = 50;
  const G4double emPb = 0.3 * cm;
  const G4double emLAr = 0.9 * cm;
  const G4double emLayer = emPb + emLAr;
  G4double emStart = tracker2OuterRadius;

  // Hadron Calorimeter: 20 layers of Fe and Scintillator (originally had 10 layers)
  const G4int nHC = 50;
  const G4double hcFe = 0.9 * cm;
  const G4double hcSc = 2.7 * cm;
  const G4double hcLayer = hcFe + hcSc;
  G4double hcStart = emStart + nEM * emLayer;

  // Muon Detector: Single Al shell
  G4double muInner = hcStart + nHC * hcLayer;
  G4double muOuter = muInner + 400.0*cm;  // Had to keep increasing to capture more muon hits.
  
  // Kept increasing the sizes of the calorimeters & detectors to increase the
  // chance the particles hit and shower fully in the layers.

  G4double calHalfZ = tracker2Length; //  same as tracker 2

  // Added Muon Endcaps at +/- z ends to ensure muon hits.
  G4double endcapHalfZ = 400.0 * cm; // thickness of cap
  G4double endcapZPos = tracker2Length + endcapHalfZ + 1.0 * cm;  // added the cm to not worry about overlap

  G4double beamAperature = 5.0 * cm; // opening aperature of ppbar to enter (included for realism) 


  // Definitions of Solids, Logical Volumes, Physical Volumes

  // WORLD: Solid (cube)
  G4GeometryManager::GetInstance()->SetWorldMaximumExtent( worldLength );
  G4Box* worldS = new G4Box(
                 "World",         // its name
                 worldLength,
                 worldLength,
                 worldLength );   // its size (in half-lengths)

  // WORLD: Logical volume (how to treat it)
  G4LogicalVolume* worldLV = new G4LogicalVolume(
                 worldS,          // its solid
                 air,             // its material
                 "World" );       // its name

  // WORLD: Physical volume (where is it)
  // Must place the World Physical volume unrotated at (0,0,0).
  G4VPhysicalVolume* worldPV = new G4PVPlacement(
                 0,               // no rotation
                 G4ThreeVector(0.0, 0.0, 0.0), // in the centre
                 worldLV,         // its logical volume
                 "World",         // its name
                 0,               // its mother volume
                 false,           // no boolean operations
                 0,               // copy number
                 true );          // checking overlaps

  // TRACKER1: Solid (tube)
  G4Tubs* tracker1S = new G4Tubs(
                 "Tracker1",        // its name
                 tracker1InnerRadius, // Hollow cylinder
                 tracker1OuterRadius,
                 tracker1Length,    // how far along the beam axis it extends
                 0.0*deg,           // starting angle
                 360.0*deg );       // ending angle (i.e. it's a full circle)

  // TRACKER1: Logical volume (how to treat it)
  G4LogicalVolume* tracker1LV = new G4LogicalVolume(
                 tracker1S,         // its solid
                 silicon,           // its material
                 "Tracker1",        // its name
                 0, 0, 0 );         // Modifiers we don't use
  // TRACKER1: Physical volume (where is it)
  G4VPhysicalVolume* tracker1PV = new G4PVPlacement(
                 0,                 // no rotation
                 G4ThreeVector(0.0, 0.0, 0.0), // in the centre
                 tracker1LV,        // its logical volume
                 "Tracker1",        // its name
                 worldLV,           // its mother volume
                 false,             // no boolean operations
                 0,                 // copy number
                 true );            // checking overlaps

  // TRACKER1: Quit if there's an overlap
  if ( tracker1PV->CheckOverlaps() ) std::cerr << "WARNING: your simulated objects overlap" << std::endl;

  // TRACKER2: Solid (tube)
  G4Tubs* tracker2S = new G4Tubs(
                 "Tracker2",        // its name
                 tracker2InnerRadius, // Hollow cylinder
                 tracker2OuterRadius,
                 tracker2Length,    // how far along the beam axis it extends
                 0.0*deg,           // starting angle
                 360.0*deg );       // ending angle (i.e. it's a full circle)

  // TRACKER2: Logical volume (how to treat it)
  G4LogicalVolume* tracker2LV = new G4LogicalVolume(
                 tracker2S,         // its solid
                 silicon,           // its material
                 "Tracker2",        // its name
                 0, 0, 0 );         // Modifiers we don't use

  // TRACKER2: Physical volume (where is it)
  G4VPhysicalVolume* tracker2PV = new G4PVPlacement(
                 0,                 // no rotation
                 G4ThreeVector(0.0, 0.0, 0.0), // in the centre
                 tracker2LV,        // its logical volume
                 "Tracker2",        // its name
                 worldLV,           // its mother volume
                 false,             // no boolean operations
                 0,                 // copy number
                 true );            // checking overlaps

  // TRACKER2: Quit if there's an overlap
  if ( tracker2PV->CheckOverlaps() ) std::cerr << "WARNING: your simulated objects overlap" << std::endl;

  // Calorimeters and Muon Detector: Loop over layers and place them

  // EM Calorimeter
  for (G4int i = 0; i < nEM; i++)
  {
    // Lead Absorber
    G4double pbInner = emStart + i * emLayer;
    G4double pbOuter = pbInner + emPb;
    G4Tubs* pbS = new G4Tubs(
              "EmPb",
              pbInner,
              pbOuter,
              calHalfZ,
              0.0*deg,
              360.0*deg );
    G4LogicalVolume* pbLV = new G4LogicalVolume(
              pbS,
              lead,
              "EmPb",
              0, 0, 0 );
    G4VPhysicalVolume* pbPV = new G4PVPlacement(
              0,
              G4ThreeVector(),
              pbLV,
              "EmPb" + std::to_string(i),
              worldLV,
              false,
              i,
              true);

    // LAr Detector
    G4double lArInner = pbOuter;
    G4double lArOuter = lArInner + emLAr;
    G4Tubs* lArS = new G4Tubs(
              "EmLAr",
              lArInner,
              lArOuter,
              calHalfZ,
              0.0*deg,
              360.0*deg );
    // m_emCalLV = new G4LogicalVolume(
    //           lArS,
    //           lAr,
    //           "EmLAr",
    //           0, 0, 0 ); // only stored last layer
    auto* lArLV = new G4LogicalVolume(
              lArS,
              lAr,
              "EmLAr",
              0,
              0,
              0);
    m_emCalLVs.push_back(lArLV); // stores all layers
    G4VPhysicalVolume* lArPV = new G4PVPlacement(
              0,
              G4ThreeVector(),
              lArLV,
              "EmLAr" + std::to_string(i),
              worldLV,
              false,
              i,
              true);
  }

  // Hadron Calorimeter
  for (G4int i = 0; i < nHC; i++)
  {
    // Iron Absorber
    G4double feInner = hcStart + i * hcLayer;
    G4double feOuter = feInner + hcFe;
    G4Tubs* feS = new G4Tubs(
              "HcFe",
              feInner,
              feOuter,
              calHalfZ,
              0.0*deg,
              360.0*deg );
    G4LogicalVolume* feLV = new G4LogicalVolume(
              feS,
              iron,
              "HcFe",
              0, 0, 0 );
    G4VPhysicalVolume* fePV = new G4PVPlacement(
              0,
              G4ThreeVector(),
              feLV,
              "HcFe" + std::to_string(i),
              worldLV,
              false,
              i,
              true);

    // Scintillator Detector
    G4double scInner = feOuter;
    G4double scOuter = scInner + hcSc;
    G4Tubs* scS = new G4Tubs(
              "HcSc",
              scInner,
              scOuter,
              calHalfZ,
              0.0*deg,
              360.0*deg );
    // m_hCalLV = new G4LogicalVolume(
    //           scS,
    //           scint,
    //           "HcSc",
    //           0, 0, 0 ); // Only stores last layer
    auto* scLV = new G4LogicalVolume(
              scS,
              scint,
              "HcSc",
              0,
              0,
              0);
    m_hCalLVs.push_back(scLV); // stores all layers
    G4VPhysicalVolume* scPV = new G4PVPlacement(
              0,
              G4ThreeVector(),
              scLV,
              "HcSc" + std::to_string(i),
              worldLV,
              false,
              i,
              true);
            
  }

  // Muon Detector
  G4Tubs* muS = new G4Tubs(
              "MuonDetector",
              muInner,
              muOuter,
              calHalfZ,
              0.0*deg,
              360.0*deg );
  G4LogicalVolume* muLV = new G4LogicalVolume(
              muS,
              alumin,
              "MuonDetector",
              0, 0, 0 );
  G4VPhysicalVolume* muPV = new G4PVPlacement(
              0,
              G4ThreeVector(),
              muLV,
              "MuonDetector",
              worldLV,
              false,
              0,
              true);
      
  if ( muPV->CheckOverlaps() ) std::cerr << "WARNING: your simulated objects overlap" << std::endl;

// Muon Endcaps - These are pretty transparent in the visualizer: alpha = 0.2 to visibility
 G4Tubs* muEndcapS = new G4Tubs( 
              "MuonEndcap",
              beamAperature, // inner radius - left small aperature for where particles enter for 'realism' = 10 cm
              muOuter, // same radius as barrel
              endcapHalfZ,
              0.0*deg,
              360*deg);
  G4LogicalVolume* muEndcapLV = new G4LogicalVolume(
              muEndcapS,
              alumin,
              "MuonEndcap",
              0,
              0,
              0);
  
  // Place at +z
  new G4PVPlacement(0, G4ThreeVector(0,0, endcapZPos),
              muEndcapLV, "MuonEndcapPos", worldLV, false, 0, true);

  // Place at -z
  new G4PVPlacement(0, G4ThreeVector(0,0, -endcapZPos),
              muEndcapLV, "MuonEndcapNeg", worldLV, false, 1, true);


  // Always return the physical world
  return worldPV;
}

// Set up the magnetic field
void DetectorConstruction::ConstructSDandField()
{
  // Create global magnetic field messenger.
  // Uniform magnetic field is then created automatically if
  // the field value is not zero.
  G4ThreeVector fieldValue = G4ThreeVector(0.0, 0.0, 0.5 * tesla);
  m_magneticFieldMessenger = new G4GlobalMagFieldMessenger( fieldValue);

  // Register the field messenger for deleting
  G4AutoDelete::Register( m_magneticFieldMessenger );

  // The silicon layers are for tracking
  auto tracker1 = new PositionFinder( "Tracker1", 1 );
  G4SDManager::GetSDMpointer()->AddNewDetector( tracker1 );
  this->SetSensitiveDetector( "Tracker1", tracker1 );

  auto tracker2 = new PositionFinder( "Tracker2", 2 );
  G4SDManager::GetSDMpointer()->AddNewDetector( tracker2 );
  this->SetSensitiveDetector( "Tracker2", tracker2 );

  // EM Cal active layer (ntuple 3)
  auto emSD = new CalorimeterSD("EmCalSD", 3);
  G4SDManager::GetSDMpointer()->AddNewDetector( emSD );
  // this->SetSensitiveDetector(m_emCalLV, emSD); 
  for (auto*lv: m_emCalLVs)
      this->SetSensitiveDetector(lv,emSD); // sets all layers to be sensitive

  // Hadron Cal active layer (ntuple 4)
  auto hcSD = new CalorimeterSD("HcCalSD", 4);
  G4SDManager::GetSDMpointer()->AddNewDetector( hcSD );
  // this->SetSensitiveDetector(m_hCalLV, hcSD);
  for (auto*lv: m_hCalLVs)
      this->SetSensitiveDetector(lv,hcSD); // sets all layers to be sensitive

  // Muon Detector (ntuple 5)
  auto muSD = new MuonSD("MuonSD", 5);
  G4SDManager::GetSDMpointer()->AddNewDetector( muSD );
  this->SetSensitiveDetector("MuonDetector", muSD);

  // Muon Endcap - same as Muon Detector
  auto muEndcapSD = new MuonSD("MuonEndcapSD", 5);
  G4SDManager::GetSDMpointer()->AddNewDetector(muEndcapSD);
  this->SetSensitiveDetector("MuonEndcap", muEndcapSD);

}
