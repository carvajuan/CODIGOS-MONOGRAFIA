
//
/// \file optical/LXe/include/LXeTrackingAction.hh
/// \brief Definition of the LXeTrackingAction class
//
//
#ifndef RastreoAction_h
#define RastreoAction_h 1

#include "G4UserTrackingAction.hh"
#include "globals.hh"

class RastreoAction : public G4UserTrackingAction 
{

  public:

    RastreoAction();
    virtual ~RastreoAction();

    virtual void PreUserTrackingAction(const G4Track*);
    virtual void PostUserTrackingAction(const G4Track*);

  private:

};

#endif
