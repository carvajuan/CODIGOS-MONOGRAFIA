

/// \file optical/LXe/src/LXePMTSD.cc
/// \brief Implementation of the LXePMTSD class
//
//
#include "PmtSD.hh"
#include "PmtHit.hh"
#include "ConstruccionDetector.hh"
#include "UsuarioTrackInformation.hh"

#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"
#include "G4ios.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleDefinition.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PmtSD::PmtSD(G4String name)
    : G4VSensitiveDetector(name), fPMTHitCollection(nullptr),
    fPMTPositionsX(nullptr), fPMTPositionsY(nullptr), fPMTPositionsZ(nullptr)
{
    collectionName.insert("pmtHitCollection");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PmtSD::~PmtSD() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PmtSD::SetPmtPositions(const std::vector<G4ThreeVector>& positions)
{
    G4int i = 0;

        if (fPMTPositionsX)fPMTPositionsX->push_back(positions[i].x());
        if (fPMTPositionsY)fPMTPositionsY->push_back(positions[i].y());
        if (fPMTPositionsZ)fPMTPositionsZ->push_back(positions[i].z());
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PmtSD::Initialize(G4HCofThisEvent* hitsCE) {
    fPMTHitCollection = new PmtHitsCollection(SensitiveDetectorName, collectionName[0]);
    //Store collection with event and keep ID
    static G4int hitCID = -1;
    if (hitCID < 0) {
        hitCID = GetCollectionID(0);
    }
    hitsCE->AddHitsCollection(hitCID, fPMTHitCollection);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool PmtSD::ProcessHits(G4Step*, G4TouchableHistory*) {
    return false;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//Generates a hit and uses the postStepPoint's mother volume replica number
//PostStepPoint because the hit is generated manually when the photon is
//absorbed by the photocathode

G4bool PmtSD::ProcessHits_constStep(const G4Step* aStep,
    G4TouchableHistory*) {

    //need to know if this is an optical photon
    if (aStep->GetTrack()->GetDefinition()
        != G4OpticalPhoton::OpticalPhotonDefinition()) return false;

    //User replica number 1 since photocathode is a daughter volume
    //to the pmt which was replicated
    G4int pmtNumber =
        aStep->GetPostStepPoint()->GetTouchable()->GetReplicaNumber(1);
    G4VPhysicalVolume* physVol =
        aStep->GetPostStepPoint()->GetTouchable()->GetVolume(1);

    //Find the correct hit collection
    G4int n = fPMTHitCollection->entries();
    PmtHit* hit = nullptr;
    for (G4int i = 0; i < n; i++) {
        if ((*fPMTHitCollection)[i]->GetPMTNumber() == pmtNumber) {
            hit = (*fPMTHitCollection)[i];
            break;
        }
    }

    if (hit == nullptr) {//this pmt wasnt previously hit in this event
        hit = new PmtHit(); //so create new hit
        hit->SetPMTNumber(pmtNumber);
        hit->SetPMTPhysVol(physVol);
        fPMTHitCollection->insert(hit);
        hit->SetPMTPos((*fPMTPositionsX)[pmtNumber], (*fPMTPositionsY)[pmtNumber],
            (*fPMTPositionsZ)[pmtNumber]);
    }

    hit->IncPhotonCount(); //increment hit for the selected pmt

    
    hit->SetDrawit(true);
    

    return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PmtSD::EndOfEvent(G4HCofThisEvent*) {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PmtSD::clear() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PmtSD::DrawAll() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PmtSD::PrintAll() {}
