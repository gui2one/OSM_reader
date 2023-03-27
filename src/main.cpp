#include <iostream>
#include <vector>
#include <map>
#include "pugixml.hpp"
#include "PlyWriter.h"

struct OSMNode
{
    uint32_t node_id;
    uint32_t point_id;
    float lat;
    float lon;

};


struct OSMWay
{
    uint32_t id;
    std::vector<uint32_t> refs;
};

struct OSMData
{
    std::map<uint32_t, OSMNode>  nodes;
    std::vector<OSMWay> ways;
};

std::ostream& operator<<(std::ostream& os, const OSMNode& self)
{
    os << "OSM Node id "  << self.node_id << std::endl;
    return os;
}

std::ostream& operator<<(std::ostream& os, const OSMWay& way)
{
    os << "OSM Way id "  << way.id << std::endl;
    os << "Num Refs -> "  << way.refs.size() << std::endl;
    return os;
}

int main(int argc, char** argv)
{
    std::cout << "OSM Reader" << std::endl;
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("C:/gui2one/3D/houdini_19_playground/geo/OSM_data/rennes_01.osm");
    if (!result)
        return -1;
        
    OSMData osm_data;
    std::map<uint32_t, OSMNode> osm_nodes;
    std::vector<OSMWay> osm_ways;

    pugi::xpath_node_set nodes_set = doc.select_nodes("/osm/node");

    uint32_t pt_inc = 0;
    for (pugi::xpath_node node: nodes_set)
    {
        pugi::xml_node _node = node.node();

        OSMNode osm_node;
        uint32_t node_id =  _node.attribute("id").as_uint();
        osm_node.node_id = node_id;
        osm_node.point_id = pt_inc;
        osm_node.lat = _node.attribute("lat").as_float(); //.as_float();
        osm_node.lon = _node.attribute("lon").as_float();

        osm_nodes[node_id] = osm_node;

        pt_inc++;
    }    

    osm_data.nodes = osm_nodes;


    pugi::xpath_node_set ways_set = doc.select_nodes("/osm/way");

    for (pugi::xpath_node way: ways_set)
    {
        pugi::xml_node _way = way.node();

        OSMWay osm_way;
        osm_way.id = _way.attribute("id").as_uint();
        pugi::xpath_node_set nd_set = _way.select_nodes("nd");

        for(pugi::xpath_node nd_xpath : nd_set)
        {
            auto nd_node = nd_xpath.node();
            auto node_ref = nd_node.attribute("ref").as_uint();
            osm_way.refs.push_back(node_ref);
        }

        osm_ways.push_back(osm_way);
    }    

    osm_data.ways = osm_ways;
    

    OSMMesh mesh;

    for(auto& point : osm_data.nodes)
    {
        mesh.points.push_back({point.second.lon, 0.0, point.second.lat, point.second.node_id, point.second.point_id});
    }

    for(const auto& way : osm_data.ways)
    {
        for(const auto& ref_node : way.refs)
        {
            std::cout << osm_data.nodes[ref_node] << std::endl;
            
        }
    }

    PlyWriter ply_writer;
    ply_writer.WriteASCII("hello.ply", mesh);
    
    return 0;
}