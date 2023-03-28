#include "BgeoWriter.h"

BgeoWriter::BgeoWriter()
{

}

BgeoWriter::~BgeoWriter()
{

}

void BgeoWriter::WriteASCII(const char *path, OSMMesh &mesh)
{
    std::ofstream out_file(path, std::ios::out);

    out_file << "[\n";

    write_header_ascii(out_file, mesh);
    write_info_ascii(out_file, mesh);
    write_topology_ascii(out_file, mesh);

    out_file << "]\n";
    out_file.close();
}


void BgeoWriter::write_header_ascii(std::ostream &output, OSMMesh& mesh)
{
	output << "\t\"fileversion\", \"19.5.534\"," << std::endl;
	output << "\t\"hasindex\",false," << std::endl;
	output << "\t\"pointcount\"," << mesh.points.size() <<","<< std::endl;
	output << "\t\"vertexcount\",0," << std::endl;
	output << "\t\"primitivecount\",0," << std::endl;
}

void BgeoWriter::write_info_ascii(std::ostream &output, OSMMesh &mesh)
{
	output << "\t\"info\",{" << std::endl;
	output << "\t\t\"date\":\"2023-03-29 00:59:29\"," << std::endl;
	output << "\t\t\"timetocook\":0.000111999999999999998," << std::endl;
	output << "\t\t\"software\":\"Houdini 19.5.534\"," << std::endl;
	output << "\t\t\"artist\":\"Sprayfly\"," << std::endl;
	output << "\t\t\"hostname\":\"DESKTOP-QD1IS0C\"," << std::endl;
	output << "\t\t\"time\":0," << std::endl;
	output << "\t\t\"bounds\":[-2,1.10000002,0,0,0,0]," << std::endl;
	output << "\t\t\"attribute_summary\":\"     1 point attributes:\\tP\\n\"" << std::endl;
	output << "\t}," << std::endl;    
}

void BgeoWriter::write_topology_ascii(std::ostream &output, OSMMesh &mesh)
{
	output << "\t\"topology\",[" << std::endl;
	output << "\t\t\"pointref\",[" << std::endl;
	output << "\t\t\t\"indices\",[]" << std::endl;
	output << "\t\t]" << std::endl;
	output << "\t]," << std::endl;    
}
