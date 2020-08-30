
/// \file Simulacion/src/ConstruccionDetector.cc
/// \brief Implementation of the ConstruccionDetector class
//
//
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// Make this appear first!
#include "AguaSD.hh"
#include "AguaHit.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4ParticleDefinition.hh"
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"
#include "G4ios.hh"
#include "G4VProcess.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

AguaSD::AguaSD(G4String name)
    : G4VSensitiveDetector(name)
{
    fAguaCollection = nullptr;
    collectionName.insert("AguaCollection");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

AguaSD::~AguaSD() 
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void AguaSD::Initialize(G4HCofThisEvent* hitsCE) 
{

    fAguaCollection = new AguaHitsCollection
                          (SensitiveDetectorName, collectionName[0]);

    //A way to keep all the hits of this event in one place if needed

    static G4int hitsCID = -1;
    if (hitsCID < 0) {
        hitsCID = GetCollectionID(0);
    }
    hitsCE->AddHitsCollection(hitsCID, fAguaCollection);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool AguaSD::ProcessHits(G4Step* aStep, G4TouchableHistory*) 
{

    G4double edep = aStep->GetTotalEnergyDeposit();
    if (edep == 0.) return false; //No edep so dont count as hit

    G4StepPoint* thePrePoint = aStep->GetPreStepPoint();
    G4TouchableHistory* theTouchable =
        (G4TouchableHistory*)(aStep->GetPreStepPoint()->GetTouchable());
    G4VPhysicalVolume* thePrePV = theTouchable->GetVolume();

    G4StepPoint* thePostPoint = aStep->GetPostStepPoint();

    //Get the average position of the hit
    G4ThreeVector pos = thePrePoint->GetPosition() + thePostPoint->GetPosition();
    pos /= 2.;

    AguaHit* aguaHit = new AguaHit(thePrePV);

    aguaHit->SetEdep(edep);
    aguaHit->SetPos(pos);

    fAguaCollection->insert(aguaHit);

    return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void AguaSD::EndOfEvent(G4HCofThisEvent*) {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void AguaSD::clear() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void AguaSD::DrawAll() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void AguaSD::PrintAll() {}

    
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
