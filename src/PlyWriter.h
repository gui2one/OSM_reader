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
    bool is_road = false;
    uint32_t road__lanes = 1;

    bool is_building = false;
    float building__height = 0.0f;

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

};

#endif