#include "../include/TrackingAction.hh"

TrackingAction::TrackingAction(ParticleGenerator *pGen)
    : fParticleGenerator(pGen)
{}

void TrackingAction::PreUserTrackingAction(const G4Track *aTrack)
{
}

void TrackingAction::PostUserTrackingAction(const G4Track *aTrack)
{
}
