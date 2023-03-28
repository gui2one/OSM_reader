#include "OSMReader.h"


std::ostream& operator<<(std::ostream& os, const OSMNode& self)
{
    os << "OSM Node "  << self.node_id ;
    os << " --point_id -> "  << self.point_id << std::endl;

    return os;
}

std::ostream& operator<<(std::ostream& os, const OSMWay& way)
{
    os << "OSM Way "  << way.id;
    os << " --Num Refs -> "  << way.refs.size() << std::endl;
    return os;
}

OSMReader::OSMReader()
{

}

OSMReader::~OSMReader()
{

}

OSMData OSMReader::Load(const char *path)
{
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(path);
    // pugi::xml_parse_result result = doc.load_file("C:/gui2one/3D/houdini_19_playground/geo/OSM_data/rennes_01.osm");
    // pugi::xml_parse_result result = doc.load_file("C:/gui2one/3D/houdini_19_playground/geo/OSM_data/test.osm");
    if (!result)
    {
        std::cout << "Error Parsing File " << path << std::endl;
        // return {};
    }
        
        
    OSMData osm_data;
    std::map<uint64_t, OSMNode> osm_nodes;
    std::map<uint64_t, OSMWay> osm_ways;
    std::vector<OSMBuildingRelation> osm_relations;

    auto all_xml_nodes = doc.child("osm").children();
    uint64_t point_number = 0;
    for( const auto& child : all_xml_nodes)
    {
        
        if(strcmp(child.name(), "node") == 0)
        {

            uint64_t node_id =  (uint64_t)child.attribute("id").as_ullong();

            OSMNode osm_node;
            osm_node.node_id = node_id;
            osm_node.point_id = (uint64_t)point_number;
            osm_node.lat = child.attribute("lat").as_float();
            osm_node.lon = child.attribute("lon").as_float();
            
            osm_nodes[node_id] = osm_node;

            point_number++;   
        }

        if( strcmp(child.name(), "relation") == 0)
        {

            bool is_multipolygon_rel = false;
            bool is_building_rel = false;        
            auto& tag_set = child.select_nodes("tag");
            for(const auto& tag_item : tag_set)
            {
        
                auto& tag_node = tag_item.node();
                if( strcmp(tag_node.attribute("k").value(), "type") == 0 && strcmp(tag_node.attribute("v").value(), "multipolygon") == 0)
                {
                    is_multipolygon_rel = true;

                }
                if( strcmp(tag_node.attribute("k").value(), "building") == 0 && strcmp(tag_node.attribute("v").value(), "yes") == 0)
                {
                    is_building_rel = true;

                }
             
            } 


            if( is_building_rel && is_multipolygon_rel)
            {
                OSMBuildingRelation relation;
                auto& member_set = child.select_nodes("member");

                for(const auto& member_item: member_set)
                {
                    auto& member_node = member_item.node();
                    
                    if(strcmp(member_node.attribute("type").value(), "way") == 0)
                    {
                        std::cout << "multipolygon -> " << member_node.attribute("role").value() << std::endl;
                        
                    }
                    
                }
            }   


        }
    
    }    

    osm_data.nodes = osm_nodes;


    pugi::xpath_node_set ways_set = doc.select_nodes("/osm/way");
    ways_set.sort(false);
    for (pugi::xpath_node way: ways_set)
    {
        pugi::xml_node _way = way.node();

        /* check if <tag k="highway" .... exists */
        pugi::xpath_node_set tags_set = _way.select_nodes("tag");
        bool is_highway = false;
        bool is_building = false;
        for(const auto& tag : tags_set){
            auto& tag_node = tag.node();
            const char* tag_key = tag_node.attribute("k").as_string();
            const char* tag_value = tag_node.attribute("v").value();
            // std::cout << tag_key << std::endl;
            
            if(strcmp(tag_key, "highway") == 0)
            {
                is_highway = true;
                break;
            }
            // if(strcmp(tag_key, "building") == 0 && strcmp(tag_value, "yes") == 0 )
            // {
            //     is_building = true;
            //     break;
            // }
            if(strcmp(tag_key, "building") == 0 )
            {
                is_building = true;
                break;
            }

        }

        /* ---------------- */

        if( is_highway || is_building)
        {
            OSMWay osm_way;
            osm_way.id = _way.attribute("id").as_ullong();
            pugi::xpath_node_set nd_set = _way.select_nodes("nd");
            nd_set.sort(false);
            for(pugi::xpath_node nd_xpath : nd_set)
            {
                auto nd_node = nd_xpath.node();
                auto node_ref = nd_node.attribute("ref").as_llong();
                osm_way.refs.push_back(node_ref);
            }

            osm_ways[osm_way.id] = osm_way;

        }
    }    

    osm_data.ways = osm_ways;    

    return osm_data;
}
