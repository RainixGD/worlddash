cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_OSX_ARCHITECTURES "x86_64")
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

project(worlddash VERSION 1.0.0)

file(GLOB SOURCES
	src/*.cpp
)

add_library(${PROJECT_NAME} SHARED ${SOURCES} "src/Button.cpp" "src/Button.h" "src/MyLoadingCircle.cpp" "src/MyLoadingCircle.h" "src/MapUiLayer.cpp" "src/MapUiLayer.h" "src/LevelPopup.cpp" "src/LevelPopup.h" "src/LevelObj.cpp" "src/LevelObj.h" "src/MapLayer.cpp" "src/MapLayer.h" "src/LoginLayer.cpp" "src/LoginLayer.h" "src/AccountLayer.cpp" "src/AccountLayer.h" "src/RegisterLayer.cpp" "src/RegisterLayer.h" "src/DataManager.cpp" "src/DataManager.h" "src/ReqManager.cpp" "src/ReqManager.h" "src/CreatedLevelsLayer.cpp" "src/CreatedLevelsLayer.h" "src/SaveingAlert.cpp" "src/SaveingAlert.h" "src/LoadingAlert.cpp" "src/LoadingAlert.h" "src/PublicMapLevelPopup.cpp" "src/PublicMapLevelPopup.h" "src/PublicLevelAlert.cpp" "src/PublicLevelAlert.h" "src/MyLevelTools.cpp" "src/MyLevelTools.h" "src/LikeLevelAlert.cpp" "src/LikeLevelAlert.h" "src/RateDifficultyLevelAlert.cpp" "src/RateDifficultyLevelAlert.h" "src/SelectLevelDifficultyAlert.cpp" "src/SelectLevelDifficultyAlert.h" "src/CounterNode.cpp" "src/CounterNode.h" "src/MapUpdateAlert.cpp" "src/MapUpdateAlert.h" "src/PublicLevelOptionsPopup.cpp" "src/PublicLevelOptionsPopup.h" "src/UpdateLevelAlert.cpp" "src/PayMoreForPublicLevelAlert.cpp" "src/PayMoreForPublicLevelAlert.h" "src/DailyChestPopup.cpp" "src/DailyChestPopup.h" "src/FrameAnimatedSprite.cpp" "src/FrameAnimatedSprite.h" "src/MapDecoContainer.cpp" "src/MapDecoContainer.h" "src/JumpingMonster.cpp" "src/JumpingMonster.h" "src/MyInput.cpp" "src/MyInput.h" "src/RainixPopup.cpp" "src/RainixPopup.h")

if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "Unable to find Geode SDK! Please define GEODE_SDK environment variable to point to Geode")
else()
    message(STATUS "Found Geode: $ENV{GEODE_SDK}")
endif()

add_subdirectory($ENV{GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)

setup_geode_mod(${PROJECT_NAME})
