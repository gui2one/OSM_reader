#ifndef OSMREADER__H
#define OSMREADER__H

#pragma once

#include <vector>
#include <map>
#include "pugixml.hpp"
#include "PlyWriter.h"


using TagsMap = std::map<const char*, const char*>;
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

struct OSMRelation
{
    uint64_t id;
    std::vector<OSMRelationMember> members;
    std::map<const char*, const char*> tags;

    friend std::ostream& operator<<(std::ostream& output, const OSMRelation& relation);
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
    std::map<uint64_t, OSMRelation> m_AllRelations;
public:
    OSMReader();
    ~OSMReader();

    OSMData Load(const char* path);

    void CollectAllWays();
    void CollectAllNodes();
    void CollectAllRelations();

    friend std::ostream& operator<<(std::ostream& output, const OSMReader& reader);
private:
    pugi::xml_document m_XMLDoc;

};


#endif