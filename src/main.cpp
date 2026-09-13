#include "DetectorConstruction.h"
#include "ActionInitialization.h"

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "FTFP_BERT.hh"
#include "G4StepLimiterPhysics.hh"
#include "Randomize.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "G4MTRunManager.hh"

// If running more than 20 events (especially for ee dataset) - turn off visualizer

int main( int argc, char* argv[] )
{
  // Set up the random number generator
  G4Random::setTheEngine( new CLHEP::RanecuEngine );
  G4Random::setTheSeed( 1234 );

  // Construct the default run manager
  G4RunManager* runManager = new G4RunManager();

  runManager->SetUserInitialization( new DetectorConstruction() );

  G4VModularPhysicsList* physicsList = new FTFP_BERT();
  physicsList->RegisterPhysics( new G4StepLimiterPhysics() );
  runManager->SetUserInitialization( physicsList );

  runManager->SetUserInitialization( new ActionInitialization() );

  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  if ( argc > 1 )
  {
    // Batch mode — no UI, no VisManager
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand( command + fileName );
  }
  else
  {
    // Interactive mode — only place UI/VisManager get constructed
    G4UIExecutive* ui = new G4UIExecutive( argc, argv );
    G4VisManager* visManager = new G4VisExecutive();
    visManager->Initialize();

    UImanager->ApplyCommand( "/control/execute vis.mac" );
    UImanager->ApplyCommand( "/control/execute run.mac" );
    ui->SessionStart();

    delete ui;
    delete visManager;
  }

  delete runManager;
  return 0;
}