#ifndef OSMDATA_H
#define OSMDATA_H
#pragma once

#include <spdlog/fmt/ostr.h>
#include "Utils.h"
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

    bool is_building = false;
    bool is_road = false;

    float building_height = 1.0f;

    friend std::ostream& operator<<(std::ostream& output, const OSMWay& osm_way);

    bool HasTag(const char* tag_key);
};

enum class OSMRelationMemberType{
    Way,
    Node,
    Relation
};

struct OSMRelationMember
{
    OSMRelationMemberType type;
    uint64_t ref_id;
    const char* role;
};

class OSMRelation
{
public:
    OSMRelation();
    ~OSMRelation();
    uint64_t id;
    std::vector<OSMRelationMember> members;

    TagsMap tags;

    friend std::ostream& operator<<(std::ostream& output, const OSMRelation& relation);
    
    bool HasTag(const char* tag_key) const;

    bool IsBuildingType() const;
    bool IsMultipolygonType() const;

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


class OSMData
{
public:
    bool is_empty = false;
    std::map<uint64_t, OSMNode> nodes;
    std::map<uint64_t, OSMWay> ways;
    std::map<uint64_t, OSMRelation> relations;
    

    
    friend std::ostream& operator<<(std::ostream& output, const OSMData& osm_data);
    // std::map<uint64_t, OSMBuilding> buildings;

    // std::vector<OSMBuildingRelation> building_relations;
public:
    OSMData();
    ~OSMData();

};



#endif