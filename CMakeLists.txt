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

find_package(SFML 2.5 REQUIRED COMPONENTS system window graphics REQUIRED)

add_library(
  nimonspoli_core STATIC
  src/core/Tiles.cpp 
  src/core/Player.cpp
  src/core/Property.cpp
  src/core/Railroad.cpp
  src/core/Street.cpp
  src/core/Utility.cpp
  src/core/ActionCard.cpp
  src/core/ChanceCard.cpp
  src/core/CommunityChestCard.cpp
  src/core/SkillCard.cpp
  src/core/TeleportCard.cpp
  src/core/MoveCard.cpp
  src/core/ShieldCard.cpp
  src/core/DiscountCard.cpp
  src/core/LassoCard.cpp
  src/core/DemolitionCard.cpp
)

add_library(
  nimonspoli_logic STATIC
  src/logic/Bank.cpp
  src/logic/Board.cpp
  src/logic/Game.cpp
  src/logic/TransactionLogger.cpp
  src/logic/Bank.cpp
  src/logic/Auction.cpp
  src/logic/Board.cpp
  src/logic/GameLoader.cpp
  src/logic/BankruptcyHandler.cpp
  # etc. lanjut sendiri
)

add_library(
  nimonspoli_data STATIC
  src/data/ConfigReader.cpp
  src/data/SaveLoadManager.cpp
  src/data/LogSerializer.cpp
  src/data/DomainBuilder.cpp
  # etc. lanjut sendiri
)

add_library(
  nimonspoli_ui STATIC 
  src/ui/Widgets.cpp
  src/ui/GameWindow.cpp
  # etc. lanjut sendiri
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

# unit testing stuff 
# enable_testing()
#
# find_package(GTest REQUIRED)
# add_executable(logictest test/logic/AuctionTest.cpp test/logic/BankTest.cpp test/logic/GameLoopTest.cpp)
#
# target_link_libraries(logictest
#     PRIVATE 
#     nimonspoli_core 
#     nimonspoli_logic
#     GTest::gtest 
#     GTest::gtest_main 
#     pthread
# )
# add_test(NAME LogicTest COMMAND logictest)
# gtest_discover_tests(logictest)
