cmake_minimum_required(VERSION 3.16)

project(nimonspoli VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

find_package(SFML 2.5 COMPONENTS system window graphics REQUIRED)

add_library(
  nimonspoli_core STATIC
  src/core/Tiles.cpp 
  # etc. lanjut sendiri
)

add_library(
  nimonspoli_logic STATIC
  src/logic/Game.cpp
  # etc. lanjut sendiri
)

add_library(
  nimonspoli_data STATIC
  src/data/ConfigReader.cpp 
  # etc. lanjut sendiri
)

add_library(
  nimonspoli_ui STATIC 

  src/ui/component/Panel.cpp
  src/ui/component/Label.cpp
  src/ui/component/ImagePanel.cpp
  src/ui/component/Button.cpp
  src/ui/component/Style.cpp

  src/ui/board/TileGeometry.cpp
  src/ui/board/TilePanel.cpp
  src/ui/board/StreetTilePanel.cpp
  src/ui/board/ActionTilePanel.cpp
  src/ui/board/CornerTilePanel.cpp
  src/ui/board/BoardPanel.cpp
 
  src/ui/sidebar/ActionPanel.cpp
  src/ui/sidebar/PlayerCard.cpp
  src/ui/sidebar/PlayerInfoPanel.cpp
  src/ui/sidebar/Game_temp.cpp
  
  src/ui/GameWindow.cpp
)

add_executable(nimonspoli src/main.cpp)

target_include_directories(nimonspoli_core PUBLIC include)
target_include_directories(nimonspoli_logic PUBLIC include)
target_include_directories(nimonspoli_data PUBLIC include)
target_include_directories(nimonspoli_ui PUBLIC include)

target_link_libraries(nimonspoli_data PUBLIC nimonspoli_core)
target_link_libraries(nimonspoli_logic PUBLIC nimonspoli_core nimonspoli_data)
target_link_libraries(nimonspoli_ui PUBLIC nimonspoli_core nimonspoli_data nimonspoli_logic)
target_link_libraries(nimonspoli_ui PUBLIC sfml-system sfml-window sfml-graphics)

target_link_libraries(nimonspoli PRIVATE nimonspoli_ui)