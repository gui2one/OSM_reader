#ifndef PLYWRITER_H
#define PLYWRITER_H
#pragma once

#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <iostream>     // std::cout, std::fixed
#include <iomanip>      // std::setprecision

struct OSMPoint{
    float x;
    float y;
    float z;
    // uint32_t node_id;
    uint32_t point_id;
};

struct OSMFace
{
    std::vector<uint32_t> indices;
    const char * role;

    bool is_building = false;
    float building__height = 0.0f;
    float building__min_height = 0.0f;
    bool is_road = false;
    uint32_t road_type;
    uint32_t road__lanes = 1;

    bool is_multipolygon = false;
    bool is_inner = false;
    bool is_outline = false;
    
    uint32_t layer_num = 0;
    


};

struct OSMMesh{
    std::vector<OSMPoint> points;
    std::vector<OSMFace> faces;
};




class PlyWriter
{
public:
    PlyWriter();
    ~PlyWriter();

    void WriteASCII(fs::path path, OSMMesh& osm_mesh);
    void WriteBinary(fs::path path, OSMMesh& osm_mesh);
private:


    template<typename T>
    void WriteFaceAttribute(std::ofstream& out, T attrib_value)
    {
        out.write(reinterpret_cast<char*>(&attrib_value), sizeof(attrib_value));
    }
    template<>
    void WriteFaceAttribute<bool>(std::ofstream& out, bool attrib_value)
    {
        uint32_t converted = attrib_value ? 1 : 0;
        out.write(reinterpret_cast<char*>(&converted), sizeof(converted));
    }

};

#endif