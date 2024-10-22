#include "../include/DetectorConstruction.hh"

DetectorConstruction::DetectorConstruction(G4double centerCoordinate, G4double distanceSticks, G4double stickRadius)
    : centerCoordinate(centerCoordinate)
    , distanceSticks(distanceSticks)
    , stickRadius(stickRadius)
{
    // TODO вынести в класс
    DefineCommands();
}

DetectorConstruction::~DetectorConstruction()
{
    delete fMessenger;
}

G4VPhysicalVolume *DetectorConstruction::Construct()
{
    // Определяем материалы
    DefineMaterials();

    // Определяем геометрию, возвращаем мир
    return DefineVolume();
}

void DetectorConstruction::ConstructSDandField()
{
    G4SDManager *SDman = G4SDManager::GetSDMpointer();
    G4VSensitiveDetector *SD = new PhotomultiplierSD("SiPM");
    
    SetSensitiveDetector("logicDetector", SD, true);
    
    SDman->AddNewDetector(SD);
};

G4VPhysicalVolume *DetectorConstruction::DefineVolume()
{
    //-------------------------------------------------------------//
    //                      Описание мира
    //-------------------------------------------------------------//
    constexpr G4double worldHX =  .3*m,
                       worldHY =  .3*m,
                       worldHZ = 1.2*m;

    auto solidWorld = new G4Box("solidWorld", worldHX, worldHY, worldHZ);
    logicWorld = new G4LogicalVolume(solidWorld, vacuum, "logicWorld");
    auto physWorld  = new G4PVPlacement(nullptr,
                                        G4ThreeVector(),
                                        logicWorld,
                                        "World",
                                        nullptr,
                                        false,
                                        0,
                                        fCheckOverlaps);

    //-------------------------------------------------------------//
    //                  Описание Стальной пластины 
    //-------------------------------------------------------------//
    constexpr G4double plateHX = worldHX,
                       plateHY = worldHY,
                       plateHZ = .8*cm;

    auto solidPlate
        = new G4Box("solidPlate", plateHX, plateHY,plateHZ);
    logicPlate 
        = new G4LogicalVolume(solidPlate, steel, "logicPlate");
    
    new G4PVPlacement(nullptr,
                      G4ThreeVector(),
                      logicPlate,
                      "Plate",
                      logicWorld,
                      false,
                      0,
                      fCheckOverlaps);
    
    //-------------------------------------------------------------//
    //                  Описание Мишени
    //-------------------------------------------------------------//
    constexpr G4double targetHZ = 20.*mm,
                       targetHX = 14.*mm,
                       targetHY = 24.*mm,
                       targetThickness = 5.*mm * 2;

    G4double targetPositionZ = -0.5*m;

    auto solidTarget 
        = new G4EllipticalTube("solidTarget", targetHX, targetHY, targetHZ);
    logicTarget 
        = new G4LogicalVolume(solidTarget, deuterium, "logicTarget");
    
    new G4PVPlacement(nullptr,
                      G4ThreeVector(0., 0., targetPositionZ),
                      logicTarget,
                      "Target",
                      logicWorld,
                      false,
                      0,
                      fCheckOverlaps);

    //-------------------------------------------------------------//
    //                  Описание Палочек и детекторов 
    //-------------------------------------------------------------//
    constexpr G4double stickHZ    = 5. *cm,
                       detectorHZ = 2.5*mm;

    auto rotate = new G4RotationMatrix();
    rotate->rotateX(90*deg);

    auto solidDetector 
        = new G4Tubs("solidDetector", 0, stickRadius, detectorHZ, 0, 2*CLHEP::pi);
    auto solidStick    
        = new G4Tubs("solidStick", 0, stickRadius, stickHZ, 0, 2*CLHEP::pi);
        
    logicDetector 
        = new G4LogicalVolume(solidDetector, detectorMat, "logicDetector");
    logicStick    
        = new G4LogicalVolume(solidStick, scintillatorMat, "logicStick");

    G4double positionZ  = 0.5*m,
             positionLX = centerCoordinate + distanceSticks,
             positionRX = centerCoordinate - distanceSticks,
             positionTY = stickHZ + detectorHZ,
             positionBY = - stickHZ - detectorHZ;

    // Sticks
    new G4PVPlacement(rotate,
                      G4ThreeVector(positionLX, 0., positionZ),
                      logicStick,
                      "LeftStick",
                      logicWorld,
                      false,
                      0,
                      fCheckOverlaps);
    new G4PVPlacement(rotate,
                      G4ThreeVector(positionRX, 0., positionZ),
                      logicStick,
                      "RightStick",
                      logicWorld,
                      false,
                      0,
                      fCheckOverlaps);

    // Detectors
    new G4PVPlacement(rotate,
                      G4ThreeVector(positionLX, positionTY, positionZ),
                      logicDetector,
                      "LeftTopDetector",
                      logicWorld,
                      false,
                      0,
                      fCheckOverlaps);
    new G4PVPlacement(rotate,
                      G4ThreeVector(positionRX, positionTY, positionZ),
                      logicDetector,
                      "RightTopDetector",
                      logicWorld,
                      false,
                      0,
                      fCheckOverlaps);
    new G4PVPlacement(rotate,
                      G4ThreeVector(positionLX, positionBY, positionZ),
                      logicDetector,
                      "LeftBottomDetector",
                      logicWorld,
                      false,
                      0,
                      fCheckOverlaps);
    new G4PVPlacement(rotate,
                      G4ThreeVector(positionRX, positionBY, positionZ),
                      logicDetector,
                      "RightBottomDetector",
                      logicWorld,
                      false,
                      0,
                      fCheckOverlaps);

    DefineVisAttributes();
    return physWorld;
}

