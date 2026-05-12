cmake_minimum_required(VERSION 3.16)
project(T3_Logic_Core C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_library(t3_logic_core src/t3_logic_core.c)
target_include_directories(t3_logic_core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

option(T3_LOGIC_CORE_BUILD_TESTS "Build host tests" ON)

if(T3_LOGIC_CORE_BUILD_TESTS)
    include(CTest)
    add_executable(host_smoke tests/host_smoke.c)
    target_link_libraries(host_smoke PRIVATE t3_logic_core)
    add_test(NAME host_smoke COMMAND host_smoke)
endif()
