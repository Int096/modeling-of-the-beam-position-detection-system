#include "../include/DetectorConstruction.hh"

DetectorConstruction::DetectorConstruction()
{
    centerCoordinate = 0.*cm;
    distanceSticks   = 6.*cm;
    stickRadius      = 3.*mm;

    DefineCommands();
}

DetectorConstruction::~DetectorConstruction()
{
    delete fMessenger;
}

G4VPhysicalVolume *DetectorConstruction::Construct()
{
    DefineMaterials();
    return DefineVolume();
}

void DetectorConstruction::ConstructSDandField(){};

G4VPhysicalVolume *DetectorConstruction::DefineVolume()
{
    //-------------------------------------------------------------//
    //                      Описание мира
    //-------------------------------------------------------------//
    constexpr G4double worldHX = .3*m,
                       worldHY = .3*m,
                       worldHZ = 1.*m;
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
                       plateHZ = 2.*cm;

    auto solidPlate = new G4Box("solidPlate", plateHX, plateHY,plateHZ);
    logicPlate = new G4LogicalVolume(solidPlate, steel, "logicPlate");
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
    
    auto solidTarget = new G4EllipticalTube("solidTarget", targetHX, targetHY, targetHZ);
    logicTarget = new G4LogicalVolume(solidTarget, deuterium, "logicTarget");
    new G4PVPlacement(nullptr,
                      G4ThreeVector(0., 0., -0.5*m),
                      logicTarget,
                      "Target",
                      logicWorld,
                      false,
                      0,
                      fCheckOverlaps);

    //-------------------------------------------------------------//
    //                  Описание Палочек и детекторов 
    //-------------------------------------------------------------//
    constexpr G4double stickHZ = 5.*cm,
                       stickR  = 5.*cm,
                       detectorHZ = 2.5*mm;

    auto rotate = new G4RotationMatrix();
    rotate->rotateX(90*deg);

    auto solidStickAndDetector = new G4Tubs("solidStickAndDetector",
                                              0,
                                              stickR,
                                              stickHZ + 2*detectorHZ,
                                              0,
                                              CLHEP::pi);
    auto solidDetector = new G4Tubs("solidDetector", 0, stickR, detectorHZ, 0, 2*CLHEP::pi);
    auto solidStick = new G4Tubs("solidStick", 0, stickR, stickHZ, 0, 2*CLHEP::pi);
        
    logicStickAndDetector = new G4LogicalVolume(solidStickAndDetector, 
                                                     vacuum, "logicStickAndDetector");
    logicDetector = new G4LogicalVolume(solidDetector, steel, "logicDetector");
    logicStick = new G4LogicalVolume(solidStick, scintillatorMat, "logicStick");
   
    // Right Stick and Detector
    new G4PVPlacement(nullptr,
                      G4ThreeVector(),
                      logicStick,
                      "StickRight",
                      logicStickAndDetector,
                      false,
                      0, 
                      fCheckOverlaps);
    new G4PVPlacement(nullptr,
                      G4ThreeVector(0., 0., stickHZ+detectorHZ),
                      logicDetector,
                      "DetectorTopRight",
                      logicStickAndDetector,
                      false,
                      0,
                      fCheckOverlaps);
    new G4PVPlacement(nullptr,
                      G4ThreeVector(0., 0., -stickHZ-detectorHZ),
                      logicDetector,
                      "DetectorBottomRight",
                      logicStickAndDetector,
                      false,
                      0,
                      fCheckOverlaps);
    new G4PVPlacement(rotate,
                      G4ThreeVector(centerCoordinate - distanceSticks, 0., 0.5*m),
                      logicStickAndDetector,
                      "StickAndDetectorRight",
                      logicWorld,
                      false,
                      0,
                      fCheckOverlaps);

    // Left Detector and Stick
    new G4PVPlacement(nullptr,
                      G4ThreeVector(),
                      logicStick,
                      "StickLeft",
                      logicStickAndDetector,
                      false,
                      0, 
                      fCheckOverlaps);
    new G4PVPlacement(nullptr,
                      G4ThreeVector(0., 0., stickHZ+detectorHZ),
                      logicDetector,
                      "DetectorTopLeft",
                      logicStickAndDetector,
                      false,
                      0,
                      fCheckOverlaps);
    new G4PVPlacement(nullptr,
                      G4ThreeVector(0., 0., -stickHZ-detectorHZ),
                      logicDetector,
                      "DetectorBottomLeft",
                      logicStickAndDetector,
                      false,
                      0,
                      fCheckOverlaps);
    new G4PVPlacement(rotate,
                      G4ThreeVector(centerCoordinate + distanceSticks, 0., 0.5*m),
                      logicStickAndDetector,
                      "StickAndDetectorLeft",
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

    deuterium = new G4Material("matD", 0.00018 * g/cm3, 1);
    deuterium->AddElement(elD, 1);
   
    scintillatorMat = nist->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
    G4cout << G4endl << "The material defined are : " << G4endl << G4endl;
    G4cout << *(G4Material::GetMaterialTable()) << G4endl;
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
    logicStickAndDetector->SetVisAttributes(invisible);
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


