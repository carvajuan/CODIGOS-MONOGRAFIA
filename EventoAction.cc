
/// \brief Implementation of the LXeEventAction class
//
//


#include "EventoAction.hh"
#include "AguaHit.hh"
#include "PmtHit.hh"
#include "Trayectoria.hh"
#include "Correr.hh"
#include "HistogramaManager.hh"
#include "ConstruccionDetector.hh"

#include "G4EventManager.hh"
#include "G4SDManager.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4VVisManager.hh"
#include "G4ios.hh"
#include "G4UImanager.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventoAction::EventoAction(const ConstruccionDetector* det)
  : fDetector(det),fScintCollID(-1),fPMTCollID(-1),fVerbose(0),
   fPMTThreshold(1),fForcedrawphotons(false),fForcenophotons(false)
{
  fEventMessenger = new EventoMessenger(this);

  fHitCount = 0;
  fPhotonCount_Scint = 0;
  fPhotonCount_Ceren = 0;
  fAbsorptionCount = 0;
  fBoundaryAbsorptionCount = 0;
  fTotE = 0.0;

  fConvPosSet = false;
  fEdepMax = 0.0;

  fPMTsAboveThreshold = 0;
}
 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventoAction::~EventoAction(){}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventoAction::BeginOfEventAction(const G4Event*) {
 
  fHitCount = 0;
  fPhotonCount_Scint = 0;
  fPhotonCount_Ceren = 0;
  fAbsorptionCount = 0;
  fBoundaryAbsorptionCount = 0;
  fTotE = 0.0;

  fConvPosSet = false;
  fEdepMax = 0.0;

  fPMTsAboveThreshold = 0;

  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  if(fScintCollID<0)
    fScintCollID=SDman->GetCollectionID("AguaCollection");//REVISAR LOS NOMBRES QUE LE PUSE 
  if(fPMTCollID<0)
    fPMTCollID=SDman->GetCollectionID("pmtHitCollection");
}
 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventoAction::EndOfEventAction(const G4Event* anEvent)

