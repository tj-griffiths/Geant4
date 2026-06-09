#include "CalorimeterSD.h"

#include "G4Step.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4SystemOfUnits.hh"
#include "G4CsvAnalysisManager.hh"

using G4AnalysisManager = G4CsvAnalysisManager; // Use the CSV analysis manager

// This is similar to PositionFinder, but maps the energy in GeV deposited in the calorimeter for each event, and fills an ntuple with that information at the end of the event.

// Constructor (pass in the name of the sensitive detector and the ID of the ntuple)
CalorimeterSD::CalorimeterSD( const G4String& name, G4int ntupleID ) 
  : G4VSensitiveDetector( name ), m_ntupleID( ntupleID )
{
}

// Destructor (nothing to do here)
CalorimeterSD::~CalorimeterSD()
{
}

// This method is called at the beginning of each event, so we can use it to reset our energy deposit
void CalorimeterSD::Initialize( G4HCofThisEvent* )
{
    m_edep.clear();
}

// This method is called every time a particle takes a step in the sensitive volume
G4bool CalorimeterSD::ProcessHits( G4Step* step, G4TouchableHistory* )
{
    // Get the energy deposited in this step
    G4double edep = step->GetTotalEnergyDeposit();
    if ( edep == 0.0 ) return false; // No energy deposited, so ignore this step

    // here we get the eventID and layerID to save the energy deposited per layer and event
    G4int eventID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
    // Gets the copy number (layer index from DetectorConstruction loop i)
    G4int layerID = step->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber();
    // Saves the energy deposited per layer
    m_edep[eventID][layerID] += edep;


    return true;
}

// Called at end of event to fill the ntuple with the total energy deposited in this event
void CalorimeterSD::EndOfEvent( G4HCofThisEvent* )
{
    // Get the analysis manager
    auto analysisManager = G4AnalysisManager::Instance();

    // Loop over the eventID and layerID to give energy deposits
    for ( const auto& [eventID, layers] : m_edep )
    {
        for (const auto& [layerID, edep]: layers)
        {
            analysisManager->FillNtupleIColumn( m_ntupleID, 0, eventID );
            analysisManager->FillNtupleIColumn( m_ntupleID, 1, layerID );
            analysisManager->FillNtupleDColumn( m_ntupleID, 2, edep / GeV ); // Convert to GeV
            analysisManager->AddNtupleRow( m_ntupleID );  
        }
    }
}