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

    out_file << "]\n";
    out_file.close();
}

void BgeoWriter::WriteBinary(const char *path, OSMMesh &mesh)
{
    std::ofstream out_file(path, std::ios::binary | std::ios::out);

    const char * str = "hello";
    out_file.write(reinterpret_cast<char*>(&str), strlen(str) * sizeof(char));
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
