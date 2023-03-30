cmake_minimum_required(VERSION 3.6)

project(osm_reader)

set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/OSMData.cpp
    src/OSMReader.cpp
    src/PlyWriter.cpp
    src/BgeoWriter.cpp
    src/Log.cpp
)

include_directories(
    src/
    vendor/pugixml/src/
    vendor/spdlog/include
)
target_include_directories(${PROJECT_NAME} PUBLIC
    src/
    vendor/pugixml/src/
    vendor/spdlog/include 
)

target_precompile_headers(${PROJECT_NAME} PRIVATE src/pch.h)

# pugixml
add_subdirectory(vendor/pugixml)
target_include_directories(${PROJECT_NAME} PUBLIC ${PUGIXML_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} pugixml ${PUGIXML_LIBRAIRIES})
