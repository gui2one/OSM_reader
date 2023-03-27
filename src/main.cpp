#include <iostream>
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

struct OSMData
{
    std::map<uint64_t, OSMNode>  nodes;
    std::vector<OSMWay> ways;
};

std::ostream& operator<<(std::ostream& os, const OSMNode& self)
{
    os << "OSM Node id "  << self.node_id << std::endl;
    os << "OSM Point id "  << self.point_id << std::endl;
    os << "-------------------------------"  << std::endl;
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
    // pugi::xml_parse_result result = doc.load_file("C:/gui2one/3D/houdini_19_playground/geo/OSM_data/rennes_01.osm");
    pugi::xml_parse_result result = doc.load_file("C:/gui2one/3D/houdini_19_playground/geo/OSM_data/manhatan_03.osm");
    // pugi::xml_parse_result result = doc.load_file("C:/gui2one/3D/houdini_19_playground/geo/OSM_data/test.osm");
    if (!result)
        return -1;
        
    OSMData osm_data;
    std::map<uint64_t, OSMNode> osm_nodes;
    std::vector<OSMWay> osm_ways;

    auto all_xml_nodes = doc.child("osm").children();
    uint64_t inc = 0;
    for( const auto& child : all_xml_nodes)
    {
        if(strcmp(child.name(), "node") == 0)
        {

            OSMNode osm_node;
            uint64_t node_id =  (uint64_t)child.attribute("id").as_ullong();
            osm_node.node_id = node_id;
            osm_node.point_id = (uint64_t)inc;
            osm_node.lat = child.attribute("lat").as_float(); //.as_float();
            osm_node.lon = child.attribute("lon").as_float();
            
            osm_nodes[node_id] = osm_node;

            inc++;   
        }
        
    }

    // return 0;

    // pugi::xpath_node_set nodes_set = doc.select_nodes("/osm/node");

    // nodes_set.sort(false);

    // for (size_t i = 0; i< nodes_set.size(); i++)
    // {
    //     auto& node = nodes_set[i];
    //     pugi::xml_node _node = node.node();

    //     OSMNode osm_node;
    //     uint64_t node_id =  (uint64_t)_node.attribute("id").as_ullong();
    //     osm_node.node_id = node_id;
    //     osm_node.point_id = (uint64_t)i;
    //     osm_node.lat = _node.attribute("lat").as_float(); //.as_float();
    //     osm_node.lon = _node.attribute("lon").as_float();

    //     osm_nodes[node_id] = osm_node;

    // }    

    osm_data.nodes = osm_nodes;


    pugi::xpath_node_set ways_set = doc.select_nodes("/osm/way");
    ways_set.sort(false);
    for (pugi::xpath_node way: ways_set)
    {
        pugi::xml_node _way = way.node();

        /* check if <tag k="highway" .... exists */
        pugi::xpath_node_set tags_set = _way.select_nodes("tag");
        bool is_highway = false;
        bool is_building = false;
        for(const auto& tag : tags_set){
            auto& tag_node = tag.node();
            const char* tag_key = tag_node.attribute("k").as_string();
            const char* tag_value = tag_node.attribute("v").value();
            // std::cout << tag_key << std::endl;
            
            if(strcmp(tag_key, "highway") == 0)
            {
                is_highway = true;
                break;
            }
            if(strcmp(tag_key, "building") == 0 && strcmp(tag_value, "yes") == 0 )
            {
                is_building = true;
                break;
            }

        }

        /* ---------------- */

        if( is_highway || is_building)
        {
            OSMWay osm_way;
            osm_way.id = _way.attribute("id").as_ullong();
            pugi::xpath_node_set nd_set = _way.select_nodes("nd");
            nd_set.sort(false);
            for(pugi::xpath_node nd_xpath : nd_set)
            {
                auto nd_node = nd_xpath.node();
                auto node_ref = nd_node.attribute("ref").as_llong();
                osm_way.refs.push_back(node_ref);
            }

            osm_ways.push_back(osm_way);

        }
    }    

    osm_data.ways = osm_ways;
    

    OSMMesh mesh;

    for(auto& point : osm_data.nodes)
    {
        mesh.points.push_back({point.second.lon, 0.0, point.second.lat, point.second.node_id, point.second.point_id});
    }

    for(const auto& way : osm_data.ways)
    {
        std::vector<uint64_t> indices;
        for(auto ref_node : way.refs)
        {
            // std::cout << ref_node << std::endl;
            
            try{    
                auto node = osm_data.nodes[ref_node];
                indices.push_back(node.point_id);
                // std::cout << node << std::endl;
            }catch(const std::exception& e){
                std::cout << e.what();
            }
        }
        if(indices.size() > 1)
        {

            mesh.faces.push_back(indices);
        }
    }

    PlyWriter ply_writer;
    ply_writer.WriteASCII("hello.ply", mesh);
    
    std::cout << "num faces : "<< mesh.faces.size() << std::endl;
    
    return 0;
}