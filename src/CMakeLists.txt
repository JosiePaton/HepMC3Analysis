add_executable(analysis analysis.cc)
add_executable(sbndFlux_kinVar sbndFlux_kinVar.cc)
add_executable(prismFlux_kinVar prismFlux_kinVar.cc)
add_executable(flatFlux_kinVar flatFlux_kinVar.cc)
add_executable(analysis_fluxWeight_plots analysis_fluxWeight_plots.cc)
target_link_libraries(analysis PUBLIC HepMC3::HepMC3 ROOT::Core ROOT::Hist)
target_link_libraries(sbndFlux_kinVar PUBLIC HepMC3::HepMC3 ROOT::Core ROOT::Hist ROOT::Physics)
target_link_libraries(prismFlux_kinVar PUBLIC HepMC3::HepMC3 ROOT::Core ROOT::Hist ROOT::Physics)
target_link_libraries(flatFlux_kinVar PUBLIC HepMC3::HepMC3 ROOT::Core ROOT::Hist ROOT::Physics)
target_link_libraries(analysis_fluxWeight_plots PUBLIC HepMC3::HepMC3 ROOT::Core ROOT::Hist ROOT::Physics)

set_target_properties(analysis PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

set_target_properties(sbndFlux_kinVar PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

set_target_properties(prismFlux_kinVar PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

set_target_properties(flatFlux_kinVar PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

set_target_properties(analysis_fluxWeight_plots PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
