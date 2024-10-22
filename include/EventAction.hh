#ifndef EVENT_ACTION_HH
#define EVENT_ACTION_HH

#include <G4UserEventAction.hh>

class EventAction : public G4UserEventAction
{
    public: 
        EventAction() = default;
        ~EventAction() override = default;

        void BeginOfEventAction(const G4Event *anEvent) override;
        void EndOfEventAction(const G4Event *anEvent) override;

    protected:
        G4EventManager *fpEventManager;
};

#endif
