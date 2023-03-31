#include "OSMReader.h"

using Utils::str_is_equal;

OSMReader::OSMReader()
{

}

OSMReader::~OSMReader()
{

}

static OSMData empty_data()
{
    OSMData empty_data;
    empty_data.is_empty = true;

    return empty_data;
}
OSMData OSMReader::Load(fs::path path)
{
    if(!fs::exists(path))
    {
        LOG_ERROR("File Not Found : {}", path);
        return empty_data();        
    }

    if( path.extension() != fs::path(".osm"))
    {
        LOG_ERROR("File extension should be .osm,  given extension is : {}", path.extension());
    }

    pugi::xml_parse_result result = m_XMLDoc.load_file(path.string().c_str());

    if (!result)
    {
        LOG_ERROR("Error Parsing File {}" ,path);
        return empty_data();
    }
      
    OSMData osm_data; 

    CollectAllNodes(osm_data);
    CollectAllWays(osm_data);
    CollectAllRelations(osm_data);


    return osm_data;
}

static TagsMap collect_child_tags(const pugi::xml_node& xml_node)
{
    auto tags_set = xml_node.select_nodes("tag");

    TagsMap tags;
    for(const auto& tag_item : tags_set)
    {   
        auto& tag_node = tag_item.node();
        auto key = tag_node.attribute("k").value();
        auto value = tag_node.attribute("v").value();

        tags[key] = value;
    }

    return tags;
}


void OSMReader::CollectAllNodes(OSMData& data)
{   
    auto all_nodes_set = m_XMLDoc.child("osm").select_nodes("node");
    
    uint32_t pt_index = 0;
    for(const auto& set_item : all_nodes_set)
    {
        auto node_node = set_item.node();
        OSMNode osm_node;
        osm_node.node_id = (uint64_t)node_node.attribute("id").as_ullong();
        osm_node.point_id = pt_index;

        auto string_lat = std::string(node_node.attribute("lat").value());

        osm_node.lat = (float)(node_node.attribute("lat").as_double());
        osm_node.lon = (float)(node_node.attribute("lon").as_double());
        
        // std::cout << string_lat << std::endl;
        // std::cout << std::fixed << osm_node.lat << std::endl;
        // std::cout << "---------------------------------------" << std::endl;
        

        data.nodes[osm_node.node_id] = osm_node;

        pt_index++;
        
    }

    LOG_INFO("Collected All Nodes : {}", data.nodes.size());
    
}

void OSMReader::CollectAllWays(OSMData& data)
{
    auto all_ways_set = m_XMLDoc.child("osm").select_nodes("way");

    
    for(const auto& set_item : all_ways_set)
    {
        auto& way_node = set_item.node();
        OSMWay osm_way;
        osm_way.id = way_node.attribute("id").as_ullong();

        auto nd_set = way_node.select_nodes("nd");
        for(const auto& nd_item : nd_set)
        {
            auto& nd_node = nd_item.node();
            uint64_t ref_id = nd_node.attribute("ref").as_ullong();
            osm_way.refs.push_back(ref_id);
        } 


        /* Collect tags */
        osm_way.tags = collect_child_tags(way_node);

        /* Bail out early ?*/
        if( osm_way.HasTag("route")) continue;
        
        if(osm_way.GetTagValue("highway") == std::string("footway")) osm_way.is_footway = true;


        if(osm_way.HasTag("building"))
        {
            osm_way.is_building = true;
            // LOG_INFO("is building : {}", osm_way.is_building);
            if(osm_way.HasTag("height"))
            {
                try{
                    float val = std::atof(osm_way.GetTagValue("height").c_str());

                    // LOG_INFO("Tags Size : {}", osm_way.tags.size());
                    
                    osm_way.building_height = val;
                }catch(std::exception e)
                {
                    // LOG_ERROR("Exception occured : {}", e.what());
                }
                
            }
        }
        if(osm_way.HasTag("highway"))
        {
            osm_way.is_road = true;
            // LOG_INFO("is building : {}", osm_way.is_building);
        }

        data.ways[osm_way.id] = osm_way;

        
    }
    
    LOG_INFO("Collected All Ways : {}", data.ways.size());

}

void OSMReader::CollectAllRelations(OSMData& data)
{
    auto all_relations_set = m_XMLDoc.child("osm").select_nodes("relation");

    for(const auto& relation_item : all_relations_set)
    {
        auto& relation_node = relation_item.node();

        uint64_t id = relation_node.attribute("id").as_ullong();
        OSMRelation relation;
        relation.id = id;

        auto members_set = relation_node.select_nodes("member");

        for(const auto& member_item : members_set)
        {
            auto& member_node = member_item.node();
            auto member_type = member_node.attribute("type").as_string();
            if(str_is_equal(member_type, "node")){

                OSMRelationMember member;
                member.type = OSMRelationMemberType::Node;
                member.ref_id = (uint64_t)member_node.attribute("ref").as_ullong();
                member.role = member_node.attribute("role").as_string();
                
                relation.members.push_back(member);

            }else if(str_is_equal(member_type, "way")){


                OSMRelationMember member;
                member.type = OSMRelationMemberType::Way;
                member.ref_id = (uint64_t)member_node.attribute("ref").as_ullong();
                member.role = member_node.attribute("role").as_string();
                
                relation.members.push_back(member);                

            }else if(str_is_equal(member_type, "relation")){


                OSMRelationMember member;
                member.type = OSMRelationMemberType::Relation;
                member.ref_id = (uint64_t)member_node.attribute("ref").as_ullong();
                member.role = member_node.attribute("role").as_string();
                
                relation.members.push_back(member);                
            }

            /* collect tags */

            relation.tags = collect_child_tags(relation_node);

        }
        

        /* try and filter building:part relations, they seem to cause troubles */
        bool is_building_part = false;
        try{
            auto building_part_value = relation.tags.at("building:part");
            if( building_part_value == "no")
            {
                is_building_part = true;
                // LOG_INFO("is building part");
            }
        }catch(std::exception e)
        {

        }

        if( !is_building_part)
        {
            data.relations[relation.id] = relation;
        }
    }
    LOG_INFO("Collected All Relations : {}", data.relations.size());
}




