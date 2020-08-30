
//
/// \file optical/LXe/include/LXeRunAction.hh
/// \brief Definition of the LXeRunAction class
//
//
#include "G4UserRunAction.hh"

#ifndef CorrerAction_h
#define CorrerAction_h 1

class Correr;
class HistogramaManager;
class G4Run;

class CorrerAction : public G4UserRunAction
{
  public:

    CorrerAction();
    virtual ~CorrerAction();

    virtual G4Run* GenerateRun();
    virtual void BeginOfRunAction(const G4Run*);
    virtual void EndOfRunAction(const G4Run*);

  private:

    Correr*          fRun;
    HistogramaManager* fHistoManager;
};

#endif
