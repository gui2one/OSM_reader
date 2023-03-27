cmake_minimum_required(VERSION 3.6)

project(osm_reader)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/PlyWriter.cpp
)

include_directories(
    vendor/pugixml/src/
)

# pugixml
add_subdirectory(vendor/pugixml)
target_include_directories(${PROJECT_NAME} PUBLIC ${PUGIXML_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} pugixml ${PUGIXML_LIBRAIRIES})