{

	
	
        




  G4TrajectoryContainer* trajectoryContainer=anEvent->GetTrajectoryContainer();
 
  G4int n_trajectories = 0;
  if (trajectoryContainer) n_trajectories = trajectoryContainer->entries();

  // extract the trajectories and draw them
  if (G4VVisManager::GetConcreteInstance()){
    for (G4int i=0; i<n_trajectories; i++){
      Trayectoria* trj = (Trayectoria*)
        ((*(anEvent->GetTrajectoryContainer()))[i]);
      if(trj->GetParticleName()=="opticalphoton"){
        trj->SetForceDrawTrajectory(fForcedrawphotons);
        trj->SetForceNoDrawTrajectory(fForcenophotons);
      }
      trj->DrawTrajectory();
    }
  }
 
  AguaHitsCollection* scintHC = nullptr; //REVISSSSSAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAR
  PmtHitsCollection* pmtHC = nullptr;
  G4HCofThisEvent* hitsCE = anEvent->GetHCofThisEvent();
 
  //Get the hit collections
  if(hitsCE){
    if(fScintCollID>=0) {
      scintHC = (AguaHitsCollection*)(hitsCE->GetHC(fScintCollID));
    }
    if(fPMTCollID>=0) {
      pmtHC = (PmtHitsCollection*)(hitsCE->GetHC(fPMTCollID));
    }
  }

  //Hits in scintillator
  if(scintHC){
    int n_hit = scintHC->entries();
    G4ThreeVector  eWeightPos(0.);
    G4double edep;
    G4double edepMax=0;

    for(int i=0;i<n_hit;i++){ //gather info on hits in scintillator
      edep=(*scintHC)[i]->GetEdep();
      fTotE += edep;
      eWeightPos += (*scintHC)[i]->GetPos()*edep;//calculate energy weighted pos
      if(edep>edepMax){
        edepMax=edep;//store max energy deposit
        G4ThreeVector posMax=(*scintHC)[i]->GetPos();
        fPosMax = posMax;
        fEdepMax = edep;
      }
    }
    
    G4AnalysisManager::Instance()->FillH1(7, fTotE);
    
    if(fTotE == 0.){
      if(fVerbose>0)G4cout<<"No HITS en el agua en este evento."<<G4endl;
    }
    else{
      //Finish calculation of energy weighted position
      eWeightPos /= fTotE;
      fEWeightPos = eWeightPos; 
      if(fVerbose>0){
        G4cout << "\tEnergy weighted position of hits in Agua : "
               << eWeightPos/mm << G4endl;
      }
    }
    if(fVerbose>0){
    G4cout << "\tTotal energia depositada en el tanque fue : "
           << fTotE / keV << " (keV)" << G4endl;
    }
  }
 
  if(pmtHC){
    G4ThreeVector reconPos(0.,0.,0.);
    G4int pmts=pmtHC->entries();
    //Gather info from all PMTs
    for(G4int i=0;i<pmts;i++){
      fHitCount += (*pmtHC)[i]->GetPhotonCount();
      reconPos+=(*pmtHC)[i]->GetPMTPos()*(*pmtHC)[i]->GetPhotonCount();
      if((*pmtHC)[i]->GetPhotonCount()>=fPMTThreshold){
        fPMTsAboveThreshold++;
      }
      else{//wasnt above the threshold, turn it back off
        (*pmtHC)[i]->SetDrawit(false);
      }
    }

    G4AnalysisManager::Instance()->FillH1(1, fHitCount);
    G4AnalysisManager::Instance()->FillH1(2, fPMTsAboveThreshold);

    if(fHitCount > 0) {//dont bother unless there were hits
      reconPos/=fHitCount;
      if(fVerbose>0){
        G4cout << "\tReconstructed position of hits in LXe : "
               << reconPos/mm << G4endl;
      }
      fReconPos = reconPos;
    }
    pmtHC->DrawAllHits();
  }

  G4AnalysisManager::Instance()->FillH1(3, fPhotonCount_Scint);
  G4AnalysisManager::Instance()->FillH1(4, fPhotonCount_Ceren);
  G4AnalysisManager::Instance()->FillH1(5, fAbsorptionCount);
  G4AnalysisManager::Instance()->FillH1(6, fBoundaryAbsorptionCount);


	archivo = fopen("PRUEBASERVIDOR.txt","a");
	fprintf(archivo,"%d %d %d %d %d %d %f \n",fHitCount,fPMTsAboveThreshold,
	fPhotonCount_Scint,fPhotonCount_Ceren,fAbsorptionCount,fBoundaryAbsorptionCount,fTotE/MeV);
	fclose(archivo);

  if(fVerbose>0){
    //End of event output. later to be controlled by a verbose level
    G4cout << "\tNumber of photons that hit PMTs in this event : "
           << fHitCount << G4endl;
    G4cout << "\tNumber of PMTs above threshold("<<fPMTThreshold<<") : "
           << fPMTsAboveThreshold << G4endl;
    G4cout << "\tNumber of photons produced by scintillation in this event : "
           << fPhotonCount_Scint << G4endl;
    G4cout << "\tNumber of photons produced by cerenkov in this event : "
           << fPhotonCount_Ceren << G4endl;
    G4cout << "\tNumber of photons absorbed (OpAbsorption) in this event : "
           << fAbsorptionCount << G4endl;
    G4cout << "\tNumber of photons absorbed at boundaries (OpBoundary) in "
           << "this event : " << fBoundaryAbsorptionCount << G4endl;
    G4cout << "Unaccounted for photons in this event : "
           << (fPhotonCount_Scint + fPhotonCount_Ceren -
                fAbsorptionCount - fHitCount - fBoundaryAbsorptionCount)
           << G4endl;
  }

  // update the run statistics
  Correr* run = static_cast<Correr*>(
    G4RunManager::GetRunManager()->GetNonConstCurrentRun());

  run->IncHitCount(fHitCount);
  run->IncPhotonCount_Scint(fPhotonCount_Scint);
  run->IncPhotonCount_Ceren(fPhotonCount_Ceren);
  run->IncEDep(fTotE);
  run->IncAbsorption(fAbsorptionCount);
  run->IncBoundaryAbsorption(fBoundaryAbsorptionCount);
  run->IncHitsAboveThreshold(fPMTsAboveThreshold);

  //If we have set the flag to save 'special' events, save here
  //if(fPhotonCount_Scint + fPhotonCount_Ceren < fDetector->GetSaveThreshold())
  //{
    //G4RunManager::GetRunManager()->rndmSaveThisEvent();
  //}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
