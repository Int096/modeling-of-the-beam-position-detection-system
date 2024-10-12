#ifndef ACTION_INITIALIZATION_HH
#define ACTION_INITIALIZATION_HH

#include <G4VUserActionInitialization.hh>

#include "ParticleGenerator.hh"

class ActionInitialization : public G4VUserActionInitialization 
{
    public: 
        ActionInitialization() = default;
        ~ActionInitialization() override = default;

        void BuildForMaster() const override;
        void Build() const override;
};

#endif
