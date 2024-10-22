#ifndef PHYSICS_LIST_HH
#define PHYSICS_LIST_HH

#include <G4VModularPhysicsList.hh>

#include <G4EmPenelopePhysics.hh>
#include <G4OpticalPhysics.hh>
#include <G4SystemOfUnits.hh>

class PhysicsList : public G4VModularPhysicsList 
{
    public:
        PhysicsList();
        ~PhysicsList() override = default;    

};

#endif
