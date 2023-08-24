#include "MyMap.h"
#include "Provided.h"
#include "Support.h"

#include <string>
#include <vector>
#include <stack>
#include <iostream>
#include <algorithm>

#include <queue>
#include <unordered_set>

using namespace std;

class NavigatorImpl
{
public:
    NavigatorImpl();
    ~NavigatorImpl();
    bool load_map_data(string map_file);
    NavResult navigate(const string &start, const string &end, vector<NavSegment>& directions) const;
private:
    MapLoader map_load_;
    AttractionMapper attr_map_;
    SegmentMapper seg_map_;

    vector<a_star_node> get_neighbor_nodes(const a_star_node* &origin_ptr, const SegmentMapper& seg_map, const GeoCoord& geo_coord) const;
    vector<NavSegment> get_return_route(const a_star_node &end_node) const;

    void clean_up_nodes(priority_queue<holds_a_star_node_ptr, vector<holds_a_star_node_ptr>, custom_compare_for_pq> &open_list, unordered_set<holds_a_star_node_ptr, holds_a_star_node_ptr_hash> &closed_list) const;
};

NavigatorImpl::NavigatorImpl()
{

}

NavigatorImpl::~NavigatorImpl()
{

}

bool NavigatorImpl::load_map_data(string map_file)
{
    if (!map_load_.load(map_file)) return false;

    attr_map_.init(map_load_);
    seg_map_.init(map_load_);
    return true;
}

vector<a_star_node> NavigatorImpl::get_neighbor_nodes(const a_star_node* &origin_ptr, const SegmentMapper& seg_map, const GeoCoord& end_coord) const
{
    vector<a_star_node> neighbors;
    const vector<StreetSegment> connecting_streets = seg_map.getSegments(origin_ptr->a_star_gc);
    for (StreetSegment specific_street : connecting_streets) {
        // Given a street, we want to make sure we have both the start and end of the street segment
        // We will not add a start or an end that we already have though
        if (!(origin_ptr->has_coord(specific_street.segment.start))) {
            const a_star_node start_of_street(origin_ptr, specific_street.segment.start, end_coord, specific_street);
            neighbors.push_back(start_of_street);
        }
        if (!(origin_ptr->has_coord(specific_street.segment.end))) {
            const a_star_node end_of_street(origin_ptr, specific_street.segment.end, end_coord, specific_street);
            neighbors.push_back(end_of_street);
        }
        // We will only add the desired end attraction
        for (const Attraction& attr : specific_street.attractions) {
            // If a is this attraction, we skip it
            if (origin_ptr->has_coord(attr.geocoordinates)) continue;
            // Otherwise if the attractrion shares the same end coords, we'll add it
            if (attr.geocoordinates == end_coord) {
                const a_star_node attr_node(origin_ptr, attr.geocoordinates, end_coord, specific_street);
                neighbors.push_back(attr_node);
            }
        }
    }
    return neighbors;
}

vector<NavSegment> NavigatorImpl::get_return_route(const a_star_node &end_node) const
{
    stack<NavSegment> nav_seg_stack;
    a_star_node current_node = end_node;
    const a_star_node* parent_node_ptr = current_node.a_star_parent;

    while (parent_node_ptr != nullptr) {
        const GeoSegment current_seg(parent_node_ptr->a_star_gc, current_node.a_star_gc);
        const string direction = segment_dir(current_seg);
        const double distance = distanceEarthMiles(current_node.a_star_gc, parent_node_ptr->a_star_gc);
        const string street_name = current_node.a_star_ss.streetName;

        NavSegment proceed_nav_seg(direction, street_name, distance, current_seg);
        nav_seg_stack.push(proceed_nav_seg);

        // Turning onto a new street
        if (current_node.a_star_ss.streetName != parent_node_ptr->a_star_ss.streetName) {
            // If the parent street is a different name than the current street
            // Then there must be another GeoCoord leading to that street
            // 
            // We went from grandparent node to parent node
            // Now we need to know if we are turning right or left from parent node to street name
            const GeoCoord p1 = parent_node_ptr->a_star_parent->a_star_gc;
            const GeoCoord p2 = parent_node_ptr->a_star_gc;
            const GeoCoord p3 = current_node.a_star_gc;
            const string turn_direction = turn_dir(p1, p2, p3);
            const NavSegment turn_nav_seg(turn_direction, street_name);
            nav_seg_stack.push(turn_nav_seg);
        }

        current_node = *parent_node_ptr;
        parent_node_ptr = parent_node_ptr->a_star_parent;
    }

    // The Nav_Seg stack now has all the directions until the beginning
    // The top of the stack is at the start node
    vector<NavSegment> route;
    while (!nav_seg_stack.empty()) {
        route.push_back(nav_seg_stack.top());
        nav_seg_stack.pop();
    }

    for (int i = 0; i < route.size(); i++) {
        print_nav_segment(route[i]);
    }

    return route;
}