void DetectorConstruction::DefineMaterials()
{
    auto nist = G4NistManager::Instance();

    vacuum = nist->FindOrBuildMaterial("G4_AIR");

    steel = new G4Material("Steel", 7.999 * g/cm3, 5);
    steel->AddElement(nist->FindOrBuildElement("Mn"), 0.02);
    steel->AddElement(nist->FindOrBuildElement("Si"), 0.01);
    steel->AddElement(nist->FindOrBuildElement("Cr"), 0.19);
    steel->AddElement(nist->FindOrBuildElement("Ni"), 0.10);
    steel->AddElement(nist->FindOrBuildElement("Fe"), 0.68);

    auto D = new G4Isotope("Deutron", 1, 2, 2.0141018 * g/mole);
    auto elD = new G4Element("Deuterium", "elD", 1);
    elD->AddIsotope(D, 1);

    detectorMat = nist->FindOrBuildMaterial("G4_F");

    deuterium = new G4Material("matD", 0.00018 * g/cm3, 1);
    deuterium->AddElement(elD, 1);
   
    scintillatorMat = nist->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");

    // Optical properties
    const G4int nEntries = 2; 
    G4double PhotonEnergy[nEntries] = {1.0*eV, 7.0*eV};

    G4double scintillatorRefractionIndex[nEntries] = {1.9, 1.9};
    G4double scintillatorAbsorptionLength[nEntries] = {50.*m, 50.*m};

    auto scintMPT = new G4MaterialPropertiesTable();
    auto detMPT   = new G4MaterialPropertiesTable();

    scintMPT->AddProperty("RINDEX", PhotonEnergy, scintillatorRefractionIndex, nEntries);
    scintMPT->AddProperty("ABSLENGTH", PhotonEnergy, scintillatorAbsorptionLength, nEntries);
    detMPT->AddProperty("RINDEX", PhotonEnergy, scintillatorRefractionIndex, nEntries);
    detMPT->AddProperty("ABSLENGTH", PhotonEnergy, scintillatorAbsorptionLength, nEntries);

    G4double scintEnergy[nEntries] = {3.26*eV, 3.44*eV};
    G4double scintFast[nEntries]   = {1.0, 1.0};

    scintMPT->AddConstProperty("SCINTILLATIONYIELD", 63./keV);
    scintMPT->AddConstProperty("RESOLUTIONSCALE", 1.);

    detMPT->AddConstProperty("SCINTILLATIONYIELD", 63./keV);
    detMPT->AddConstProperty("RESOLUTIONSCALE", 1.);
    
    scintillatorMat->SetMaterialPropertiesTable(scintMPT);
    detectorMat->SetMaterialPropertiesTable(detMPT);
}

void DetectorConstruction::DefineVisAttributes()
{
    G4VisAttributes invisible(G4VisAttributes::GetInvisible());
    G4VisAttributes invisibleYellow(false, G4Colour::Yellow());
    G4VisAttributes yellow(G4Colour::Yellow());
    G4VisAttributes blue(G4Colour::Blue());
    G4VisAttributes red(G4Colour::Red());
    G4VisAttributes green(G4Colour::Green());

    logicWorld->SetVisAttributes(invisible);
    logicPlate->SetVisAttributes(yellow);
    logicStick->SetVisAttributes(green);
    logicTarget->SetVisAttributes(blue);
    logicDetector->SetVisAttributes(red);
}

void DetectorConstruction::DefineCommands()
{
    fMessenger = new G4GenericMessenger(this,
                                        "/detector/",
                                        "Detector Control");
    auto &centerCoordinateCmd = fMessenger->DeclarePropertyWithUnit("centerCoordinate",
                                                                    "cm",
                                                                    centerCoordinate,
                                                                    "Coordinate of center");
    centerCoordinateCmd.SetParameterName("coordinate", true);

    auto &distanceSticksCmd = fMessenger->DeclarePropertyWithUnit("distanceSticks",
                                                                  "cm",
                                                                  distanceSticks,
                                                                  "Distance between of stiks");
    auto &stickRadiusCmd = fMessenger->DeclarePropertyWithUnit("stickRadius",
                                                               "mm",
                                                               stickRadius,
                                                               "Radius of sticks");
}


