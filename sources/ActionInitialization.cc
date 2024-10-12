#include "../include/ActionInitialization.hh"

void ActionInitialization::BuildForMaster() const 
{
}

void ActionInitialization::Build() const 
{
    SetUserAction(new ParticleGenerator());
}
