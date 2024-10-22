#include "../include/PhysicsList.hh"

PhysicsList::PhysicsList()
{
    RegisterPhysics(new G4EmPenelopePhysics());

    auto opticalPhysics = new G4OpticalPhysics();
    RegisterPhysics(opticalPhysics);
};

