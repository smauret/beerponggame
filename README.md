## Beer pong game
This project has been split into two parts. We use Urho3D to develop our game, and because of the restrictions it puts into place, we cannot effectively produce two executables.

### To test the 2D application :
Compile this project with Urho3D dependencies:
- Clone this repository and swith to `master` branch.
- Ensure that you have also downloaded Urho3D repository. We added it as a submodule in `include/Urho3D`. To download it, use `git submodule update --init` in the root folder of the project. This should download the latest Urho3D code in `include/Urho3D`. If this does not work, run `cd include && git submodule add https://github.com/urho3d/Urho3D.git`.
- Then, we need to build Urho3D to use its library. Create a folder named `build` in Uhro3D root directory `include/Urho3D`, and run `cmake .. && make` in this buld directory `include/Urho3D/build`. You may want to use `make -j4` to compile Urho3D, as the compilation can take some time.
- Copy the folder `bin/Data`, `bin/CoreData` and `CMake` from Uhro3D's folder to our project's root folder. This is needed to find Uhro3D's library correctly.
- In this project's root directory, run `mkdir cmake-build-debug`, `cd cmake-build-debug`, `cmake ..`, `make` with your desired build options (such as `make -j4` for faster compilation). Everything should compile well.
- To test the first program, in the build directory, run `bin/beerpongproject`

**Note**: watch out for Urho3D syntax URHO3D_DEFINE_APPLICATION_MAIN(). Because of the CMake utilities Urho3D provide, only one C++ file can contain a main function in the directory at the same time. Otherwise, compilation is impossible.

### To test the 3D application:
The procedure is the same as before, but on the `graphics3d` branch.
- Clone this repository and swith to `graphics3d` branch.
- If you have already compiled Urho3D, the library should be available. Otherwise, follow the same procedure as above to compile Urho3D.
- Copy the folder `bin/Data`, `bin/CoreData` and `CMake` from Uhro3D's folder to our project's root folder. This is needed to find Uhro3D's library correctly.
- In this project's root directory, run `mkdir cmake-build-debug`, `cd cmake-build-debug`, `cmake ..`, `make` with your desired build options (such as `make -j4` for faster compilation). Everything should compile well.
- To test the first program, in the build directory, run `bin/beerpongproject`