void NavigatorImpl::clean_up_nodes(priority_queue<holds_a_star_node_ptr, vector<holds_a_star_node_ptr>, custom_compare_for_pq> &open_list, unordered_set<holds_a_star_node_ptr, holds_a_star_node_ptr_hash> &closed_list) const
{
    while (!open_list.empty())
    {
        const a_star_node* node_to_delete = open_list.top().ptr;
        open_list.pop();
        delete node_to_delete;
    }

    for (const auto& specific_container : closed_list)
    {
        const a_star_node* node_to_delete = specific_container.ptr;
        delete node_to_delete;
    }

    closed_list.clear();
}

NavResult NavigatorImpl::navigate(const string &start, const string &end, vector<NavSegment>& directions) const
{
    GeoCoord start_coord;
    if (!attr_map_.getGeoCoord(start, start_coord)) return NAV_BAD_SOURCE;
    GeoCoord end_coord;
    if (!attr_map_.getGeoCoord(end, end_coord)) return NAV_BAD_DESTINATION;

    if (start_coord == end_coord) return NAV_SUCCESS; // Unsure what to do with this

    priority_queue<holds_a_star_node_ptr, vector<holds_a_star_node_ptr>, custom_compare_for_pq> open_list;
    unordered_set<holds_a_star_node_ptr, holds_a_star_node_ptr_hash> closed_list;

    const StreetSegment start_street = seg_map_.getSegments(start_coord)[0];
    const a_star_node *start_node_ptr = new a_star_node(nullptr, start_coord, end_coord, start_street);
    const holds_a_star_node_ptr copy_start_node_ptr(start_node_ptr);
    open_list.push(copy_start_node_ptr);

    while (!open_list.empty()) {
        const a_star_node* current_node_ptr = open_list.top().ptr;
        open_list.pop();

        // We'll see if we can find it in the closed list
        if (closed_list.find(current_node_ptr) != closed_list.end()) {
            continue;
        }

        const vector<a_star_node> neighbors = get_neighbor_nodes(current_node_ptr, seg_map_, end_coord);
        for (a_star_node specific_neighbor : neighbors) {
            // Found the Result
            if (specific_neighbor.has_coord(end_coord)) {
                // Now let's trace back through and get the directions
                directions = get_return_route(specific_neighbor);

                clean_up_nodes(open_list, closed_list);

                // We'll stop here
                return NAV_SUCCESS;
            }

            else {
                if (closed_list.find(&specific_neighbor) == closed_list.end()) {
                    const a_star_node* neighbor_to_add = new a_star_node(specific_neighbor);
                    const holds_a_star_node_ptr neighbor_ptr(neighbor_to_add);
                    open_list.push(neighbor_ptr);
                }
            }

        }

        holds_a_star_node_ptr remove_node(current_node_ptr);
        closed_list.insert(remove_node);
    }
    clean_up_nodes(open_list, closed_list);
    return NAV_NO_ROUTE;
}

Navigator::Navigator()
{
    m_impl = new NavigatorImpl;
}

Navigator::~Navigator()
{
    delete m_impl;
}

bool Navigator::loadMapData(string map_file)
{
    return m_impl->load_map_data(map_file);
}

NavResult Navigator::navigate(string start, string end, vector<NavSegment>& directions) const
{
    return m_impl->navigate(start, end, directions);
}
