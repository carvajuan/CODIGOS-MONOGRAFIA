
/// \file OpNovice/include/OpNoviceDetectorConstruction.hh
/// \brief Definition of the OpNoviceDetectorConstruction class
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef AguaHit_h
#define AguaHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"
#include "G4VPhysicalVolume.hh"

#include "tls.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class AguaHit : public G4VHit
{
  public:

    AguaHit();
    AguaHit(G4VPhysicalVolume* pVol);
    virtual ~AguaHit();
    AguaHit(const AguaHit& right);


    // operators
    const AguaHit& operator=(const AguaHit& right);
    G4bool operator==(const AguaHit& right) const;

    inline void* operator new(size_t);
    inline void  operator delete(void *aHit);

    virtual void Draw();
    virtual void Print();

    inline void SetEdep(G4double de) { fEdep = de; }
    inline void AddEdep(G4double de) { fEdep += de; }
    inline G4double GetEdep() { return fEdep; }

    inline void SetPos(G4ThreeVector xyz) { fPos = xyz; }
    inline G4ThreeVector GetPos() { return fPos; }

    inline const G4VPhysicalVolume* GetPhysV() { return fPhysVol; }

private:
    G4double fEdep;
    G4ThreeVector fPos;
    const G4VPhysicalVolume* fPhysVol;



};


typedef G4THitsCollection<AguaHit> AguaHitsCollection;

extern G4ThreadLocal G4Allocator<AguaHit>* AguaHitAllocator;


inline void* AguaHit::operator new(size_t)
{
    if (!AguaHitAllocator)
        AguaHitAllocator = new G4Allocator<AguaHit>;
    return (void*)AguaHitAllocator->MallocSingle();
}


inline void AguaHit::operator delete(void* aHit)
{
    AguaHitAllocator->FreeSingle((AguaHit*) aHit);
}


#endif /*AguaHit_h*/
