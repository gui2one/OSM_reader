#ifndef OSMREADER__H
#define OSMREADER__H

#pragma once

#include <vector>
#include <map>
#include "pugixml.hpp"
#include "PlyWriter.h"

struct OSMNode
{
    uint64_t node_id;
    uint64_t point_id;
    float lat;
    float lon;

};


struct OSMWay
{
    uint64_t id;
    std::vector<uint64_t> refs;
};

struct OSMBuildingRelation
{
    std::vector<uint64_t> way_refs;
};


struct OSMData
{
    std::map<uint64_t, OSMNode> nodes;
    std::map<uint64_t, OSMWay> ways;
};



class OSMReader
{
public:
    OSMReader();
    ~OSMReader();

    OSMData Load(const char* path);

private:

};

#endif