#pragma once

#include "Provided.h"

#include <string>

////////////////////////////////////////////////
// Helper Struct
////////////////////////////////////////////////

class a_star_node
{
public:
    a_star_node(const a_star_node* parent, const GeoCoord& location, const GeoCoord& objective, const StreetSegment& ss);

    const a_star_node* a_star_parent;
    GeoCoord a_star_gc;
    StreetSegment a_star_ss;
    std::string string_to_hash;
    double g_cost, h_cost, f_cost;

    bool has_coord(const GeoCoord& other) const;
    bool has_street_seg(const StreetSegment& other) const;
    bool operator<(const a_star_node& other) const;
    bool operator==(const a_star_node& other) const;
private:
};

struct holds_a_star_node_ptr
{
    holds_a_star_node_ptr(const a_star_node* ptr_to_copy);
    a_star_node* ptr;
};

struct holds_a_star_node_ptr_hash
{
    std::size_t operator()(const holds_a_star_node_ptr& container) const;
};

struct custom_compare_for_pq
{
    bool operator()(const holds_a_star_node_ptr &a, const holds_a_star_node_ptr &b);
};

////////////////////////////////////////////////
// Comparison Operators
////////////////////////////////////////////////

bool operator== (const GeoCoord& a, const GeoCoord& b);
bool operator!= (const GeoCoord& a, const GeoCoord& b);
bool operator> (const GeoCoord& a, const GeoCoord& b);
bool operator< (const GeoCoord& a, const GeoCoord& b);

bool operator> (const StreetSegment& a, const StreetSegment& b);
bool operator== (const StreetSegment& a, const StreetSegment& b);

bool operator< (const holds_a_star_node_ptr& a, const holds_a_star_node_ptr& b);
bool operator== (const holds_a_star_node_ptr& a, const holds_a_star_node_ptr& b);

////////////////////////////////////////////////
// Navigation Directions
////////////////////////////////////////////////

std::string angle_to_dir(const double travel_angel);
std::string segment_dir(const GeoSegment& geo_seg);
std::string turn_dir(const GeoCoord& p1, const GeoCoord& p2, const GeoCoord& p3);
void print_nav_segment(NavSegment nav);
