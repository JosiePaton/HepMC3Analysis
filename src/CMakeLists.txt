add_executable(analysis analysis.cc)
add_executable(sbndFlux_kinVar sbndFlux_kinVar.cc)
target_link_libraries(analysis PUBLIC HepMC3::HepMC3 ROOT::Core ROOT::Hist)
target_link_libraries(sbndFlux_kinVar PUBLIC HepMC3::HepMC3 ROOT::Core ROOT::Hist)

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
