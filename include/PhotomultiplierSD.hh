#ifndef PHOTOMULTIPLIER_SD_HH
#define PHOTOMULTIPLIER_SD_HH

#include <G4VSensitiveDetector.hh>
#include <G4Step.hh>
#include <G4HCofThisEvent.hh>
#include <G4Track.hh>
#include <G4AnalysisManager.hh>
#include <G4SystemOfUnits.hh>

class PhotomultiplierSD : public G4VSensitiveDetector 
{
    public: 
        PhotomultiplierSD(G4String name);
        ~PhotomultiplierSD() override = default;

        void Initialize(G4HCofThisEvent *) override;
        G4bool ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist) override;
        void EndOfEvent(G4HCofThisEvent *) override;
};

#endif
