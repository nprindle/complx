find_package(Boost COMPONENTS unit_test_framework REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
include_directories(../liblc3)

if(CMAKE_HOST_UNIX)
    add_definitions(-DBOOST_TEST_DYN_LINK=1)
endif(CMAKE_HOST_UNIX)

add_definitions(-DPLUGIN_INSTALL_PREFIX=${CMAKE_BINARY_DIR})

set(SRC_TESTS
    lc3_assemble_test.cpp
    lc3_basic_test.cpp
    lc3_parser_test.cpp
    lc3_plugin_test.cpp
    lc3_revision_2019_test.cpp
)

if(ENABLE_REPLAY)
    include_directories(../replay)
    set(SRC_TESTS
        ${SRC_TESTS}
        lc3_replay_test.cpp
    )
endif(ENABLE_REPLAY)

add_executable(
    lc3_test_suite
    ${SRC_TESTS}
)

target_link_libraries(
    lc3_test_suite
    lc3
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
)

if(ENABLE_REPLAY)
    target_link_libraries(
        lc3_test_suite
        lc3_replay
    )
endif(ENABLE_REPLAY)

add_dependencies(
    lc3_test_suite
    lc3_multiply
    lc3_udiv
)

add_custom_command(
    TARGET lc3_test_suite POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/plugins/instructions/liblc3_multiply${CMAKE_SHARED_LIBRARY_SUFFIX} ${CMAKE_BINARY_DIR}/liblc3_multiply${CMAKE_SHARED_LIBRARY_SUFFIX}
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/plugins/traps/liblc3_udiv${CMAKE_SHARED_LIBRARY_SUFFIX} ${CMAKE_BINARY_DIR}/liblc3_udiv${CMAKE_SHARED_LIBRARY_SUFFIX}
    COMMENT "Copying needed plugins for testing")

set_target_properties(lc3_test_suite PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_test(NAME lc3_test_suite
         WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
         COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/lc3_test_suite --log_level=message)

if(ENABLE_COVERAGE)
    set(CMAKE_SKIP_RPATH FALSE)
    set(CMAKE_INSTALL_RPATH
        ${CMAKE_BINARY_DIR}
        ${CMAKE_BINARY_DIR}/plugins/devices
        ${CMAKE_BINARY_DIR}/plugins/instructions
        ${CMAKE_BINARY_DIR}/plugins/traps
        ${CMAKE_BINARY_DIR}/plugins/other)

    set(COVERAGE_EXCLUDES 'boost/*' 'c++/*' 'tests/*')

    setup_target_for_coverage(
        NAME "lc3_test_coverage"
        EXECUTABLE lc3_test_suite -j ${PROCESSOR_COUNT}
        DEPENDENCIES lc3_test_suite)
endif(ENABLE_COVERAGE)
