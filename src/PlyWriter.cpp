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
    ss << "element face " << osm_mesh.faces.size() << std::endl;
    ss << "property list uchar int vertex_index" << std::endl;
    ss << "end_header" << std::endl;

    /* write vertices */
    for(const auto& point : osm_mesh.points)
    {
        ss << std::fixed ;
        ss << std::setprecision(8) << point.x << " " ;
        ss << std::setprecision(8) << point.y << " " ;
        ss << std::setprecision(8) << point.z << std::endl;
    }
    /* write stringstream buffer content to file */
    out_file << ss.rdbuf();

    out_file.close();
}
