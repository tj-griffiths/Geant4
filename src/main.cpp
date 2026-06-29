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
  // Start interactive session using the command line arguments
  G4UIExecutive* ui = new G4UIExecutive( argc, argv );

  // Set up the random number generator
  G4Random::setTheEngine( new CLHEP::RanecuEngine );
  G4Random::setTheSeed( 1234 );

  // Construct the default run manager - switched to MT to handle better mac performace
  G4RunManager* runManager = new G4RunManager(); 

  // Set up detector
  runManager->SetUserInitialization( new DetectorConstruction() );

  // Set up physics processes
  G4VModularPhysicsList* physicsList = new FTFP_BERT();
  physicsList->RegisterPhysics( new G4StepLimiterPhysics() );
  runManager->SetUserInitialization( physicsList );

  // Set user action classes (just the generator really)
  runManager->SetUserInitialization( new ActionInitialization() );

  // Set up display
  G4VisManager* visManager = new G4VisExecutive(); //Comment out to disable Visualizer
  visManager->Initialize(); //Comment out to disable Visualizer

  // Set up the command line interface
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  // Used claude to setup a macro that runs the simulation headlessly (no vismanager, no UI session, no window)
  if ( argc > 1)
  {
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand( command + fileName );
  }
  else
  {
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
