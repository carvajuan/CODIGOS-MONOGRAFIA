
//
#include "Trayectoria.hh"
#include "RastreoAction.hh"
#include "UsuarioTrackInformation.hh"
#include "ConstruccionDetector.hh"

#include "G4TrackingManager.hh"
#include "G4Track.hh"
#include "G4ParticleTypes.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RastreoAction::RastreoAction()
{}

RastreoAction::~RastreoAction() 
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RastreoAction::PreUserTrackingAction(const G4Track* aTrack)
{
  //Let this be up to the user via vis.mac
  //  fpTrackingManager->SetStoreTrajectory(true);

  //Use custom trajectory class
  fpTrackingManager->SetTrajectory(new Trayectoria(aTrack));

  //This user track information is only relevant to the photons
  fpTrackingManager->SetUserTrackInformation(new UsuarioTrackInformation);

  /*  const G4VProcess* creator = aTrack->GetCreatorProcess();
  if(creator)
    G4cout<<creator->GetProcessName()<<G4endl;
  */
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RastreoAction::PostUserTrackingAction(const G4Track* aTrack)
{
  Trayectoria* trajectory =
    (Trayectoria*)fpTrackingManager->GimmeTrajectory();
  UsuarioTrackInformation*
    trackInformation=(UsuarioTrackInformation*)aTrack->GetUserInformation();

  //Lets choose to draw only the photons that hit the sphere and a pmt
  if(aTrack->GetDefinition()==G4OpticalPhoton::OpticalPhotonDefinition())
  {
      if(trackInformation->GetTrackStatus()&hitPMT)
      {
        trajectory->SetDrawTrajectory(false);
      }
      else
        trajectory->SetDrawTrajectory(false);
   
  }
  else //draw all other trajectories
    trajectory->SetDrawTrajectory(false);

  if(trackInformation->GetForceDrawTrajectory())
    trajectory->SetDrawTrajectory(true);
}
