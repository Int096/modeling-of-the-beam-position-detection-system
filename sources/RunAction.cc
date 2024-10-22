#include "../include/RunAction.hh"

RunAction::RunAction()
{
    auto analysisManager = G4AnalysisManager::Instance();
    G4cout << "Using " << analysisManager->GetType() << G4endl;

    G4int id = analysisManager->CreateH2("hist", "Coord", 50, -35, 35, 50, -35, 35);
}

G4Run *RunAction::GenerateRun() 
{
    return nullptr;
}

void RunAction::BeginOfRunAction(const G4Run *aRun)
{
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->OpenFile("run.root");

    G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl; 
}

void RunAction::EndOfRunAction(const G4Run *)
{
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->Write();
    analysisManager->CloseFile();
}
