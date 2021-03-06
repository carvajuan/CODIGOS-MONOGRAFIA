
/// \file OpNovice/include/OpNoviceDetectorConstruction.hh
/// \brief Definition of the OpNoviceDetectorConstruction class
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef PmtHit_h
#define PmtHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"
#include "G4VPhysicalVolume.hh"

#include "tls.hh"

class G4VTouchable;

class PmtHit : public G4VHit
{
public:

    PmtHit();
    virtual ~PmtHit();
    PmtHit(const PmtHit& right);

    const PmtHit& operator=(const PmtHit& right);
    G4bool operator==(const PmtHit& right) const;

    inline void* operator new(size_t);
    inline void operator delete(void* aHit);

    virtual void Draw();
    virtual void Print();

    inline void SetDrawit(G4bool b) { fDrawit = b; }
    inline G4bool GetDrawit() { return fDrawit; }

    inline void IncPhotonCount() { fPhotons++; }
    inline G4int GetPhotonCount() { return fPhotons; }

    inline void SetPMTNumber(G4int n) { fPmtNumber = n; }
    inline G4int GetPMTNumber() { return fPmtNumber; }

    inline void SetPMTPhysVol(G4VPhysicalVolume* physVol) { this->fPhysVol = physVol; }
    inline G4VPhysicalVolume* GetPMTPhysVol() { return fPhysVol; }

    inline void SetPMTPos(G4double x, G4double y, G4double z) {
        fPos = G4ThreeVector(x, y, z);
    }

    inline G4ThreeVector GetPMTPos() { return fPos; }

private:

    G4int fPmtNumber;
    G4int fPhotons;
    G4ThreeVector fPos;
    G4VPhysicalVolume* fPhysVol;
    G4bool fDrawit;

};

typedef G4THitsCollection<PmtHit> PmtHitsCollection;

extern G4ThreadLocal G4Allocator<PmtHit>* PmtHitAllocator;

inline void* PmtHit::operator new(size_t) {
    if (!PmtHitAllocator)
        PmtHitAllocator = new G4Allocator<PmtHit>;
    return (void*)PmtHitAllocator->MallocSingle();
}

inline void PmtHit::operator delete(void* aHit) {
    PmtHitAllocator->FreeSingle((PmtHit*)aHit);
}

#endif
