#ifndef RUN_ACTION_HH
#define RUN_ACTION_HH

#include <G4UserRunAction.hh>
#include <G4AnalysisManager.hh>
#include <G4Run.hh>

class RunAction : public G4UserRunAction 
{
    public:
        RunAction();
        ~RunAction() override = default;

        G4Run *GenerateRun() override;
        void BeginOfRunAction(const G4Run *) override;
        void EndOfRunAction(const G4Run *) override;
};

#endif
