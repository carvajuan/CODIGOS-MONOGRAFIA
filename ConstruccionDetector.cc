
/// \file Simulacion/src/ConstruccionDetector.cc
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// Make this appear first!
#include "ConstruccionDetector.hh"
#include "PmtSD.hh"
#include "AguaSD.hh"


#include "G4SDManager.hh"
#include "G4RunManager.hh"

#include "G4GeometryManager.hh"
#include "G4SolidStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"

#include "G4OpticalSurface.hh"
#include "G4MaterialTable.hh"
#include "G4VisAttributes.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "globals.hh"
#include "G4UImanager.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ConstruccionDetector::ConstruccionDetector()
    : G4VUserDetectorConstruction()
{
    Mundo_box = nullptr;
    Mundo_log = nullptr;
    Mundo_phys = nullptr;

    Agua_tanque = nullptr;
    Tanque = nullptr;
    fPMT_Tubo = nullptr;
    fPMT_SD = nullptr;

    Agua_tanque_log = nullptr;
    Tanque_log = nullptr;
    fPMT_Tubo_log = nullptr;
    fPMT_SD_log = nullptr;


    fAgua = fAluminio = fAire = fVacuum = fGlass = nullptr;

    fN = fO = fH = fC = nullptr;

    SetDefaults();

    DefineMaterials();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ConstruccionDetector::~ConstruccionDetector() 
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


void ConstruccionDetector::DefineMaterials()
{
    // ------------- Materials -------------

    G4double a, z, density;
    G4int nelements;

    // ELEMENTOS BASICOS
    //
    fN = new G4Element("Nitrogen", "N", z = 7, a = 14.01 * g / mole);
    fO = new G4Element("Oxygen", "O", z = 8, a = 16.00 * g / mole);
    fH = new G4Element("Hydrogen", "H", z = 1, a = 1.01 * g / mole);
    fC = new G4Element("Carbon", "C", z = 6, a = 12.01 * g / mole);
    
    // Air
    //

    fAire = new G4Material("Aire", density = 1.29 * mg / cm3, nelements = 2);
    fAire->AddElement(fN, 70. * perCent);
    fAire->AddElement(fO, 30. * perCent);

    // Water
    //


    fAgua = new G4Material("Agua", density = 1.0 * g / cm3, nelements = 2);
    fAgua->AddElement(fH, 2);
    fAgua->AddElement(fO, 1);

    // Aluminio
    //
    fAluminio = new G4Material("Al", z = 13., a = 26.98 * g / mole, density = 2.7 * g / cm3);


    // Vidrio
    //
    fGlass = new G4Material("Vidrio", density = 1.032 * g / cm3, 2);
    fGlass->AddElement(fC, 91.533 * perCent);
    fGlass->AddElement(fH, 8.467 * perCent);

    fVacuum = new G4Material("Vacuum", z = 1., a = 1.01 * g / mole,
        density = universe_mean_density, kStateGas, 0.1 * kelvin,
        1.e-19 * pascal);


    //
    // ------------ Generate & Add Material Properties Table ------------
    //

    G4double vacuum_Energy[] = { 2.0 * eV,7.0 * eV,7.14 * eV };
    const G4int vacnum = sizeof(vacuum_Energy) / sizeof(G4double);
    G4double vacuum_RIND[] = { 1.,1.,1. };
    assert(sizeof(vacuum_RIND) == sizeof(vacuum_Energy));
    G4MaterialPropertiesTable* vacuum_mt = new G4MaterialPropertiesTable();
    vacuum_mt->AddProperty("RINDEX", vacuum_Energy, vacuum_RIND, vacnum);
    fVacuum->SetMaterialPropertiesTable(vacuum_mt);



    G4double photonEnergy[] =
    { 2.034 * eV, 2.068 * eV, 2.103 * eV, 2.139 * eV,
      2.177 * eV, 2.216 * eV, 2.256 * eV, 2.298 * eV,
      2.341 * eV, 2.386 * eV, 2.433 * eV, 2.481 * eV,
      2.532 * eV, 2.585 * eV, 2.640 * eV, 2.697 * eV,
      2.757 * eV, 2.820 * eV, 2.885 * eV, 2.954 * eV,
      3.026 * eV, 3.102 * eV, 3.181 * eV, 3.265 * eV,
      3.353 * eV, 3.446 * eV, 3.545 * eV, 3.649 * eV,
      3.760 * eV, 3.877 * eV, 4.002 * eV, 4.136 * eV };

    const G4int nEntries = sizeof(photonEnergy) / sizeof(G4double);

    //
    // Water
    //
    G4double refractiveIndex1[] =
    { 1.3435, 1.344,  1.3445, 1.345,  1.3455,
      1.346,  1.3465, 1.347,  1.3475, 1.348,
      1.3485, 1.3492, 1.35,   1.3505, 1.351,
      1.3518, 1.3522, 1.3530, 1.3535, 1.354,
      1.3545, 1.355,  1.3555, 1.356,  1.3568,
      1.3572, 1.358,  1.3585, 1.359,  1.3595,
      1.36,   1.3608 };

    assert(sizeof(refractiveIndex1) == sizeof(photonEnergy));

    G4double absorption[] =
    { 3.448 * m,  4.082 * m,  6.329 * m,  9.174 * m, 12.346 * m, 13.889 * m,
    15.152 * m, 17.241 * m, 18.868 * m, 20.000 * m, 26.316 * m, 35.714 * m,
    45.455 * m, 47.619 * m, 52.632 * m, 52.632 * m, 55.556 * m, 52.632 * m,
    52.632 * m, 47.619 * m, 45.455 * m, 41.667 * m, 37.037 * m, 33.333 * m,
    30.000 * m, 28.500 * m, 27.000 * m, 24.500 * m, 22.000 * m, 19.500 * m,
    17.500 * m, 14.500 * m };

    assert(sizeof(absorption) == sizeof(photonEnergy));

    G4double scintilFast[] =
    { 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
      1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
      1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
      1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
      1.00, 1.00, 1.00, 1.00 };

    assert(sizeof(scintilFast) == sizeof(photonEnergy));

    G4double scintilSlow[] =
    { 0.01, 1.00, 2.00, 3.00, 4.00, 5.00, 6.00,
      7.00, 8.00, 9.00, 8.00, 7.00, 6.00, 4.00,
      3.00, 2.00, 1.00, 0.01, 1.00, 2.00, 3.00,
      4.00, 5.00, 6.00, 7.00, 8.00, 9.00, 8.00,
      7.00, 6.00, 5.00, 4.00 };

    assert(sizeof(scintilSlow) == sizeof(photonEnergy));

    G4MaterialPropertiesTable* myMPT1 = new G4MaterialPropertiesTable();

    myMPT1->AddProperty("RINDEX", photonEnergy, refractiveIndex1, nEntries)
        ->SetSpline(true);
    myMPT1->AddProperty("ABSLENGTH", photonEnergy, absorption, nEntries)
        ->SetSpline(true);
    myMPT1->AddProperty("FASTCOMPONENT", photonEnergy, scintilFast, nEntries)
        ->SetSpline(true);
    myMPT1->AddProperty("SLOWCOMPONENT", photonEnergy, scintilSlow, nEntries)
        ->SetSpline(true);

    myMPT1->AddConstProperty("SCINTILLATIONYIELD", 50. / MeV);
    myMPT1->AddConstProperty("RESOLUTIONSCALE", 1.0);
    myMPT1->AddConstProperty("FASTTIMECONSTANT", 1. * ns);
    myMPT1->AddConstProperty("SLOWTIMECONSTANT", 10. * ns);
    myMPT1->AddConstProperty("YIELDRATIO", 0.8);

    G4double energy_water[] = {
       1.56962 * eV, 1.58974 * eV, 1.61039 * eV, 1.63157 * eV,
       1.65333 * eV, 1.67567 * eV, 1.69863 * eV, 1.72222 * eV,
       1.74647 * eV, 1.77142 * eV, 1.7971 * eV, 1.82352 * eV,
       1.85074 * eV, 1.87878 * eV, 1.90769 * eV, 1.93749 * eV,
       1.96825 * eV, 1.99999 * eV, 2.03278 * eV, 2.06666 * eV,
       2.10169 * eV, 2.13793 * eV, 2.17543 * eV, 2.21428 * eV,
       2.25454 * eV, 2.29629 * eV, 2.33962 * eV, 2.38461 * eV,
       2.43137 * eV, 2.47999 * eV, 2.53061 * eV, 2.58333 * eV,
       2.63829 * eV, 2.69565 * eV, 2.75555 * eV, 2.81817 * eV,
       2.88371 * eV, 2.95237 * eV, 3.02438 * eV, 3.09999 * eV,
       3.17948 * eV, 3.26315 * eV, 3.35134 * eV, 3.44444 * eV,
       3.54285 * eV, 3.64705 * eV, 3.75757 * eV, 3.87499 * eV,
       3.99999 * eV, 4.13332 * eV, 4.27585 * eV, 4.42856 * eV,
       4.59258 * eV, 4.76922 * eV, 4.95999 * eV, 5.16665 * eV,
       5.39129 * eV, 5.63635 * eV, 5.90475 * eV, 6.19998 * eV
    };

    const G4int numentries_water = sizeof(energy_water) / sizeof(G4double);

    //assume 100 times larger than the rayleigh scattering for now.
    
    G4double mie_water[] = {
       1670.244 * m, 1587.267 * m, 1507.42 * m,
       1430.625 * m, 1356.802 * m, 1285.874 * m,
       1217.763 * m, 1152.395 * m, 1089.695 * m,
       1029.588 * m, 972.0035 * m, 916.8686 * m,
       864.1133 * m, 813.6679 * m, 765.4642 * m,
       719.4346 * m, 675.5129 * m, 633.6336 * m,
       593.7325 * m, 555.7461 * m, 519.6124 * m,
       485.2700 * m, 452.6587 * m, 421.7194 * m,
       392.3939 * m, 364.6250 * m, 338.3568 * m,
       313.5341 * m, 290.1030 * m, 268.0103 * m,
       247.2042 * m, 227.6336 * m, 209.2488 * m,
       192.0007 * m, 175.8416 * m, 160.7245 * m,
       146.6038 * m, 133.4346 * m, 121.1733 * m,
       109.7770 * m, 99.20416 * m, 89.41407 * m,
       80.36711 * m, 72.02470 * m, 64.34927 * m,
       57.30429 * m, 50.85425 * m, 44.96467 * m,
       39.60210 * m, 34.73413 * m, 30.32937 * m,
       26.35746 * m, 22.78907 * m, 19.59588 * m,
       16.75064 * m, 14.22710 * m, 12.00004 * m,
       10.04528 * m, 8.339666 * m, 6.861063 * m
    };

    assert(sizeof(mie_water) == sizeof(energy_water));

    // gforward, gbackward, forward backward ratio
    G4double mie_water_const[3] = { 0.99,0.99,0.8 };

    //myMPT1->AddProperty("MIEHG", energy_water, mie_water, numentries_water)
      //  ->SetSpline(true);

    //myMPT1->AddConstProperty("MIEHG_FORWARD", mie_water_const[0]);
    //myMPT1->AddConstProperty("MIEHG_BACKWARD", mie_water_const[1]);
    //myMPT1->AddConstProperty("MIEHG_FORWARD_RATIO", mie_water_const[2]);

    G4cout << "LA TABLA DE PROPIEDADES DEL AGUA ES LA SIGUIENTE" << G4endl;
    myMPT1->DumpTable();

    fAgua->SetMaterialPropertiesTable(myMPT1);

    // Set the Birks Constant for the Water scintillator

    //
    // Air
    //
    G4double refractiveIndex2[] =
    { 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
      1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
      1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
      1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
      1.00, 1.00, 1.00, 1.00 };

    G4MaterialPropertiesTable* myMPT2 = new G4MaterialPropertiesTable();
    myMPT2->AddProperty("RINDEX", photonEnergy, refractiveIndex2, nEntries);

    G4cout << "LA TABLA DE PROPIEDADES DEL AIRE ES LA SIGUIENTE" << G4endl;
    myMPT2->DumpTable();

    fAire->SetMaterialPropertiesTable(myMPT2);


    //
    // PROPIEDADES VIDRIO
    //


    G4double glass_RIND[] =
    { 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49,
     1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49,
     1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49,
     1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49,

    };

    G4double glass_AbsLength[] =
    { 420. * cm,420. * cm,420. * cm, 420. * cm,420. * cm,420. * cm,420. * cm,420. * cm,420. * cm,420. * cm,420. * cm,420. * cm,420. * cm,420. * cm,420. * cm,
      420. * cm,420. * cm,420. * cm, 420. * cm,420. * cm,420. * cm,420. * cm,420. * cm,420. * cm,420. * cm,420. * cm,420. * cm,420. * cm,420. * cm,420. * cm,
      420. * cm,420. * cm,420. * cm, 420. * cm,420. * cm,420. * cm,420. * cm,420. * cm,420. * cm,420. * cm,420. * cm,420. * cm,420. * cm,420. * cm,420. * cm,
      420. * cm,420. * cm,420. * cm, 420. * cm,420. * cm,420. * cm,420. * cm,420. * cm,420. * cm,420. * cm,420. * cm,420. * cm,420. * cm,420. * cm,420. * cm,
    };


    assert(sizeof(glass_RIND) == sizeof(energy_water));

    assert(sizeof(glass_AbsLength) == sizeof(energy_water));
    G4MaterialPropertiesTable* glass_mt = new G4MaterialPropertiesTable();
    glass_mt->AddProperty("ABSLENGTH", energy_water, glass_AbsLength, numentries_water);
    glass_mt->AddProperty("RINDEX", energy_water, glass_RIND, numentries_water);
    fGlass->SetMaterialPropertiesTable(glass_mt);


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* ConstruccionDetector::Construct()
{
    G4double Mundo_x = fAltura_Agua +  1000. * cm;
    G4double Mundo_y = fAltura_Agua +  1000. * cm;
    G4double Mundo_z = fAltura_Agua +  1000. * cm;
    // The experimental Hall
    //
    Mundo_box = new G4Box("World", Mundo_x, Mundo_y, Mundo_z);

    Mundo_log = new G4LogicalVolume(Mundo_box, fVacuum, "World", 0, 0, 0);

    Mundo_phys = new G4PVPlacement(0, G4ThreeVector(), Mundo_log, "World", 0, false, 0);

    G4VisAttributes* MUNDO_va = new G4VisAttributes(G4Colour(1., 0., 0.));
    Mundo_log->SetVisAttributes(MUNDO_va);

    //********************************AGUA Y TANQUE

    G4double startAngle = 0. * deg;
    G4double spanningAngle = 360. * deg;

    G4double fRadio_Tanque = fRadio_Agua + 2. * fD_mtl;
    G4double fAltura_Tanque = fAltura_Agua + 2. * fD_mtl;

    Agua_tanque = new G4Tubs("AguaTanque", fRadio_Interno, fRadio_Agua, fAltura_Agua / 2., startAngle, spanningAngle);

    Tanque = new G4Tubs("Tanque", fRadio_Interno, fRadio_Tanque, fAltura_Tanque / 2., startAngle, spanningAngle);

    Agua_tanque_log = new G4LogicalVolume(Agua_tanque, fAgua, "AguaTanque_log", 0, 0, 0);

    Tanque_log = new G4LogicalVolume(Tanque, fAluminio, "Tanque", 0, 0, 0);

    Tanque_phys = new G4PVPlacement(0, G4ThreeVector(), Tanque_log, "Tanque_physical", Mundo_log, false, 0);

    Agua_phys = new G4PVPlacement(0, G4ThreeVector(), Agua_tanque_log, "Agua_physical", Tanque_log, false, 0);


    //********************************CONSTRUIR EL PMT

    G4double Altura_PMT = fD_mtl / 2.;

    fPMT_Tubo = new G4Tubs("PMT_tubo", fRadio_Interno, fRadio_PMT, Altura_PMT, startAngle, spanningAngle);


    fPMT_SD = new G4Tubs("pmt_SD", fRadio_Interno, fRadio_PMT, Altura_PMT / 2, startAngle, spanningAngle);


    fPMT_Tubo_log = new G4LogicalVolume(fPMT_Tubo, fGlass, "PMT_Tubo");

    fPMT_SD_log = new G4LogicalVolume(fPMT_SD, fAluminio, "photocath_log");


    new G4PVPlacement(0, G4ThreeVector(0, 0, -Altura_PMT / 2), fPMT_SD_log, "photocath", fPMT_Tubo_log, false, 0);

    G4double x, y, z;
    z = fAltura_Agua / 2. + Altura_PMT;
    G4int k = 0;

    PlacePMTs(fPMT_Tubo_log, 0, x, y, z, k);

    VisAttributes();
    SurfaceProperties();



    return Mundo_phys;

    
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void ConstruccionDetector::ConstructSDandField()
{

    // Sensitive detectors
    //PMT SD

    G4cout << "CONSTRUCCION /Det/PmtSD" << G4endl;

    PmtSD* pSD = new PmtSD("SD_PMT");
    pSD->InitPMTs();
    pSD->SetPmtPositions(GetPmtPositions());

    G4SDManager::GetSDMpointer()->AddNewDetector(pSD);
    // Setting pSD to all logical volumes with the same name 
    // of "photocath_log".
    SetSensitiveDetector(GetLogPhotoCath(), pSD);

    AguaSD* aSD = new AguaSD("SD_AGUA");
    G4SDManager::GetSDMpointer()->AddNewDetector(aSD);
    // Setting aSD to all logical volumes with the same name 
    // of "AguaTanque_log".
    SetSensitiveDetector(GetLogAgua(), aSD);

}

void ConstruccionDetector::SetDefaults() 
{

    fAltura_Agua = 100.0 * cm;
    fRadio_PMT = 2.3 * cm;
    fRadio_Agua = 11.5 * cm;
    fRadio_Interno = 0.0 * cm;
    fD_mtl = 0.635 * cm;
    fRefl = 0.0;
    

}

void ConstruccionDetector::CopyValues()
{
    fAltura_Agua = GetAlturaAgua();
    fRadio_Agua = GetRadioAgua();
    fRadio_PMT = GetPMTRadius();
    fRadio_Interno = GetRadioInterno();
    fD_mtl = GetGrosor();

}

void ConstruccionDetector::PlacePMTs(G4LogicalVolume* pmt_log, G4RotationMatrix* rot, G4double& x, G4double& y, G4double& z,
    G4int& k)
{

    new G4PVPlacement(rot, G4ThreeVector(x, y, z), pmt_log, "pmt", Tanque_log, false, k);
    fPmtPositions.push_back(G4ThreeVector(x, y, z));



}

void ConstruccionDetector::VisAttributes()
{
    G4VisAttributes* housing_va = new G4VisAttributes(G4Colour(0.8, 0.8, 0.8));
    Tanque_log->SetVisAttributes(housing_va);

    G4VisAttributes* agua_va = new G4VisAttributes(G4Colour(0., 0., 1.));
    Agua_tanque_log->SetVisAttributes(agua_va);

}

void ConstruccionDetector::SurfaceProperties()
{

    // ------------- Surfaces --------------
    //
    // Tank
    //
    G4OpticalSurface* opSuperficieTanque = new G4OpticalSurface("SuperficieTanque");

    opSuperficieTanque->SetType(dielectric_LUT);
    opSuperficieTanque->SetModel(LUT);
    opSuperficieTanque->SetFinish(polishedtyvekair);

    //G4LogicalSkinSurface* SkinTanque =
      //  new G4LogicalSkinSurface("SurfaceTanque", Tanque_log, opSuperficieTanque);


    //
    // Propiedades superficie PMT
    //

    G4double ephoton[] = { 7.0 * eV, 7.14 * eV };
    const G4int num = sizeof(ephoton) / sizeof(G4double);

    G4double eluz[] = 
	{ 4.275 * eV, 4.260 * eV, 4.246 * eV, 4.231 * eV, 4.217 * eV,
	  4.202 * eV, 4.188 * eV, 4.174 * eV, 4.160 * eV, 4.146 * eV,
          4.132 * eV, 4.065 * eV, 3.999 * eV, 3.936 * eV, 3.874 * eV,
	  3.874 * eV, 3.815 * eV, 3.757 * eV, 3.701 * eV, 3.646 * eV,
	  3.593 * eV, 3.542 * eV, 3.492 * eV, 3.444 * eV, 3.396 * eV,
          3.351 * eV, 3.306 * eV, 3.262 * eV, 3.220 * eV, 3.179 * eV,
	  3.138 * eV, 3.099 * eV, 3.061 * eV, 3.024 * eV, 2.987 * eV,
	  2.952 * eV, 2.917 * eV, 2.883 * eV, 2.850 * eV, 2.817 * eV,
	  2.786 * eV, 2.755 * eV, 2.725 * eV, 2.695 * eV, 2.666 * eV,
	  2.637 * eV, 2.610 * eV, 2.583 * eV, 2.556 * eV, 2.530 * eV,
	  2.504 * eV, 2.479 * eV, 2.455 * eV, 2.431 * eV, 2.407 * eV,
	  2.384 * eV, 2.361 * eV, 2.339 * eV, 2.317 * eV, 2.296 * eV,
	  2.274 * eV, 2.254 * eV, 2.233 * eV, 2.214 * eV, 2.194 * eV,
	  2.175 * eV, 2.156 * eV, 2.137 * eV, 2.119 * eV, 2.101 * eV,
	  2.083 * eV, 2.066 * eV, 2.049 * eV, 2.032 * eV, 2.016 * eV,
	  1.999 * eV, 1.983 * eV, 1.968 * eV, 1.952 * eV, 1.937 * eV,
	  1.922 * eV, 1.907 * eV, 1.892 * eV, 1.878 * eV, 1.864 * eV,
	  1.850 * eV, 1.836 * eV, 1.823 * eV, 1.809 * eV, 1.796 * eV,
	  1.783 * eV, 1.771 * eV, 1.758 * eV, 1.746 * eV, 1.734 * eV};
    
    const G4int nmo = sizeof(eluz) / sizeof(G4double);

    G4double photocath_EFF[] = 
	{ 0.0100, 0.015, 0.0170, 0.020, 0.0280, 0.035, 0.0450, 0.058, 0.0700, 0.1000,
	  0.1075, 0.115, 0.1225, 0.130, 0.1375, 0.145, 0.1525, 0.160, 0.1675, 0.1750,
	  0.1825, 0.190, 0.1975, 0.205, 0.2125, 0.250, 0.2500, 0.250, 0.2500, 0.2350,
	  0.2300, 0.225, 0.2200, 0.215, 0.2100, 0.205, 0.2000, 0.195, 0.1900, 0.1850,
	  0.1800, 0.175, 0.1700, 0.165, 0.1600, 0.155, 0.1500, 0.145, 0.1400, 0.1400,
	  0.1300, 0.120, 0.1200, 0.115, 0.1100, 0.100, 0.0900, 0.080, 0.0700, 0.0600,
	  0.0550, 0.050, 0.04200, 0.04, 0.0370, 0.032, 0.0280, 0.025, 0.0230, 0.0200,
	  0.0180, 0.015, 0.01250, 0.01, 0.0080, 0.005, 0.0020, 0.001, 0.0010, 0.0010,
          0.0001, 0.0001, 0.0001, 0.0001, 0.0001, 0.0001, 0.0001, 0.0001, 0.0001, 		  0.0001, 0.0001, 0.0001, 0.0001, 0.0001, 0.0001}; //Enables 'detection' of photons
    assert(sizeof(photocath_EFF) == sizeof(eluz));
    G4double photocath_ReR[] = { 1.92,1.92 };
    assert(sizeof(photocath_ReR) == sizeof(ephoton));
    G4double photocath_ImR[] = { 1.69,1.69 };
    assert(sizeof(photocath_ImR) == sizeof(ephoton));
    G4MaterialPropertiesTable* photocath_mt = new G4MaterialPropertiesTable();
    photocath_mt->AddProperty("EFFICIENCY", eluz, photocath_EFF, nmo);
    photocath_mt->AddProperty("REALRINDEX", ephoton, photocath_ReR, num);
    photocath_mt->AddProperty("IMAGINARYRINDEX", ephoton, photocath_ImR, num);
    G4OpticalSurface* photocath_opsurf =
        new G4OpticalSurface("photocath_opsurf", glisur, polished, dielectric_metal);
    photocath_opsurf->SetMaterialPropertiesTable(photocath_mt);

    G4LogicalSkinSurface* SkinPMT =
        new G4LogicalSkinSurface("SurfacePMT", fPMT_SD_log, photocath_opsurf);

    

    //
    // PROPIEDADES SUPERFICIE ENTRE EL AGUA Y EL TANQUE
    //
  const G4int numero = 2;
  G4double photone[num] = { 2.034 * eV, 4.136 * eV };

  G4OpticalSurface* opWaterSurface = new G4OpticalSurface("WaterSurface");
  opWaterSurface->SetType(dielectric_metal);
  opWaterSurface->SetFinish(polished);
  opWaterSurface->SetModel(unified);
  //opWaterSurface->SetType(dielectric_LUT);
  //opWaterSurface->SetFinish(polishedtyvekair);
  //opWaterSurface->SetModel(LUT);

  G4double reflectivity[] = { fRefl, fRefl };
  G4double reflectividad[] = 
	{ fRefl, fRefl, fRefl, fRefl, fRefl,
	  fRefl, fRefl, fRefl, fRefl, fRefl,
          fRefl, fRefl, fRefl, fRefl, fRefl,
	  fRefl, fRefl, fRefl, fRefl, fRefl,
	  fRefl, fRefl, fRefl, fRefl, fRefl,
	  fRefl, fRefl, fRefl, fRefl, fRefl,
	  fRefl, fRefl, fRefl, fRefl, fRefl,
	  fRefl, fRefl, fRefl, fRefl, fRefl,
	  fRefl, fRefl, fRefl, fRefl, fRefl,
	  fRefl, fRefl, fRefl, fRefl, fRefl,
	  fRefl, fRefl, fRefl, fRefl, fRefl,
	  fRefl, fRefl, fRefl, fRefl, fRefl,
	  fRefl, fRefl, fRefl, fRefl, fRefl,
	  fRefl, fRefl, fRefl, fRefl, fRefl,
	  fRefl, fRefl, fRefl, fRefl, fRefl,
	  fRefl, fRefl, fRefl, fRefl, fRefl,
	  fRefl, fRefl, fRefl, fRefl, fRefl,
	  fRefl, fRefl, fRefl, fRefl, fRefl,
	  fRefl, fRefl, fRefl, fRefl, fRefl,};

  assert(sizeof(reflectividad) == sizeof(eluz));
  G4double efficiency[] = { 0.0, 0.0 };
  assert(sizeof(efficiency) == sizeof(ephoton));
  G4MaterialPropertiesTable* scintHsngPT = new G4MaterialPropertiesTable();
  scintHsngPT->AddProperty("REFLECTIVITY", eluz, reflectividad, nmo);
  scintHsngPT->AddProperty("EFFICIENCY", photone, efficiency, numero);
  
  opWaterSurface->SetMaterialPropertiesTable(scintHsngPT);

  new G4LogicalSkinSurface("photocath_surf", Tanque_log, opWaterSurface);

  //G4LogicalBorderSurface* waterSurface = new G4LogicalBorderSurface("WaterSurface", Tanque_phys,Agua_phys,opWaterSurface);

//G4OpticalSurface* opticalSurface = dynamic_cast <G4OpticalSurface*>
       // (waterSurface->GetSurface(Tanque_phys,Agua_phys)-> GetSurfaceProperty());
  //if (opticalSurface) opticalSurface->DumpInfo();

 //Muestra las propiedades de la superficie del tanque


}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
