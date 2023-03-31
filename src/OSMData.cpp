#include "OSMData.h"


using Utils::str_is_equal;


bool OSMWay::HasTag(const char* tag_key)
{
    for(std::map<const char*, const char*>::iterator it = tags.begin(); it != tags.end(); ++it)
    {
        if(str_is_equal(tag_key, it->first)) 
        {
            return true;
        }
    }

    return false;
}



OSMRelation::OSMRelation()
{
}

OSMRelation::~OSMRelation()
{
}

bool OSMRelation::HasTag(const char* tag_key) const
{
    for(TagsMap::const_iterator it = tags.begin(); it != tags.end(); ++it)
    {
        if(str_is_equal(tag_key, it->first)) 
        {
            return true;
        }
    }

    return false;
}
bool OSMRelation::IsBuildingType() const
{
    for(const auto& [key, value]: tags)
    {
        if( str_is_equal(key, "type") && str_is_equal(value, "building") )
        {
            return true;
        }
    }
    return false;
}

bool OSMRelation::IsMultipolygonType() const
{
    for(const auto& [key, value]: tags)
    {
        if( str_is_equal(key, "type")  && str_is_equal(value, "multipolygon") )
        {
            return true;
        }
    }
    return false;
}

OSMData::OSMData()
{

}

OSMData::~OSMData()
{

}

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

    os << "---------------------------------" << std::endl;
    return os;
}

std::ostream& operator<<(std::ostream& os, const OSMRelation& relation)
{
    os << "OSM Relation "  << relation.id << std::endl;
    os << "\t- Members : "  << relation.members.size() << std::endl;
    for(const auto& member : relation.members)
    {
        os << "\t\t- ref id : " << member.ref_id << std::endl; 
    }    
    os << "\t- Tags : "  << std::endl;
    for(const auto&[key, value] : relation.tags)
    {
        os << "\t\t" << key << " : " << value << std::endl; 
    }
    os << "---------------------------------" << std::endl;
    return os;
}



std::ostream& operator<<(std::ostream& os, const OSMData& data)
{
    os << "OSM Data ->" << std::endl;
    os << "\tNum Nodes     : "  << data.nodes.size() << std::endl;
    os << "\tNum Ways      : "  << data.ways.size() << std::endl;
    os << "\tNum Relations : "  << data.relations.size() << std::endl;
    os << "---------------------------------" << std::endl;
    return os;
}
