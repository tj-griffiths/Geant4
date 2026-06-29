#include "RunAction.h"

#include "G4CsvAnalysisManager.hh"
using G4AnalysisManager = G4CsvAnalysisManager;

RunAction::RunAction() : G4UserRunAction()
{
  // Create analysis manager
  auto analysisManager = G4AnalysisManager::Instance();

  // Add an ntuple for truth (ntuple id 0)
  analysisManager->CreateNtuple( "Truth", "Truth information" );
  analysisManager->CreateNtupleIColumn( "EventNumber" );
  analysisManager->CreateNtupleDColumn( "Phi" );
  analysisManager->CreateNtupleDColumn( "Theta" );
  analysisManager->CreateNtupleDColumn( "Momentum" );
  analysisManager->CreateNtupleIColumn( "ParticleCode");
  analysisManager->CreateNtupleIColumn( "Charge");
  analysisManager->FinishNtuple();

  // Add an ntuple for tracker layer 1 (ntuple id 1)
  analysisManager->CreateNtuple( "Tracker1", "Tracker 1 coordinates" );
  analysisManager->CreateNtupleIColumn( "EventNumber" );
  analysisManager->CreateNtupleIColumn( "TrackID" );
  analysisManager->CreateNtupleDColumn( "Phi" );
  analysisManager->CreateNtupleDColumn( "Theta" );
  analysisManager->CreateNtupleDColumn( "EnergyDeposited" );
  analysisManager->CreateNtupleDColumn( "PathLength");
  analysisManager->FinishNtuple();

  // Add an ntuple for tracker layer 2 (ntuple id 2)
  analysisManager->CreateNtuple( "Tracker2", "Tracker 2 coordinates" );
  analysisManager->CreateNtupleIColumn( "EventNumber" );
  analysisManager->CreateNtupleIColumn( "TrackID" );
  analysisManager->CreateNtupleDColumn( "Phi" );
  analysisManager->CreateNtupleDColumn( "Theta" );
  analysisManager->CreateNtupleDColumn( "EnergyDeposited" );
  analysisManager->CreateNtupleDColumn( "PathLength");
  analysisManager->FinishNtuple();

  // Add an ntuple for EM Cal (ntuple id 3)
  analysisManager->CreateNtuple( "EMCal", "Energy deposited in EM Calorimeter" );
  analysisManager->CreateNtupleIColumn( "EventNumber" );
  analysisManager->CreateNtupleIColumn( "Layer");
  analysisManager->CreateNtupleDColumn( "Energy" );
  analysisManager->FinishNtuple();

  // Add an ntuple for HCal (ntuple id 4)
  analysisManager->CreateNtuple( "HCal", "Energy deposited in Hadron Calorimeter" );
  analysisManager->CreateNtupleIColumn( "EventNumber" );
  analysisManager->CreateNtupleIColumn( "Layer");
  analysisManager->CreateNtupleDColumn( "Energy" );
  analysisManager->FinishNtuple();

  // Add an ntuple for Muon Detector (ntuple id 5)
  analysisManager->CreateNtuple( "MuonDetector", "Coordinates of hits in Muon Detector" );
  analysisManager->CreateNtupleIColumn( "EventNumber" );
  analysisManager->CreateNtupleDColumn( "Phi" );
  analysisManager->CreateNtupleDColumn( "Theta" );
  analysisManager->FinishNtuple();
}
// Got rid of the delete instance since it would create crash windows
// when closing the program.
RunAction::~RunAction()
{
}

void RunAction::BeginOfRunAction( const G4Run* )
{
  // Get analysis manager
  auto analysisManager = G4AnalysisManager::Instance();

  // Open an output file
  analysisManager->OpenFile( "output.csv" ); // Gives a csv file as output for each ntuple (truth and detector segments)
}

void RunAction::EndOfRunAction( const G4Run* )
{
  // Get analysis manager
  auto analysisManager = G4AnalysisManager::Instance();

  // Save output data
  analysisManager->Write();
  analysisManager->CloseFile();
}
