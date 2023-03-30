#include <iostream>
#include <vector>
#include <map>
// #include "pugixml.hpp"
#include "PlyWriter.h"
#include "BgeoWriter.h"

#include "OSMReader.h"
OSMMesh OSMDataToMesh(const OSMData& osm_data)
{
    OSMMesh mesh;

    for(auto& point : osm_data.nodes)
    {
        mesh.points.push_back({point.second.lon, 0.0, point.second.lat, (uint32_t)(point.second.point_id)});
    }

    for(const auto& way : osm_data.buildings)
    {
        std::vector<uint32_t> indices;
        for(auto& ref_node : way.second.refs)
        {
            try
            {
                const OSMNode& node = osm_data.nodes.at(ref_node);
                indices.push_back(node.point_id);

            }catch(const std::exception& e){
            
                std::cout << "Problem while creating face from OSMWay Refs : " << e.what() << std::endl;
            }
        }
        
        if(indices.size() > 1)
        {
            OSMFace face;
            face.indices = indices;
            // face.is_building = way.second.is_building;
            // face.is_road = way.second.is_road;
            mesh.faces.push_back(face);
        }
    }

    return mesh;
}

int main(int argc, char** argv)
{
    std::cout << "OSM Reader" << std::endl;

    std::cout << "Loading Data ..." << std::endl;
    OSMReader reader;
    // OSMData osm_data = reader.Load("C:/gui2one/3D/houdini_19_playground/geo/OSM_data/rennes_01.osm");
    OSMData osm_data = reader.Load("C:/gui2one/3D/houdini_19_playground/geo/OSM_data/manhatan_02.osm");
    std::cout << reader;
    
    for(const auto& [way_id, way_data] : reader.m_AllWays)
    {
        std::cout << (OSMWay)way_data << std::endl;
        break;
    }

    if( !osm_data.is_empty)
    {

        std::cout << "Mesh Convertion ..." << std::endl;
        auto mesh = OSMDataToMesh(osm_data);

        PlyWriter ply_writer;
        // ply_writer.WriteASCII("hello_ascii.ply", mesh);
        ply_writer.WriteBinary("hello_bin.ply", mesh);

        std::cout << "num faces : "<< mesh.faces.size() << std::endl;

    }else{

        std::cout << "OSMData is empty" << std::endl;
        
    }
    
    
    return 0;
}