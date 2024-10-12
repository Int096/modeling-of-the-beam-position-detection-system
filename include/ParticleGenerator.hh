#ifndef PARTICLE_GENERATOR_HH
#define PARTICLE_GENERATOR_HH

#include <G4VUserPrimaryGeneratorAction.hh>
#include <G4ParticleGun.hh>
#include <G4SystemOfUnits.hh>
#include <G4ParticleTable.hh>
#include <G4LogicalVolumeStore.hh>
#include <G4Event.hh>
#include <G4EllipticalTube.hh>
#include <G4RandomTools.hh>

class ParticleGenerator : public G4VUserPrimaryGeneratorAction 
{
    public:
        ParticleGenerator();
        ~ParticleGenerator() override;

        void GeneratePrimaries(G4Event *anEvent) override;

    private:
        G4ParticleGun *fParticleGun = nullptr;
    
        void ParticlePositionGenerator();
        void ParticleEnergyGenerator();
};

#endif 
