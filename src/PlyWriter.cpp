#include "PlyWriter.h"

PlyWriter::PlyWriter()
{

}

PlyWriter::~PlyWriter()
{

}

void PlyWriter::WriteASCII(const char *path, OSMMesh &osm_mesh)
{
    std::ofstream out_file(path);


    std::stringstream ss;


    ss << "ply" << std::endl;
    ss << "format ascii 1.0" << std::endl;
    ss << "comment made by gui2one (PlyWriter.cpp v0.1)" << std::endl;
    ss << "comment this file is a cube" << std::endl;
    ss << "element vertex " << osm_mesh.points.size()<< std::endl;
    ss << "property float x" << std::endl;
    ss << "property float y" << std::endl;
    ss << "property float z" << std::endl;
    ss << "property uint point_id" << std::endl;

    ss << "element face " << osm_mesh.faces.size() << std::endl;
    ss << "property list uchar uint vertex_index" << std::endl;
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
        ss << std::fixed << (uint64_t)point.point_id << std::endl;
    }

    for(const auto& face : osm_mesh.faces)
    {
        /* first num in the line is the number of indices of the polygon */
        ss << face.size() << " ";
        for(auto& index : face)
        {
            ss << std::fixed;
            ss << std::setprecision(20);
            ss << index << " ";
        }

        /* repeat first vertex to close ? */
        // ss << face[0];

        ss << std::endl;
    }



    /* write stringstream buffer content to file */
    out_file << ss.rdbuf();

    out_file.close();
}
