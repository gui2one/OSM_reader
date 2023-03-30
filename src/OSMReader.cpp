#include "OSMReader.h"


bool OSMWay::HasTag(const char* tag_key)
{
    for(std::map<const char*, const char*>::iterator it = tags.begin(); it != tags.end(); ++it)
    {
        if(strcmp(tag_key, it->first) == 0) 
        {
            return true;
        }
    }

    return false;
}


OSMReader::OSMReader()
{

}

OSMReader::~OSMReader()
{

}

OSMData OSMReader::Load(fs::path path)
{

    if( path.extension() == fs::path(".osm"))
    {

    }
    
    pugi::xml_parse_result result = m_XMLDoc.load_file(path.string().c_str());

    if (!result)
    {
        std::cout << "Error Parsing File " << path << std::endl;
        OSMData empty_data;
        empty_data.is_empty = true;
        return empty_data;
    }
      
    OSMData osm_data; 

    CollectAllWays(osm_data);
    CollectAllNodes(osm_data);
    CollectAllRelations(osm_data);


    std::vector<OSMBuildingRelation> osm_relations;

    auto all_xml_nodes = m_XMLDoc.child("osm").children();
    uint32_t point_number = 0;
    for( const auto& child : all_xml_nodes)
    {
        
        if(strcmp(child.name(), "node") == 0)
        {

            uint64_t node_id =  (uint64_t)child.attribute("id").as_ullong();

            OSMNode osm_node;
            osm_node.node_id = node_id;
            osm_node.point_id = point_number;
            osm_node.lat = child.attribute("lat").as_float();
            osm_node.lon = child.attribute("lon").as_float();
            
            osm_data.nodes[node_id] = osm_node;

            point_number++;   
        }

        if(strcmp(child.name(), "way") == 0)
        {        
            pugi::xpath_node_set tags_set = child.select_nodes("tag");
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

            if( is_highway)
            {
                OSMHighway osm_way;
                
                
                osm_way.id = child.attribute("id").as_ullong();
                pugi::xpath_node_set nd_set = child.select_nodes("nd");
                nd_set.sort(false);
                for(pugi::xpath_node nd_xpath : nd_set)
                {
                    auto nd_node = nd_xpath.node();
                    auto node_ref = nd_node.attribute("ref").as_llong();
                    osm_way.refs.push_back(node_ref);
                }

                osm_data.ways[osm_way.id] = osm_way;

            }else if(is_building){
                OSMBuilding osm_way;
                
                
                osm_way.id = child.attribute("id").as_ullong();
                pugi::xpath_node_set nd_set = child.select_nodes("nd");
                nd_set.sort(false);
                for(pugi::xpath_node nd_xpath : nd_set)
                {
                    auto nd_node = nd_xpath.node();
                    auto node_ref = nd_node.attribute("ref").as_llong();
                    osm_way.refs.push_back(node_ref);
                }

                osm_data.ways[osm_way.id] = osm_way;                
            }

        }
 
    }    



    return osm_data;
}

static TagsMap collect_child_tags(const pugi::xml_node& xml_node)
{
    auto tags_set = xml_node.select_nodes("tag");

    TagsMap tags;
    for(const auto& tag_item : tags_set)
    {   
        auto& tag_node = tag_item.node();
        auto key = tag_node.attribute("k").as_string();
        auto value = tag_node.attribute("v").as_string();

        tags[key] = value;
    }

    return tags;
}

void OSMReader::CollectAllWays(OSMData& data)
{
    auto all_ways_set = m_XMLDoc.child("osm").select_nodes("way");
    
    std::cout << "Collecting All Ways ...";
    
    for(const auto& set_item : all_ways_set)
    {
        auto way_node = set_item.node();
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

        data.ways[osm_way.id] = osm_way;

        
    }

    std::cout << "OK" << std::endl;

}

void OSMReader::CollectAllNodes(OSMData& data)
{

    std::cout << "Collecting All Nodes ....";
    
    auto all_nodes_set = m_XMLDoc.child("osm").select_nodes("node");
    
    uint32_t pt_index = 0;
    for(const auto& set_item : all_nodes_set)
    {
        auto node_node = set_item.node();
        OSMNode osm_node;
        osm_node.node_id = node_node.attribute("id").as_ullong();
        osm_node.point_id = pt_index;

        osm_node.lat = node_node.attribute("lat").as_float();
        osm_node.lon = node_node.attribute("lon").as_float();

        data.nodes[osm_node.node_id] = osm_node;

        pt_index++;
        
    }

    std::cout << "OK" << std::endl;
    
}

void OSMReader::CollectAllRelations(OSMData& data)
{

    std::cout << "Collecting All Relations ....";
    
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
            if(strcmp(member_type, "node") == 0){

                OSMRelationMember member;
                member.type = OSMRelationMemberType::Node;
                member.ref_id = member_node.attribute("ref").as_ullong();
                member.role = member_node.attribute("role").as_string();
                
                relation.members.push_back(member);

            }else if(strcmp(member_type, "way") == 0){


                OSMRelationMember member;
                member.type = OSMRelationMemberType::Way;
                member.ref_id = member_node.attribute("ref").as_ullong();
                member.role = member_node.attribute("role").as_string();
                
                relation.members.push_back(member);                

            }else if(strcmp(member_type, "relation") == 0){


                OSMRelationMember member;
                member.type = OSMRelationMemberType::Relation;
                member.ref_id = member_node.attribute("ref").as_ullong();
                member.role = member_node.attribute("role").as_string();
                
                relation.members.push_back(member);                
            }

            /* collect tags */

            relation.tags = collect_child_tags(relation_node);

        }
        data.relations[relation.id] = relation;
    }
    std::cout << "OK" << std::endl;
}




