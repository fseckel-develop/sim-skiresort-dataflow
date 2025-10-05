cmake_minimum_required(VERSION 3.29)
project(SkiResort)

set(CMAKE_C_FLAGS "-std=c90 -pedantic -Wall -Wextra -Werror -fsanitize=address")

add_executable(SkiResort main.c
        source/Bistro.c
        source/Bus.c
        source/BusStop.c
        source/Car.c
        source/CarPark.c
        source/Clock.c
        source/Gondola.c
        source/Hotel.c
        source/Input.c
        source/Lift.c
        source/LiftQueue.c
        source/List.c
        source/Node.c
        source/Person.c
        source/Probability.c
        source/Queue.c
        source/Resort.c
        source/Simulation.c
        source/Slope.c
        source/Station.c
)

target_include_directories(SkiResort PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)
