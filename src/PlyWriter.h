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
    uint64_t node_id;
    uint64_t point_id;
};
struct OSMMesh{
    std::vector<OSMPoint> points;
    std::vector<std::vector<uint64_t>> faces;
};

class PlyWriter
{
public:
    PlyWriter();
    ~PlyWriter();

    void WriteASCII(const char* path, OSMMesh& osm_mesh);
private:

};

#endif