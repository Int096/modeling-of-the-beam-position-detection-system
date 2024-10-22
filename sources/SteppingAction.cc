#include "../include/SteppingAction.hh"

void SteppingAction::UserSteppingAction(const G4Step *aStep)
{
    GammaKnife(aStep); 
}

void SteppingAction::GammaKnife(const G4Step *aStep)
{
    G4Track *track = aStep->GetTrack(); 
    G4int depthHistory = track->GetTouchableHandle()->GetHistoryDepth();

    if (0 != depthHistory) {
        const G4ParticleDefinition *particleDef = track->GetParticleDefinition();
        
        G4String thisVolume = track->GetTouchableHandle()->GetVolume(1)->GetName();
        G4String prevVolume = track->GetTouchableHandle()->GetVolume(0)->GetName();

        if (particleDef == G4Gamma::Gamma() &&
            "World" == thisVolume &&
            "Plate" == prevVolume) 
        {
            track->SetTrackStatus(fStopAndKill);
        }   
    }
}
