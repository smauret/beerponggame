cmake_minimum_required(VERSION 3.9)
#project(beerponggame)

set(CMAKE_CXX_STANDARD 17)



if (COMMAND cmake_policy)
    # Libraries linked via full path no longer produce linker search paths
    cmake_policy (SET CMP0003 NEW)
    # INTERFACE_LINK_LIBRARIES defines the link interface
    cmake_policy (SET CMP0022 NEW)
    # Disallow use of the LOCATION target property - so we set to OLD as we still need it
    cmake_policy (SET CMP0026 OLD)
    # MACOSX_RPATH is enabled by default
    cmake_policy (SET CMP0042 NEW)
    # Honor the visibility properties for SHARED target types only
    cmake_policy (SET CMP0063 OLD)
endif ()


set (URHO3D_HOME /Users/mac/Documents/Adv.Prog.Tech/BP/Urho3D/cmake-build-debug)
# Set project name
project (beerponggame)
# Set CMake modules search path
set (CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake/Modules)
# Include UrhoCommon.cmake module after setting project name
include (UrhoCommon)
# Define target name
set (TARGET_NAME mainNajwa.cpp)
# Define source files
define_source_files (EXTRA_H_FILES ${COMMON_SAMPLE_H_FILES})
# Setup target with resource copying
setup_main_executable ()
#add_executable(beerpongproject src/mainLucas.cpp src/BoardGame.cpp src/BoardGame.h src/Player.cpp src/Player.h src/Ball.cpp src/Ball.h src/Cup.cpp src/Cup.h src/Vec2.h src/Vec3.h mainNajwa.cpp mainNajwa.h Sample.h)


add_executable(beerpongproject mainNajwa.cpp mainNajwa.h Sample.h Sample.inl)


