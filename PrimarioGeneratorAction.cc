
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

#include<iostream>
#include<fstream>
#include<iomanip>
#include<string>

using namespace std;




//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimarioGeneratorAction::PrimarioGeneratorAction()
{

  G4int n_particle = 1;
  fParticleGun = new G4ParticleGun(n_particle);

 
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
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimarioGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
	int digit;

	ifstream archivo("file.dat", ios::in);

	
	if(!archivo)
	{
		G4int n_particle = 1;
		fParticleGun = new G4ParticleGun(n_particle);
		G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
		G4String particleName;
		fParticleGun->SetParticleDefinition(particleTable->
                                     FindParticle(particleName="mu-"));
               //Default energy,position,momentum
  		fParticleGun->SetParticleEnergy(3.*GeV);
  		fParticleGun->SetParticlePosition(G4ThreeVector(0.0, 0.0,-75.0*cm ));
  		fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
  		fParticleGun->GeneratePrimaryVertex(anEvent);
	}
	else
	{
		G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
		G4String particleName;
		
								while(archivo>>ParticulaID>>digit>>Ux>>Uy>>Uz>>fx>>fy>>TimeDelay>>Energia)
		{
			if(ParticulaID==1)
			{
				particleName="gamma";
			}
			else if(ParticulaID==2)
			{
				particleName="e+";
			}
			else if(ParticulaID==3)
			{
				particleName="e-";
			}
			else if(ParticulaID==5)
			{
				particleName="mu+";
			}
			else if(ParticulaID==6)
			{
				particleName="mu-";
			}
			else
			continue;
			
			
			G4ThreeVector direction(Ux*GeV, Uy*GeV, Uz*GeV);
			G4ThreeVector position(fx*cm, fy*cm,-600.0*cm);
			fParticleGun->SetParticleDefinition(particleTable->
                                     FindParticle(particleName));
                       fParticleGun->SetParticleEnergy(Energia*GeV);
  		fParticleGun->SetParticlePosition(position);
  		fParticleGun->SetParticleMomentumDirection(direction);
  		fParticleGun->SetParticleTime(TimeDelay*ns);
  		fParticleGun->GeneratePrimaryVertex(anEvent);
			
		
		}
	
	}
	
	archivo.close();
	
  	
}
