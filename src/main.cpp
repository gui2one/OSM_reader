
#include "PlyWriter.h"
#include "BgeoWriter.h"

#include "OSMReader.h"

OSMMesh OSMDataToMesh(const OSMData& osm_data)
{
    LOG_INFO("Converting to mesh");
    OSMMesh mesh;

    for(auto& point : osm_data.nodes)
    {
        mesh.points.push_back({point.second.lon, 0.0, point.second.lat, (uint32_t)(point.second.point_id)});
    }

    // for(const auto& way : osm_data.ways)
    // {
    //     std::vector<uint32_t> indices;
    //     for(auto& ref_node : way.second.refs)
    //     {
    //         try
    //         {
    //             const OSMNode& node = osm_data.nodes.at(ref_node);
    //             indices.push_back(node.point_id);

    //         }catch(const std::exception& e){
            
    //             LOG_ERROR("Problem while creating face from OSMWay Refs : \n{}", e.what());
    //         }
    //     }

    //     if(indices.size() > 1)
    //     {
    //         OSMFace face;
    //         face.indices = indices;
    //         // face.is_building = way.second.is_building;
    //         // face.is_road = way.second.is_road;
    //         mesh.faces.push_back(face);
    //     }
    // }

    for(const auto& [id, relation] : osm_data.relations)
    {

        if(relation.IsBuildingType())
        {

            // LOG_INFO(relation);
            // continue;

            OSMFace face;

            for(auto& member : relation.members)
            {
                if(member.type == OSMRelationMemberType::Way)
                {
                    uint64_t ref_way_id = member.ref_id;

                    try
                    {
                        const OSMWay& way = osm_data.ways.at(ref_way_id);
                        std::vector<uint32_t> indices;
                        for(const auto& node_ref : way.refs)
                        {
                            const OSMNode& node = osm_data.nodes.at(node_ref);
                            indices.push_back(node.point_id);

                        }

                        face.indices = indices;
                        mesh.faces.push_back(face);                        
                        // LOG_INFO("building relation : Pushed indices");
                    }catch(const std::exception& e){
                    
                        LOG_ERROR("Problem while creating face from OSMWay Refs : \n{}", e.what());
                    }
                }else{
                    LOG_INFO("Not implemented yet");
                }
            }


        }else if(relation.IsMultipolygonType())
        {
            // LOG_INFO(relation);
            // continue;

            OSMFace face;

            for(auto& member : relation.members)
            {
                if(member.type == OSMRelationMemberType::Way)
                {
                    uint64_t ref_way_id = member.ref_id;

                    try
                    {
                        const OSMWay& way = osm_data.ways.at(ref_way_id);
                        std::vector<uint32_t> indices;
                        for(const auto& node_ref : way.refs)
                        {
                            const OSMNode& node = osm_data.nodes.at(node_ref);
                            indices.push_back(node.point_id);

                        }

                        face.indices = indices;
                        mesh.faces.push_back(face);                        
                        LOG_INFO("building relation : Pushed indices");
                    }catch(const std::exception& e){
                    
                        LOG_ERROR("Problem while creating face from OSMWay Refs : \n{}", e.what());
                    }
                }else{
                    LOG_INFO("Not implemented yet");
                }
            }            
        }
    
    }

    return mesh;
}

int main(int argc, char** argv)
{

    GOSM::Log::Init();

    fs::path osm_file_path;
    fs::path output_mesh_file;

    if(argc > 1)
    {
        osm_file_path = argv[1];
    }else{
        osm_file_path = "C:/gui2one/3D/houdini_19_playground/geo/OSM_data/manhatan_02.osm";
    }

    if( argc > 3 )
    {
        if(strcmp(argv[2], "-o") == 0)
        {
            output_mesh_file = argv[3];
        }
    }else{
        output_mesh_file = "hello_bin.ply";
    }


    LOG_INFO("Loading Data from {}", osm_file_path.filename());
    OSMReader reader;

    OSMData osm_data = reader.Load(osm_file_path);

    LOG_INFO(osm_data);

    if( !osm_data.is_empty)
    {

       
        auto mesh = OSMDataToMesh(osm_data);

        PlyWriter ply_writer;
        // ply_writer.WriteASCII("hello_ascii.ply", mesh);
        ply_writer.WriteBinary(output_mesh_file, mesh);

        std::cout << "num points : "<< mesh.points.size() << std::endl;
        std::cout << "num faces : "<< mesh.faces.size() << std::endl;

    }else{

        std::cout << "OSMData is empty" << std::endl;
        
    }
    
    
    return 0;
}