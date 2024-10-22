#ifndef TRACKING_ACTION_HH
#define TRACKING_ACTION_HH

#include <G4UserTrackingAction.hh>
#include <G4Track.hh>
#include <G4SystemOfUnits.hh>
#include <G4UnitsTable.hh>
#include <G4Gamma.hh>

class ParticleGenerator;

class TrackingAction : public G4UserTrackingAction {
    public:
        TrackingAction(ParticleGenerator*);
        ~TrackingAction() override = default;

        void  PreUserTrackingAction(const G4Track *) override;
        void PostUserTrackingAction(const G4Track *) override;

    private:
        ParticleGenerator *fParticleGenerator = nullptr;
};

#endif 
