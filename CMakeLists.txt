cmake_minimum_required(VERSION 3.16)

project(Practical_02_QTTelemetry LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets)

add_library(ground_station ground_station.cpp ground_station.h)
target_link_libraries(ground_station PUBLIC Qt${QT_VERSION_MAJOR}::Core)
target_include_directories(ground_station PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_library(ground_station_ui main_window.cpp main_window.h)
target_link_libraries(ground_station_ui PUBLIC ground_station Qt${QT_VERSION_MAJOR}::Widgets)
target_include_directories(ground_station_ui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(ground_station_app main.cpp)
target_link_libraries(ground_station_app PRIVATE ground_station_ui)

include(CTest)
if(BUILD_TESTING)
    find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Test)
    add_executable(test_ground_station tests/test_ground_station.cpp)
    target_link_libraries(test_ground_station PRIVATE ground_station)
    add_test(NAME ground_station COMMAND test_ground_station)

    add_executable(test_window tests/test_window.cpp)
    target_link_libraries(test_window PRIVATE ground_station_ui Qt${QT_VERSION_MAJOR}::Test)
    add_test(NAME window COMMAND test_window)
    set_tests_properties(window PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=offscreen")
endif()
