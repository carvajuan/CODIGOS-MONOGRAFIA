
//
/// \file optical/LXe/include/LXePrimaryGeneratorAction.hh
/// \brief Definition of the LXePrimaryGeneratorAction class
//
//
#ifndef PrimarioGeneratorAction_h
#define PrimarioGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"

class G4GeneralParticleSource;
class G4ParticleGun;
class G4Event;

class PrimarioGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:

    PrimarioGeneratorAction();
    virtual ~PrimarioGeneratorAction();
 
  public:

    virtual void GeneratePrimaries(G4Event* anEvent);

  private:

    G4ParticleGun* fParticleGun;
    G4GeneralParticleSource* FuenteParticulas;
    
    int ParticulaID;
    double Ux, Uy, Uz, fx, fy, Energia, TimeDelay;
    
};

#endif
