cmake_minimum_required(VERSION 3.20)
project(neat)

set(CMAKE_CXX_STANDARD 20)

add_executable(neat src/main.cpp src/neat/NetworkGenome.cpp src/neat/NetworkGenome.h src/neat/Population.cpp src/neat/Population.h src/graphics/Graphics.cpp src/graphics/Graphics.h src/utils/FastNetwork.cpp src/utils/FastNetwork.h src/neat/Gene.cpp src/neat/Gene.h src/utils/GraphNetwork.cpp src/utils/GraphNetwork.h src/neat/Species.cpp src/neat/Species.h src/simulation/Creature.cpp src/simulation/Creature.h src/simulation/Point.cpp src/simulation/Point.h src/simulation/Vector2D.cpp src/simulation/Vector2D.h src/simulation/Stick.cpp src/simulation/Stick.h)
target_link_libraries(neat sfml-graphics sfml-window sfml-system pthread)
