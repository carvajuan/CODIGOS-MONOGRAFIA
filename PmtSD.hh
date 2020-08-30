
/// \file OpNovice/include/OpNoviceDetectorConstruction.hh
/// \brief Definition of the OpNoviceDetectorConstruction class
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef PmtSD_h
#define PmtSD_h 1

#include "G4DataVector.hh"
#include "G4VSensitiveDetector.hh"
#include "PmtHit.hh"

#include <vector>

class G4Step;
class G4HCofThisEvent;

class PmtSD : public G4VSensitiveDetector
{

public:

    PmtSD(G4String name);
    virtual ~PmtSD();

    virtual void Initialize(G4HCofThisEvent*);
    virtual G4bool ProcessHits(G4Step* aStep, G4TouchableHistory*);

    //A version of processHits that keeps aStep constant
    G4bool ProcessHits_constStep(const G4Step*, G4TouchableHistory*);
    virtual void EndOfEvent(G4HCofThisEvent*);
    virtual void clear();
    void DrawAll();
    void PrintAll();

    //Initialize the arrays to store pmt possitions
    inline void InitPMTs() {
        if (fPMTPositionsX)delete fPMTPositionsX;
        if (fPMTPositionsY)delete fPMTPositionsY;
        if (fPMTPositionsZ)delete fPMTPositionsZ;
        fPMTPositionsX = new G4DataVector();
        fPMTPositionsY = new G4DataVector();
        fPMTPositionsZ = new G4DataVector();
    }

    //Store a pmt position
    void SetPmtPositions(const std::vector<G4ThreeVector>& positions);

private:

    PmtHitsCollection* fPMTHitCollection;

    G4DataVector* fPMTPositionsX;
    G4DataVector* fPMTPositionsY;
    G4DataVector* fPMTPositionsZ;
};

#endif
