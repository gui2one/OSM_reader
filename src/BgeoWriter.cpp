#include "BgeoWriter.h"

BgeoWriter::BgeoWriter()
{

}

BgeoWriter::~BgeoWriter()
{

}

void BgeoWriter::WriteASCII(const char *path, OSMMesh &mesh)
{
    /* sort vertices by point_id */
    std::sort(mesh.points.begin(), mesh.points.end(), [](OSMPoint& pt1, OSMPoint& pt2){
        return (pt1.point_id < pt2.point_id);
    });	
    std::ofstream out_file(path, std::ios::out);

    out_file << "[\n";

    write_header_ascii(out_file, mesh);
    write_info_ascii(out_file, mesh);
    write_topology_ascii(out_file, mesh);

	write_attributes_ascii(out_file, mesh);

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

void BgeoWriter::write_attributes_ascii(std::ostream &output, OSMMesh &mesh)
{
	output << "\t\"attributes\",[" << std::endl;
	
	write_point_attributes_ascii(output, mesh);
	
	output << "\t]" << std::endl;

}

void BgeoWriter::write_point_attributes_ascii(std::ostream &output, OSMMesh &mesh)
{
	output << "\t\t\"pointattributes\",[" << std::endl;
	output << "\t\t\t[" << std::endl;

	/* attributes 'properties' ? */

	output << "\t\t\t\t[ " << std::endl;
	output << "\t\t\t\t\t\"scope\",\"public\"," << std::endl;
	output << "\t\t\t\t\t\"type\",\"numeric\"," << std::endl;
	output << "\t\t\t\t\t\"name\",\"P\"," << std::endl;
	output << "\t\t\t\t\t\"options\",{" << std::endl;
	output << "\t\t\t\t\t\t\"type\":{" << std::endl;
	output << "\t\t\t\t\t\t\t\"type\":\"string\"," << std::endl;
	output << "\t\t\t\t\t\t\t\"value\":\"point\"" << std::endl;
	output << "\t\t\t\t\t\t}" << std::endl;
	output << "\t\t\t\t\t}" << std::endl;
	output << "\t\t\t\t]," << std::endl;

	/* end properties */


	/* 
		START attribute Data ? 
	*/
	output << "\t\t\t\t[" << std::endl;	

	output <<"\t\t\t\t\t\"size\",3," << std::endl;
	output <<"\t\t\t\t\t\"storage\",\"fpreal32\"," << std::endl;
	output <<"\t\t\t\t\t\"defaults\",[" << std::endl;
	output <<"\t\t\t\t\t\t\"size\",1," << std::endl;
	output <<"\t\t\t\t\t\t\"storage\",\"fpreal64\"," << std::endl;
	output <<"\t\t\t\t\t\t\"values\",[0]" << std::endl;
	output <<"\t\t\t\t\t]," << std::endl;


	/* point  positions */

	output << "\t\t\t\t\t\"values\",[" << std::endl;
	output << "\t\t\t\t\t\t\"size\",3," << std::endl;
	output << "\t\t\t\t\t\t\"storage\",\"fpreal32\"," << std::endl;
	output << "\t\t\t\t\t\t\"tuples\",[";


	/* write actual positions tuples */
	uint64_t inc = 0;
	uint64_t num_points = mesh.points.size();
	for(const auto pt : mesh.points)
	{
		output << std::setprecision(20) << std::fixed;
		output << "[" << pt.x << "," << pt.y << "," << pt.z << "]";

		if(inc < num_points - 1)
		{
			output << ",";
		}

		inc ++;
	}
	output << std::endl;
	output << "\t\t\t\t\t\t]" << std::endl;

	/* end positions */
	
	output << "\t\t\t\t\t] " << std::endl;	/* end values */

	output << "\t\t\t\t]" << std::endl;	/* end attribute data */

	/* 
		END attribute Data
	*/


	output << "\t\t\t]" << std::endl; 

	output << "\t\t]" << std::endl; /* end point attributes */
}
