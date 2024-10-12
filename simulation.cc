#include "include/ActionInitialization.hh"
#include "include/DetectorConstruction.hh"
#include "include/PhysicsList.hh"

#include <G4RunManagerFactory.hh>
#include <G4SteppingVerbose.hh>

#include <G4UIExecutive.hh>
#include <G4UImanager.hh>
#include <G4VisExecutive.hh>

#include <QBBC.hh>

int main(int argc, char **argv)
{
    // Интерактивный режим, если нет аргументов
    //
    G4UIExecutive *ui = nullptr;
    if ( argc == 1 ) { ui = new G4UIExecutive(argc, argv); }

    // Use G4SteppingVerboseWithUnits
    //
    G4int precision = 4;
    G4SteppingVerbose::UseBestUnit(precision);

    // Дергаем дефолтный runManager 
    //
    auto runManager =
        G4RunManagerFactory::CreateRunManager(G4RunManagerType::Default);

    runManager->SetUserInitialization(new PhysicsList);
    runManager->SetUserInitialization(new ActionInitialization());
    runManager->SetUserInitialization(new DetectorConstruction());

    // Конструктор Visualization manager 
    auto visManager = new G4VisExecutive;
    visManager->Initialize();

    // Получение указателя на User Interface manager
    auto UImanager = G4UImanager::GetUIpointer();

    if ( !ui ) {
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command+fileName);
    } else {
        UImanager->ApplyCommand("/control/execute init_vis.mac");
        // Стартуем интерактивный режим
        ui->SessionStart();
        delete ui;
    }

    delete visManager;
    delete runManager;

    return 0;
}
