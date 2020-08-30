
/// \file optical/LXe/include/LXeEventMessenger.hh
/// \brief Definition of the LXeEventMessenger class
//
//
#ifndef EventoMessenger_h
#define EventoMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class EventoAction;
class G4UIcmdWithAnInteger;
class G4UIcmdWithABool;

class EventoMessenger: public G4UImessenger
{
  public:

    EventoMessenger(EventoAction*);
    virtual ~EventoMessenger();
 
    virtual void SetNewValue(G4UIcommand*, G4String);
 
  private:

    EventoAction*        fEvent;
    G4UIcmdWithAnInteger*  fVerboseCmd;
    G4UIcmdWithAnInteger*  fPmtThresholdCmd;
    G4UIcmdWithABool*      fForceDrawPhotonsCmd;
    G4UIcmdWithABool*      fForceDrawNoPhotonsCmd;
};

#endif
