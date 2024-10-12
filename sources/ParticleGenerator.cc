#include "../include/ParticleGenerator.hh"
#include "CLHEP/Units/SystemOfUnits.h"
#include "G4Types.hh"
#include <cmath>

ParticleGenerator::ParticleGenerator()
{
    G4int nOfParticles = 1;
    G4String nameOfParticles = "gamma";

    fParticleGun = new G4ParticleGun(nOfParticles);

    auto particleDefinition = G4ParticleTable::GetParticleTable()->FindParticle(nameOfParticles);
    fParticleGun->SetParticleDefinition(particleDefinition);
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.));
    fParticleGun->SetParticleEnergy(800.*MeV);
}

ParticleGenerator::~ParticleGenerator()
{
    delete fParticleGun;
}

void ParticleGenerator::GeneratePrimaries(G4Event *anEvent)
{
    ParticlePositionGenerator();
    fParticleGun->GeneratePrimaryVertex(anEvent);
}

void ParticleGenerator::ParticlePositionGenerator()
{
    G4ThreeVector startParticlePosition(0., 0., 0.);

    auto TargetLV = G4LogicalVolumeStore::GetInstance()->GetVolume("logicTarget");
    auto WorldLV  = G4LogicalVolumeStore::GetInstance()->GetVolume("logicWorld");

    G4ThreeVector targetPosition;
    if ( WorldLV )
        targetPosition = WorldLV->GetDaughter(1)->GetTranslation();

    G4EllipticalTube *TargetSolid = nullptr;
    if ( TargetLV )
        TargetSolid = dynamic_cast<G4EllipticalTube*>(TargetLV->GetSolid());

    G4ThreeVector targetSize;
    if ( TargetSolid )
        targetSize.set(TargetSolid->GetDx(), TargetSolid->GetDy(), TargetSolid->GetDz());

    //TODO 
        // Написать обработку ошибок
    //Генерация продольного распределения точки генерации первичной вершини
    //по треугольному распределению методом браковки Неймана
    //
    G4double a = targetPosition.z() - targetSize.z(),
             b = targetPosition.z() + targetSize.z(),
             p = 1;

    auto function = [=](G4double x) -> G4double
    {
        G4double c = (std::fabs(b) - std::fabs(a)) / 2;
        return (x < c)
               ? (x - a) * p / (c - a)
               : (x - b) * p / (c - b);
    };

    G4double ksi, eta;
    do {
        ksi = (b - a) * G4UniformRand() + a;
        eta = p * G4UniformRand();
    } while (eta > function(ksi));

    //Поперечное распределение генерируется методом Бокса-Мюллера
    // 
    G4double phi = G4UniformRand(),
             r   = G4UniformRand();
    G4double x = std::sin(2*CLHEP::pi*phi) * std::sqrt(-2 * std::log(r));
    G4double y = std::sin(2*CLHEP::pi*phi) * std::sqrt(-2 * std::log(r));
    
    startParticlePosition.set(x, y, ksi);

    fParticleGun->SetParticlePosition(startParticlePosition);
}

