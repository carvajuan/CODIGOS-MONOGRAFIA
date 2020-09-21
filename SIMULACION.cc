
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4Types.hh"

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "G4UImanager.hh"
#include "G4String.hh"

#include "FTFP_BERT.hh"
#include "G4OpticalPhysics.hh"
#include "G4EmStandardPhysics_option4.hh"

#include "ConstruccionDetector.hh"
#include "ActionInitialization.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "Randomize.hh"

#include "time.h"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc, char** argv)
{
  //detect interactive mode (if no arguments) and define UI session
  G4UIExecutive* ui = nullptr;
  if (argc == 1) { ui = new G4UIExecutive(argc,argv); }
  
  
  
  G4Random::setTheEngine(new CLHEP::RanecuEngine());
  
  G4long seed = time(NULL);
  
  G4Random::setTheSeed(seed);
  
  // Optionally: choose a different Random engine...
  //G4Random::setTheEngine(new CLHEP::RanluxEngine);







#ifdef G4MULTITHREADED
  G4MTRunManager * runManager = new G4MTRunManager;
  G4int nThreads = std::min(G4Threading::G4GetNumberOfCores(), 4);
  runManager->SetNumberOfThreads(nThreads);
  G4cout << "===== LXe is started with "
         <<  runManager->GetNumberOfThreads() << " threads =====" << G4endl;



  //
  //PHYSICS LIST
  //
#else
  G4RunManager * runManager = new G4RunManager;
#endif
  ConstruccionDetector* det = new ConstruccionDetector();
  runManager->SetUserInitialization(det);

  G4VModularPhysicsList* physicsList = new FTFP_BERT;
  physicsList->ReplacePhysics(new G4EmStandardPhysics_option4());
  G4OpticalPhysics* opticalPhysics = new G4OpticalPhysics();
  
  physicsList->RegisterPhysics(opticalPhysics);
  runManager->SetUserInitialization(physicsList);

  runManager->SetUserInitialization(new ActionInitialization(det));

  //
  //PHYSICS LIST END///////////////////
  //

  //initialize visualization
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();

  //get the pointer to the User Interface manager 
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  if (ui) {
    //interactive mode
    UImanager->ApplyCommand("/control/execute vis.mac");
    if (ui->IsGUI()) {
      UImanager->ApplyCommand("/control/execute gui.mac");
    }
    ui->SessionStart();
    delete ui;
  } else {
    //batch mode  
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command+fileName);
  }

  // job termination
  delete visManager;
  delete runManager;
  return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
