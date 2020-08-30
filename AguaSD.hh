
/// \file OpNovice/include/OpNoviceDetectorConstruction.hh
/// \brief Definition of the OpNoviceDetectorConstruction class
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef AguaSD_h
#define AguaSD_h 1

#include "AguaHit.hh"

#include "G4VSensitiveDetector.hh"

class G4Step;
class G4HCofThisEvent;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class AguaSD : public G4VSensitiveDetector
{
public:

    AguaSD(G4String name);
    virtual ~AguaSD();

    virtual void Initialize(G4HCofThisEvent*);
    virtual G4bool ProcessHits(G4Step* aStep, G4TouchableHistory*);
    virtual void EndOfEvent(G4HCofThisEvent*);
    virtual void clear();
    virtual void DrawAll();
    virtual void PrintAll();

private:

    AguaHitsCollection* fAguaCollection;

};

#endif
