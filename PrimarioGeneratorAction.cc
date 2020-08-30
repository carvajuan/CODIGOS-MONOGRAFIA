
/// \file optical/LXe/src/LXePrimaryGeneratorAction.cc
/// \brief Implementation of the LXePrimaryGeneratorAction class
//
//
#include "PrimarioGeneratorAction.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4GeneralParticleSource.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimarioGeneratorAction::PrimarioGeneratorAction()
{

  G4int n_particle = 1;
  fParticleGun = new G4ParticleGun(n_particle);

  FuenteParticulas = new G4GeneralParticleSource();
 
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
 
  G4String particleName;
  fParticleGun->SetParticleDefinition(particleTable->
                                     FindParticle(particleName="mu-"));
  //Default energy,position,momentum
  fParticleGun->SetParticleEnergy(3.*GeV);
  fParticleGun->SetParticlePosition(G4ThreeVector(0.0 , -75.0*cm,0.0 ));
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,1,0.));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimarioGeneratorAction::~PrimarioGeneratorAction(){
    delete fParticleGun;
    delete FuenteParticulas;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimarioGeneratorAction::GeneratePrimaries(G4Event* anEvent){
  fParticleGun->GeneratePrimaryVertex(anEvent);
  FuenteParticulas->GeneratePrimaryVertex(anEvent);
}
