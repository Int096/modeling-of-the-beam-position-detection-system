#include "../include/ActionInitialization.hh"
#include "../include/TrackingAction.hh"
#include "../include/RunAction.hh"
#include "../include/SteppingAction.hh"

void ActionInitialization::BuildForMaster() const 
{
    SetUserAction(new RunAction);
}

void ActionInitialization::Build() const 
{
    auto pGen = new ParticleGenerator();
    SetUserAction(pGen);
    SetUserAction(new TrackingAction(pGen));
    SetUserAction(new RunAction);
    SetUserAction(new SteppingAction());
}
