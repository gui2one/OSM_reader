#include <iostream>
#include <vector>
#include <map>
// #include "pugixml.hpp"
#include "PlyWriter.h"

#include "OSMReader.h"
OSMMesh OSMDataToMesh(const OSMData& osm_data)
{
    OSMMesh mesh;

    for(auto& point : osm_data.nodes)
    {
        mesh.points.push_back({point.second.lon, 0.0, point.second.lat, point.second.node_id, point.second.point_id});
    }

    for(const auto& way : osm_data.ways)
    {
        std::vector<uint64_t> indices;
        for(auto& ref_node : way.second.refs)
        {
            // std::cout << ref_node << std::endl;
            
            try{    
                auto& node = osm_data.nodes.at(ref_node);
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
    return mesh;
}

int main(int argc, char** argv)
{
    std::cout << "OSM Reader" << std::endl;

    OSMReader reader;
    // OSMData osm_data = reader.Load("C:/gui2one/3D/houdini_19_playground/geo/OSM_data/rennes_01.osm");
    OSMData osm_data = reader.Load("C:/gui2one/3D/houdini_19_playground/geo/OSM_data/manhatan_02.osm");

    OSMMesh mesh;

    for(auto& point : osm_data.nodes)
    {
        mesh.points.push_back({point.second.lon, 0.0, point.second.lat, point.second.node_id, point.second.point_id});
    }

    for(const auto& way : osm_data.ways)
    {
        std::vector<uint64_t> indices;
        for(auto& ref_node : way.second.refs)
        {
            // std::cout << ref_node << std::endl;
            
            try{    
                auto node = osm_data.nodes.at(ref_node);
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
    // ply_writer.WriteASCII("hello.ply", mesh);
    ply_writer.WriteBinary("hello_bin.ply", mesh);
    
    std::cout << "num faces : "<< mesh.faces.size() << std::endl;
    
    return 0;
}