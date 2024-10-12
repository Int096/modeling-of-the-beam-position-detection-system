#ifndef PHYSICS_LIST_HH
#define PHYSICS_LIST_HH

#include <G4VModularPhysicsList.hh>

#include <G4EmStandardPhysics.hh>

class PhysicsList : public G4VModularPhysicsList 
{
    public:
        PhysicsList();
        ~PhysicsList() override = default;    
};

#endif