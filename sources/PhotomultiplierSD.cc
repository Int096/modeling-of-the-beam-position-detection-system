#include "../include/PhotomultiplierSD.hh"

PhotomultiplierSD::PhotomultiplierSD(G4String name)
    : G4VSensitiveDetector(name)
{}

void PhotomultiplierSD::Initialize(G4HCofThisEvent*)
{}

G4bool PhotomultiplierSD::ProcessHits(G4Step *aStep, G4TouchableHistory*)
{
    //G4cout << "X = " << aStep->GetTrack()->GetPosition().getX() << G4endl;
    //G4cout << "Y = " << aStep->GetTrack()->GetPosition().getY() << G4endl;
    //G4cout << "Z = " << aStep->GetTrack()->GetPosition().getZ() << G4endl;

    G4Track *track = aStep->GetTrack();
    G4cout << "new HIT!!!" << G4endl;

    auto posX = aStep->GetPreStepPoint()->GetPosition().getX();
    auto posY = aStep->GetPreStepPoint()->GetPosition().getY();
    
    auto energy = track->GetTotalEnergy();
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->FillH2(0, posX/cm, posY/cm);

    return true;
}

void PhotomultiplierSD::EndOfEvent(G4HCofThisEvent*)
{}
