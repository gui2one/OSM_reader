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
    uint32_t point_id;
    float lat;
    float lon;

};


struct OSMWay
{
    uint64_t id;
    std::vector<uint64_t> refs;
    std::map<const char*, const char*> tags;

    friend std::ostream& operator<<(std::ostream& output, const OSMWay& osm_way);

};

struct OSMBuilding : public OSMWay
{
    float height = 0.0f;
};
struct OSMHighway : public OSMWay
{
    uint32_t lanes = 1;
};

struct OSMBuildingRelation
{
    std::vector<uint64_t> way_refs;
};


struct OSMData
{
    bool is_empty = false;
    std::map<uint64_t, OSMNode> nodes;
    std::map<uint64_t, OSMHighway> highways;
    std::map<uint64_t, OSMBuilding> buildings;

    std::vector<OSMBuildingRelation> building_relations;
};



class OSMReader
{
public:
    std::map<uint64_t, OSMNode> m_AllNodes;
    std::map<uint64_t, OSMWay> m_AllWays;
public:
    OSMReader();
    ~OSMReader();

    OSMData Load(const char* path);

    void CollectAllWays();
    void CollectAllNodes();

    friend std::ostream& operator<<(std::ostream& output, const OSMReader& reader);
private:
    pugi::xml_document m_XMLDoc;

};


#endif