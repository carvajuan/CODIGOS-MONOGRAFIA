
/// \file LXeActionInitialization.hh
/// \brief Definition of the LXeActionInitialization class

#ifndef ActionInitialization_h
#define ActionInitialization_h 1

#include "G4VUserActionInitialization.hh"

class ConstruccionDetector;

/// Action initialization class.
///

class ActionInitialization : public G4VUserActionInitialization
{
  public:
    ActionInitialization(const ConstruccionDetector* det);
    virtual ~ActionInitialization();

    virtual void BuildForMaster() const;
    virtual void Build() const;

  private:

    const ConstruccionDetector* fDetector;
};

#endif
