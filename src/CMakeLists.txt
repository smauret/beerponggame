cmake_minimum_required(VERSION 3.9)
project(BeerPongProject)

set(CMAKE_CXX_STANDARD 17)

add_executable(BeerPongProject mainArchitecture.cpp Table.cpp Table.h Player.cpp Player.h Ball.cpp Ball.h Cup.cpp Cup.h Vec2.h Vec3.h)