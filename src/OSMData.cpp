#include "OSMData.h"


std::ostream& operator<<(std::ostream& os, const OSMNode& node)
{
    os << "OSM Node "  << node.node_id ;
    os << " --point_id -> "  << node.point_id << std::endl;

    return os;
}

std::ostream& operator<<(std::ostream& os, const OSMWay& way)
{
    os << "OSM Way "  << way.id << std::endl;
    os << "\t- Num Refs : "  << way.refs.size() << std::endl;
    os << "\t- Tags : " << std::endl;

    for(const auto&[key, value] : way.tags)
    {
        os << "\t\t" << key << " : " << value << std::endl; 
    }

    os << "---------------------------"; 
    return os;
}

std::ostream& operator<<(std::ostream& os, const OSMRelation& relation)
{
    os << "OSM Relation "  << relation.id << std::endl;
    os << "\t- Members : "  << relation.members.size() << std::endl;
    os << "\t- Tags : "  << std::endl;
    for(const auto&[key, value] : relation.tags)
    {
        os << "\t\t" << key << " : " << value << std::endl; 
    }
    os << "---------------------------" << std::endl;
    return os;
}



/*
    std::ostream STUFF
*/

std::ostream& operator<<(std::ostream& os, const OSMData& data)
{
    os << "OSM Data ->" << std::endl;
    os << "\tNum Nodes : "  << data.nodes.size() << std::endl;
    os << "\tNum Ways  : "  << data.ways.size() << std::endl;
    os << "\tNum Relations  : "  << data.relations.size() << std::endl;
    return os;
}


OSMData::OSMData()
{

}

OSMData::~OSMData()
{

}