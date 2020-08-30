
/// \file optical/LXe/include/LXeSteppingAction.hh
/// \brief Definition of the LXeSteppingAction class
//
#ifndef PasoAction_H
#define PasoAction_H 1

#include "globals.hh"
#include "G4UserSteppingAction.hh"

#include "G4OpBoundaryProcess.hh"

class EventoAction;
class RastreoAction;
class PasoMessenger;

class PasoAction : public G4UserSteppingAction
{
  public:

    PasoAction(EventoAction*);
    virtual ~PasoAction();
    virtual void UserSteppingAction(const G4Step*);

    void SetOneStepPrimaries(G4bool b){fOneStepPrimaries=b;}
    G4bool GetOneStepPrimaries(){return fOneStepPrimaries;}
 
  private:

    G4bool fOneStepPrimaries;
    PasoMessenger* fSteppingMessenger;
    EventoAction*       fEventAction;

    G4OpBoundaryProcessStatus fExpectedNextStatus;
};

#endif
