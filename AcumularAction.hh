
/// \file optical/LXe/include/LXeStackingAction.hh
/// \brief Definition of the LXeStackingAction class
//
//
#ifndef AcumularAction_H
#define AcumularAction_H 1

#include "globals.hh"
#include "G4UserStackingAction.hh"

class EventoAction;

class AcumularAction : public G4UserStackingAction
{
  public:

    AcumularAction(EventoAction*);
    virtual ~AcumularAction();
 
    virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* aTrack);
    virtual void NewStage();
    virtual void PrepareNewEvent();
 
  private:
    EventoAction* fEventAction;
};

#endif
