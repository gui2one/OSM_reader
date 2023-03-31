#include "PlyWriter.h"
#include <cstdint>
PlyWriter::PlyWriter()
{

}

PlyWriter::~PlyWriter()
{

}

void PlyWriter::WriteASCII(fs::path path, OSMMesh &osm_mesh)
{
    std::ofstream out_file(path);


    std::stringstream ss;


    ss << "ply" << std::endl;
    ss << "format ascii 1.0" << std::endl;
    ss << "comment made by gui2one (PlyWriter.cpp v0.1)" << std::endl;
    ss << "element vertex " << osm_mesh.points.size()<< std::endl;
    ss << "property float x" << std::endl;
    ss << "property float y" << std::endl;
    ss << "property float z" << std::endl;
    ss << "property uint point_id" << std::endl;

    ss << "element face " << osm_mesh.faces.size() << std::endl;
    ss << "property list uchar uint vertex_index" << std::endl;
    ss << "property uint is_building" << std::endl;
    ss << "end_header" << std::endl;

    /* sort vertices by point_id */
    std::sort(osm_mesh.points.begin(), osm_mesh.points.end(), [](OSMPoint& pt1, OSMPoint& pt2){
        return (pt1.point_id < pt2.point_id);
    });

    /* write vertices */
    for(const auto& point : osm_mesh.points)
    {
        ss << std::fixed;
        ss << std::setprecision(20) << point.x << " " ;
        ss << std::setprecision(20) << point.y << " " ;
        ss << std::setprecision(20) << point.z << " " ;
        ss << std::fixed << (uint32_t)point.point_id << std::endl;
    }

    for(const auto& face : osm_mesh.faces)
    {
        /* first num in the line is the number of indices of the polygon */
        ss << face.indices.size() << " ";
        for(auto& index : face.indices)
        {
            ss << std::fixed;
            ss << std::setprecision(20);
            ss << index << " ";
        }


        /* repeat first vertex to close ? */
        // ss << face[0];

        /* is_building */
        ss << (uint32_t)42;
        ss << std::endl;
    }



    /* write stringstream buffer content to file */
    out_file << ss.rdbuf();

    out_file.close();
}

struct test_struct
{
    uint32_t id;
    float x;
    float y;
    float z;
};



void PlyWriter::WriteBinary(fs::path path, OSMMesh &osm_mesh)
{

    std::ofstream out_file(path, std::ios::binary | std::ios::out);
    std::stringstream ss;


    ss << "ply" << std::endl;
    ss << "format binary_little_endian 1.0" << std::endl;
    ss << "comment made by gui2one (PlyWriter.cpp v0.1)" << std::endl;
    ss << "element vertex " << osm_mesh.points.size()<< std::endl;
    ss << "property float x" << std::endl;
    ss << "property float y" << std::endl;
    ss << "property float z" << std::endl;
    ss << "property uint point_id" << std::endl;

    ss << "element face " << osm_mesh.faces.size() << std::endl;
    ss << "property list uint uint vertex_index" << std::endl;
    ss << "property uint is_building" << std::endl;
    ss << "property float building__height" << std::endl;
    ss << "property uint is_road" << std::endl;
    ss << "property uint road__lanes" << std::endl;
    ss << "property uint is_multipolygon" << std::endl;
    ss << "property uint is_inner" << std::endl;
    ss << "property uint is_outline" << std::endl;
    ss << "property uint is_footway" << std::endl;
    ss << "end_header" << std::endl;
    
    out_file << ss.rdbuf();

    /* sort vertices by point_id */
    std::sort(osm_mesh.points.begin(), osm_mesh.points.end(), [](OSMPoint& pt1, OSMPoint& pt2){
        return (pt1.point_id < pt2.point_id);
    });
    
    for(auto& pt : osm_mesh.points)
    {
        out_file << std::setprecision(20);
        out_file.write(reinterpret_cast<char*>(&pt), sizeof(pt));
    }
    

    for(auto& face : osm_mesh.faces)
    {
        /* first write number of vertices in this face */
        uint32_t num_verts = (uint32_t)face.indices.size();

        out_file.write(reinterpret_cast<char*>(&num_verts), sizeof(num_verts));

        /* then write the face vertices indices */
        for(auto& index : face.indices )
        {
            out_file.write(reinterpret_cast<char*>(&index), sizeof(index));
        }

        WriteFaceAttribute(out_file, face.is_building);

        WriteFaceAttribute(out_file, face.building__height);

        WriteFaceAttribute(out_file, face.is_road);

        WriteFaceAttribute(out_file, face.road__lanes);

        WriteFaceAttribute(out_file, face.is_multipolygon);

        WriteFaceAttribute(out_file, face.is_inner);

        WriteFaceAttribute(out_file, face.is_outline);

        WriteFaceAttribute(out_file, face.is_footway);
    }

    out_file << std::endl;
    out_file.close();
}
