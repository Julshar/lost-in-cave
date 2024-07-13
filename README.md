# A simple 2D game project

The purpose of this project is to practise and learn tools and techniques that are useful for game and game engines programming.

Tech stack:
- Cmake
- C++
- OpenGL

The code consists of two main parts. The engine part includes functionalities which are more generic and can be applied to different projects such as UI creation and sprite rendering. It is located in "src/engine" directory. Further it is divided into main engine code and third party libraries which are present in form of static libraries or header files. The second main part is the game code which is located in "src" directory. It demonstrates functionalities provided with engine code. These functionalities include:
- Sprite rendering
- Animation rendering
- Keyboard and mouse input handling
- UI subsystem
- Text rendering
- Simple physics
- Shader loading
- Texture loading
- Camera abstraction
- Pseudorandom number generator
- Time measuring

The game itself is fairly simple. Objective it to gather as many gems as possible while dodging enemies. One of three difficulty levels can be chosen. Higher difficulty means more enemies, higher enemy speed, less lives and more gems to collect.

# How to run?

Project can be built on Windows machine with CMake and MSVC compiler installed. Once these prerequisites are met just run one of building scripts from the src directory (batch files). Depending on which script was chosen the project shall be built, built and ran immediately after or built in release configuration. Regardless of the chosen script the output will be generated in the "build" directory.


