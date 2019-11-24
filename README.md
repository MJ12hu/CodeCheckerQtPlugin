# CodeCheckerQtPlugin
A repository for the Qt Creator plugin CodeCheckerPlugin

Currently compiles with Thrift version 0.11.0 (static library, built with -fPIC)

# Compiling:

- Compiling Qt Creator
  - Get Qt Creator's source code: https://github.com/qt-creator/qt-creator
  - Compile it
- Compiling CodeCheckerPlugin
  - Open src/codecheckerplugin.pro with a pre-installed Qt Creator
  - Update the path to Qt Creator's source and build folder in the .pro file (currently ../../qt-creator and ../../qt-creator/build - if you placed Qt Creator's source and build folder there, then there's nothing to do).
  -Build the project, the .so of this project will be placed in the built Qt Creator's plugin folder, and can be tested on that Qt Creator.
