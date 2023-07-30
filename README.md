# Tic Tac Toe

Classic Tic Tac Toe game implemented in C++ and SFML2.

## Setup

Clone the repository:

```sh
git --recusrsive clone https://github.com/i9or/tictactoe.git
```

## Bootstrap Xcode project

CMake is used to generate the Xcode project. From the project root folder:

```sh
cd build
cmake -GXcode ..
open ./tictactoe.xcodeproj
```

From this point it should be possible to build, run, and debug project from the
Xcode IDE. Any changes in `CMakeLists.txt` or `src` fodler should be applied
automatically via built-in CMake re-generation. To force re-generation of the
project `ZERO_CHECK` target should be used.

## Build from command line

Steps are almost the same as for Xcode project bootstrap:

```sh
cd build
cmake ..
cmake --build .
```

If build is successful the binary should be available to run from the `bin`
folder in the root of the project:

```sh
./bin/tictactoe
```

## License

Code is distributed under the GNU AFFERO GENERAL PUBLIC LICENSE Version 3 only.

<img src="./AGPLv3_Logo.svg" width="100"/>
