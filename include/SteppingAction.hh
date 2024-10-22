#ifndef STEPPING_ACTION_HH
#define STEPPING_ACTION_HH

#include <G4UserSteppingAction.hh>
#include <G4RunManager.hh>
#include <G4Gamma.hh> 

class SteppingAction : public G4UserSteppingAction 
{
    public:
        SteppingAction() = default;
        ~SteppingAction() override = default;

        void UserSteppingAction(const G4Step *) override;

    private:
        void GammaKnife(const G4Step *);
};

#endif
