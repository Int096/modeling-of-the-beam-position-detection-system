#-------------------------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.16...3.30)
project(Simulation_beam_coordinates_control_system)

option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" OFF)
if(WITH_GEANT4_UIVIS)
    find_package(Geant4 REQUIRED ui_all vis_all)
else()
    find_package(Geant4 REQUIRED)
endif()

include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB sources ${PROJECT_SOURCE_DIR}/sources/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

add_executable(simulation simulation.cc ${sources} ${headers})
target_link_libraries(simulation ${Geant4_LIBRARIES})
add_custom_target(Simulation_beam_coordinates_control_system DEPENDS simulation)

set(SCRIPTS
    vis.mac
    init_vis.mac
    runTest.mac
)

foreach(_script ${SCRIPTS})
    configure_file(
        ${PROJECT_SOURCE_DIR}/${_script}
        ${PROJECT_BINARY_DIR}/${_script}
        COPYNLY
    )
endforeach()

