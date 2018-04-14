### To compile this project with Urho3D dependencies
- Clone this repository and swith to `graphics` branch.
- Ensure that you have also downloaded Urho3D repository. We added it as a submodule in `include/Urho3D`. To download it, use `git submodule update --init` in the root folder of the project. This should download the latest Urho3D code in `include/Urho3D`. If this does not work, run `cd include && git submodule add https://github.com/urho3d/Urho3D.git`.
- Then, please build Urho3D. Create a folder named `cmake-build-debug` in `include/Urho3D`, and run `cmake .. && make` in `include/Urho3D/cmake-build-debug`. You may want to use `make -j4` to compile Urho3D, as the compilation can take some time.
- In this project's root directory, run `mkdir cmake-build-debug`, `cd cmake-build-debug`, `cmake ..`, `make` with your desired build options (such as `make -j4` for faster compilation). Everything should compile well.
- To test the first program, in the build directory, run `bin/beerpongproject`
