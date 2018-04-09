cmake_minimum_required(VERSION 3.9)
project(beerponggame)

set(CMAKE_CXX_STANDARD 17)
# MODIF NAJWA

add_executable(beerpongproject src/mainLucas.cpp src/BoardGame.cpp src/BoardGame.h src/Player.cpp src/Player.h src/Ball.cpp src/Ball.h src/Cup.cpp src/Cup.h src/Vec2.h src/Vec3.h)
#add_executable(beerpongproject main.cpp src/BoardGame.cpp src/BoardGame.h src/Player.cpp src/Player.h src/Ball.cpp src/Ball.h src/Cup.cpp src/Cup.h src/Vec2.h src/Vec3.h)