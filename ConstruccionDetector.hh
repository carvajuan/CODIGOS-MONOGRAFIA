
/// \file OpNovice/include/OpNoviceDetectorConstruction.hh
/// \brief Definition of the OpNoviceDetectorConstruction class
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef ConstruccionDetector_h
#define ConstruccionDetector_h 1

class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Box;
class G4Tubs;
class MainVolume;

#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4RotationMatrix.hh"

#include "AguaSD.hh"
#include "PmtSD.hh"

#include "G4VUserDetectorConstruction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class ConstruccionDetector : public G4VUserDetectorConstruction
{
  public:
    ConstruccionDetector();
    virtual ~ConstruccionDetector();

    virtual G4VPhysicalVolume* Construct();
    virtual void ConstructSDandField();

    //Functions to modify the geometry

    void SetDefaults();

    //GET VALUES

    G4double GetAlturaAgua() const { return fAltura_Agua; }
    G4double GetRadioAgua() const { return fRadio_Agua; }
    G4double GetPMTRadius() const { return fRadio_PMT; }
    G4double GetRadioInterno() const { return fRadio_Interno; }
    G4double GetGrosor() const { return fD_mtl; }

    G4double GetReflectividad() const { return fRefl; }

    G4LogicalVolume* GetLogPhotoCath() { return fPMT_SD_log; }
    G4LogicalVolume* GetLogAgua() { return Agua_tanque_log; }

    std::vector<G4ThreeVector> GetPmtPositions() { return fPmtPositions; }


  private:


      void DefineMaterials();

      void VisAttributes();
      void SurfaceProperties();

      void PlacePMTs(G4LogicalVolume* pmt_Log,
          G4RotationMatrix* rot,
          G4double& x, G4double& y, G4double& z, G4int& k);

      void CopyValues();


      G4Box* Mundo_box;
      G4LogicalVolume* Mundo_log;
      G4VPhysicalVolume* Mundo_phys;
      G4VPhysicalVolume* Tanque_phys;
      G4VPhysicalVolume* Agua_phys;

      //Materials & Elements
      G4Material* fAgua;
      G4Material* fAluminio;
      G4Material* fVacuum;
      G4Element* fN;
      G4Element* fO;
      G4Material* fAire;
      G4Element* fC;
      G4Element* fH;
      G4Material* fGlass;


      //****GEOMETRIA

      G4double fAltura_Agua;
      G4double fRadio_PMT;
      G4double fRadio_Agua;
      G4double fRadio_Interno;
      G4double fD_mtl;
      G4double fRefl;

      //VOLUMENES BASICOS
   //
      G4Tubs* Agua_tanque;
      G4Tubs* Tanque;
      G4Tubs* fPMT_Tubo;
      G4Tubs* fPMT_SD;

      // Logical volumes
      //

      G4LogicalVolume* Agua_tanque_log;
      G4LogicalVolume* Tanque_log;
      G4LogicalVolume* fPMT_Tubo_log;
      G4LogicalVolume* fPMT_SD_log;


      // Sensitive Detectors positions
      std::vector<G4ThreeVector> fPmtPositions;


};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif /*ConstruccionDetector_h*/
