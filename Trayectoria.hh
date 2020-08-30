
/// \file optical/LXe/include/LXeTrajectory.hh
/// \brief Definition of the LXeTrajectory class
//
#ifndef Trayectoria_h
#define Trayectoria_h 1

#include "G4Trajectory.hh"
#include "G4Allocator.hh"
#include "G4ios.hh"
#include "globals.hh"
#include "G4ParticleDefinition.hh"
#include "G4TrajectoryPoint.hh"
#include "G4Track.hh"
#include "G4Step.hh"

class G4Polyline;                   // Forward declaration.

class Trayectoria : public G4Trajectory
{
  public:

    Trayectoria();
    Trayectoria(const G4Track* aTrack);
    Trayectoria(Trayectoria &);
    virtual ~Trayectoria();
 
    virtual void DrawTrajectory() const;
 
    inline void* operator new(size_t);
    inline void  operator delete(void*);

    void SetDrawTrajectory(G4bool b){fDrawit=b;}
    //void WLS(){fWls=true;}
    void SetForceDrawTrajectory(G4bool b){fForceDraw=b;}
    void SetForceNoDrawTrajectory(G4bool b){fForceNoDraw=b;}

  private:

    //G4bool fWls;
    G4bool fDrawit;
    G4bool fForceNoDraw;
    G4bool fForceDraw;
    G4ParticleDefinition* fParticleDefinition;
};

extern G4ThreadLocal G4Allocator<Trayectoria>* TrayectoriaAllocator;

inline void* Trayectoria::operator new(size_t)
{
  if(!TrayectoriaAllocator)
      TrayectoriaAllocator = new G4Allocator<Trayectoria>;
  return (void*)TrayectoriaAllocator->MallocSingle();
}

inline void Trayectoria::operator delete(void* aTrajectory)
{
  TrayectoriaAllocator->FreeSingle((Trayectoria*)aTrajectory);
}

#endif
