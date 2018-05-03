cmake_minimum_required(VERSION 3.2.3)
set(CMAKE_CXX_STANDARD 11)

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

<<<<<<< HEAD
add_executable(beerpongproject src/mainArchitecture.cpp src/Table.cpp src/Table.h src/Player.cpp src/Player.h src/Ball.cpp src/Ball.h src/Cup.cpp src/Cup.h src/Vec2.h src/Vec3.h)
#add_executable(beerpongproject main.cpp src/Table.cpp src/Table.h src/Player.cpp src/Player.h src/Ball.cpp src/Ball.h src/Cup.cpp src/Cup.h src/Vec2.h src/Vec3.h)
=======
project (beerponggame)

# Set URHO3D_HOME environment variable so that CMake can find Urho3D library
set (ENV{URHO3D_HOME} include/Urho3D/build)
# Set CMake modules search path
set (CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake/Modules)

# Include UrhoCommon.cmake module after setting project name
include (UrhoCommon)
# Define target name
set (TARGET_NAME beerpongproject_emptyscene)
# Define source files
define_source_files (EXTRA_H_FILES ${COMMON_SAMPLE_H_FILES})
# Setup target with resource copying
setup_main_executable ()
>>>>>>> origin/graphics
