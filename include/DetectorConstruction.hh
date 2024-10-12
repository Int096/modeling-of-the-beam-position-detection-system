#ifndef DETECTOR_CONSTRUCTION_HH
#define DETECTOR_CONSTRUCTION_HH

#include <G4VUserDetectorConstruction.hh>
#include <G4GenericMessenger.hh>

#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>

#include <G4Tubs.hh>
#include <G4EllipticalTube.hh>
#include <G4NistManager.hh>
#include <G4Box.hh>
#include <G4Material.hh>
#include <G4SystemOfUnits.hh>
#include <G4VisAttributes.hh>

class DetectorConstruction : public G4VUserDetectorConstruction 
{
    public:
        DetectorConstruction();
        ~DetectorConstruction() override;

        G4VPhysicalVolume * Construct() override;
        void ConstructSDandField() override;

        G4bool GetFCheckOverlaps() const;
        void SetFCheckOverlaps(G4bool fCheckOverlaps);

    private:
        G4bool fCheckOverlaps = true;

        void DefineMaterials();
        void DefineCommands();
        G4VPhysicalVolume *DefineVolume();
        void DefineVisAttributes();

        G4Material *vacuum          = nullptr,
                   *scintillatorMat = nullptr,
                   *aluminium       = nullptr,
                   *deuterium       = nullptr,
                   *steel           = nullptr;

        G4LogicalVolume *logicWorld    = nullptr,
                        *logicPlate    = nullptr,
                        *logicTarget   = nullptr,
                        *logicStick    = nullptr,
                        *logicDetector = nullptr,
                        *logicStickAndDetector = nullptr;

        G4double centerCoordinate = 0*cm,
                 stickRadius      = 0*cm,
                 distanceSticks   = 0*cm;

        G4GenericMessenger *fMessenger = nullptr;
};

#endif
