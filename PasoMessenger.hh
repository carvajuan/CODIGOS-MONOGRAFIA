
//
/// \file optical/LXe/include/LXeSteppingMessenger.hh
/// \brief Definition of the LXeSteppingMessenger class
//
//
#ifndef PasoMessenger_h
#define PasoMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class PasoAction;
class G4UIcmdWithABool;

class PasoMessenger: public G4UImessenger
{
  public:
    PasoMessenger(PasoAction*);
    virtual ~PasoMessenger();
 
    virtual void SetNewValue(G4UIcommand*, G4String);

  private:

    PasoAction*        fStepping;
    G4UIcmdWithABool*  fOneStepPrimariesCmd;
 
};

#endif
