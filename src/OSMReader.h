#ifndef OSMREADER__H
#define OSMREADER__H

#pragma once

#include "pugixml.hpp"
#include "PlyWriter.h"

#include "OSMData.h"



class OSMReader
{
public:

public:
    OSMReader();
    ~OSMReader();

    OSMData Load(fs::path path);

    void CollectAllNodes(OSMData& data);
    void CollectAllWays(OSMData& data);
    void CollectAllRelations(OSMData& data);

private:
    pugi::xml_document m_XMLDoc;

};


#endif