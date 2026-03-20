cmake_minimum_required(VERSION 3.16)

project(skiresort
        VERSION 1.0.0
        DESCRIPTION "Modular ski resort simulation written in C"
        LANGUAGES C
)


# ---- Global build settings --------------------------------------------------

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# ---- Project options --------------------------------------------------------

option(SKIRESORT_ENABLE_WARNINGS "Enable strict compiler warnings" ON)
option(SKIRESORT_ENABLE_SANITIZERS "Enable address/undefined sanitizers in Debug builds" ON)
option(SKIRESORT_BUILD_TESTS "Build test targets" ON)


# ---- Interface targets: warnings / sanitizers -------------------------------

add_library(skiresort_warnings INTERFACE)
add_library(skiresort_sanitizers INTERFACE)

if(SKIRESORT_ENABLE_WARNINGS)
    if(CMAKE_C_COMPILER_ID MATCHES "Clang|GNU")
        target_compile_options(skiresort_warnings INTERFACE
                -Wall
                -Wextra
                -Wpedantic
                -Wshadow
                -Wconversion
                -Werror
        )
    elseif(MSVC)
        target_compile_options(skiresort_warnings INTERFACE
                /W4
                /WX
        )
    endif()
endif()

if(SKIRESORT_ENABLE_SANITIZERS)
    if(CMAKE_C_COMPILER_ID MATCHES "Clang|GNU")
        target_compile_options(skiresort_sanitizers INTERFACE
                $<$<CONFIG:Debug>:-fsanitize=address,undefined -g>
        )
        target_link_options(skiresort_sanitizers INTERFACE
                $<$<CONFIG:Debug>:-fsanitize=address,undefined>
        )
    endif()
endif()


# ---- Library ----------------------------------------------------------------

set(SKIRESORT_SOURCES
        src/bistro.c
        src/bus.c
        src/bus_stop.c
        src/car.c
        src/car_park.c
        src/clock.c
        src/gondola.c
        src/hotel.c
        src/input.c
        src/lift.c
        src/lift_queue.c
        src/list.c
        src/node.c
        src/person.c
        src/probability.c
        src/queue.c
        src/resort.c
        src/simulation.c
        src/slope.c
        src/station.c
)


add_library(skiresort ${SKIRESORT_SOURCES})

target_include_directories(skiresort
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_definitions(skiresort
        PUBLIC
        _POSIX_C_SOURCE=199309L # for usage of nanosleep() (clock.h)
)

target_link_libraries(skiresort
        PUBLIC
        m
        PRIVATE
        skiresort_warnings
        skiresort_sanitizers
)


# ---- Executable -------------------------------------------------------------

add_executable(skiresort_app
        app/main.c
)

target_link_libraries(skiresort_app
        PRIVATE
        skiresort
        skiresort_warnings
        skiresort_sanitizers
)


# ---- Tests ------------------------------------------------------------------

include(CTest)

if(BUILD_TESTING AND SKIRESORT_BUILD_TESTS)
    add_subdirectory(tests)
endif()