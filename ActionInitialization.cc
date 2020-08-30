
/// \file LXeActionInitialization.cc
/// \brief Implementation of the LXeActionInitialization class

#include "ActionInitialization.hh"

#include "PrimarioGeneratorAction.hh"
#include "ConstruccionDetector.hh"

#include "CorrerAction.hh"
#include "EventoAction.hh"
#include "RastreoAction.hh"
#include "PasoAction.hh"
#include "AcumularAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ActionInitialization::ActionInitialization(
   const ConstruccionDetector* det)
  : G4VUserActionInitialization(), fDetector(det)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ActionInitialization::~ActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ActionInitialization::BuildForMaster() const
{
  SetUserAction(new CorrerAction());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ActionInitialization::Build() const
{
  SetUserAction(new PrimarioGeneratorAction());

  EventoAction* eventAction = new EventoAction(fDetector);
  SetUserAction(eventAction);
  SetUserAction(new AcumularAction(eventAction));

  SetUserAction(new CorrerAction());
  SetUserAction(new RastreoAction());
  SetUserAction(new PasoAction(eventAction));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
