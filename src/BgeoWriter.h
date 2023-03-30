#ifndef BGEOWRITER_H
#define BGEOWRITER_H

#pragma once
#include "PlyWriter.h" /* for OSMMesh*/
#include <fstream>
#include <iostream>
#include <string>

class BgeoWriter
{
public:
    BgeoWriter();
    ~BgeoWriter();


    void WriteASCII(const char * path, OSMMesh& mesh);


    void write_header_ascii(std::ostream& output, OSMMesh& mesh);
    void write_info_ascii(std::ostream& output, OSMMesh& mesh);
    void write_topology_ascii(std::ostream& output, OSMMesh& mesh);


    void write_attributes_ascii(std::ostream& output, OSMMesh& mesh);
    void write_point_attributes_ascii(std::ostream& output, OSMMesh& mesh);
private:

};

#endif