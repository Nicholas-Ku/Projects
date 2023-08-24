#include "Support.h"
#include "Provided.h"

#include <iostream>

////////////////////////////////////////////////
// Helper Struct
////////////////////////////////////////////////

// Constructor
a_star_node::a_star_node(const a_star_node* parent, const GeoCoord& location, const GeoCoord& objective, const StreetSegment& ss)
{
    a_star_parent = parent;
    a_star_gc = location;
    a_star_ss = ss;
    string_to_hash = a_star_gc.latitudeText + a_star_gc.longitudeText;

    if (a_star_parent == nullptr) {
        f_cost = h_cost = distanceEarthMiles(a_star_gc, objective);
        g_cost = 0;
    }

    else {
        g_cost = parent->g_cost + distanceEarthMiles(parent->a_star_gc, a_star_gc);
        h_cost = distanceEarthMiles(a_star_gc, objective);
        f_cost = g_cost + h_cost;
    }
}

bool a_star_node::has_coord(const GeoCoord& other) const
{
    return (a_star_gc == other);
}

bool a_star_node::has_street_seg(const StreetSegment& other) const
{
    if (a_star_ss == other) return true;
    else return false;
}

bool a_star_node::operator<(const a_star_node& other) const
{
    if (a_star_gc != other.a_star_gc) return (a_star_gc < other.a_star_gc);
    else if (a_star_ss == other.a_star_ss) return !(a_star_ss > other.a_star_ss);
    return false;
}

bool a_star_node::operator==(const a_star_node& other) const
{
    return has_coord(other.a_star_gc);
}

holds_a_star_node_ptr::holds_a_star_node_ptr(const a_star_node* ptr_to_copy)
{
    // Ask about this later
    ptr = const_cast<a_star_node*>(ptr_to_copy);
}

size_t holds_a_star_node_ptr_hash:: operator() (const holds_a_star_node_ptr& container) const
{
    const std::size_t h1 = std::hash<std::string>{}(container.ptr->string_to_hash);
    return h1;
}

bool custom_compare_for_pq::operator()(const holds_a_star_node_ptr &a, const holds_a_star_node_ptr &b)
{
    const a_star_node &first = *(a.ptr);
    const a_star_node &second = *(b.ptr);
    
    if (first.f_cost != second.f_cost)
        return first.f_cost < second.f_cost;
    else if (first.h_cost != second.h_cost)
        return first.h_cost < second.h_cost;
    else if (first.g_cost != second.g_cost)
        return first.g_cost < second.g_cost;
    else
        return true;
}

////////////////////////////////////////////////
// Comparison Operators
////////////////////////////////////////////////

bool operator== (const GeoCoord& a, const GeoCoord& b)
{
    return (a.latitude == b.latitude && a.longitude == b.longitude);
}

bool operator!= (const GeoCoord& a, const GeoCoord& b)
{
    return !(a == b);
}

bool operator> (const GeoCoord& a, const GeoCoord& b)
{
    if (a.latitude > b.latitude) return true;

    else if (a.latitude == b.latitude) {
        if (a.longitude > b.longitude) return true;
        else return false;
    }

    return false;
}

bool operator< (const GeoCoord& check1, const GeoCoord& check2)
{
    return !(check1 > check2);
}

bool operator> (const StreetSegment& check1, const StreetSegment& check2)
{
    if (check1.segment.start != check2.segment.start) {
        if (check1.segment.start.latitudeText != check2.segment.start.latitudeText)
            return check1.segment.start.latitude > check2.segment.start.latitude;
        else if (check1.segment.start.longitudeText != check2.segment.start.longitudeText)
            return check1.segment.start.longitude > check2.segment.start.longitude;
    }

    else if (check1.segment.end != check2.segment.end) {
        if (check1.segment.end.latitudeText != check2.segment.end.latitudeText)
            return check1.segment.end.latitude > check2.segment.end.latitude;
        else if (check1.segment.end.longitudeText != check2.segment.end.longitudeText)
            return check1.segment.end.longitude > check2.segment.end.longitude;
    }

    else if (check1.streetName != check2.streetName) {
        return check1.streetName < check2.streetName;
    }

    return false;
}

bool operator== (const StreetSegment& check1, const StreetSegment& check2)
{
    if (check1.segment.start == check2.segment.start && check1.segment.end == check2.segment.end) return true;
    else return false;
}

bool operator< (const holds_a_star_node_ptr& a, const holds_a_star_node_ptr& b)
{
    return (*(a.ptr) < *(b.ptr));
}

bool operator== (const holds_a_star_node_ptr& a, const holds_a_star_node_ptr& b)
{
    return (*(a.ptr) == *(b.ptr));
}

////////////////////////////////////////////////
// Navigation Directions
////////////////////////////////////////////////

std::string angle_to_dir(const double travel_angle)
{
    if (0 <= travel_angle && travel_angle <= 22.5) return "east";
    else if (travel_angle <= 62.5) return "northeast";
    else if (travel_angle <= 112.5) return "north";
    else if (travel_angle <= 157.5) return "northwest";
    else if (travel_angle <= 202.5) return "west";
    else if (travel_angle <= 247.5) return "southwest";
    else if (travel_angle <= 292.5) return "south";
    else if (travel_angle <= 337.5) return "southeast";
    else if (travel_angle <= 360) return "east";
}

std::string segment_dir(const GeoSegment& geoseg)
{
    return angle_to_dir(angleOfLine(geoseg));
}

std::string turn_dir(const GeoCoord& p1, const GeoCoord& p2, const GeoCoord& p3)
{
    // Dot Product
    // We are going from p1 to p2 to p3
    // After we get to p2 from p1, we need to know if we should go left or right to get to p3
    const double v1x = p2.latitude - p1.latitude;
    const double v1y = p2.longitude - p1.longitude;
    const double v2x = p3.latitude - p2.latitude;
    const double v2y = p3.longitude - p2.longitude;

    // Negative dot product means to turn left
    // Positive dot product means to turn right
    // Zero dot product would imply that we don't go to p2, but will not happen for our purposes
    if (v1x * v2y - v1y * v2x < 0) return "left";
    else return "right";
}

void print_nav_segment(const NavSegment nav)
{
    if (nav.m_command == 0)
    {
        std::cout << "ACTION: PROCEED" << std::endl;

        std::cout << "Start of Segment: " << std::endl;
        GeoCoord start_of_seg = nav.m_geoSegment.start;
        std::cout << "Start Latitude: " << start_of_seg.latitudeText << std::endl;
        std::cout << "Start Longitude: " << start_of_seg.longitudeText << std::endl;

        std::cout << "End of Segment: " << std::endl;
        GeoCoord end_of_seg = nav.m_geoSegment.end;
        std::cout << "End Latitude: " << end_of_seg.latitude << std::endl;
        std::cout << "End Longitude: " << end_of_seg.longitude << std::endl;

        std::cout << "Distance: " << nav.m_distance << std::endl;
    }

    else
    {
        std::cout << "ACTION: TURN" << std::endl;
    }

    std::cout << "Direction: " << nav.m_direction << std::endl;
    std::cout << "Street: " << nav.m_streetName << std::endl;
    std::cout << std::endl;
}
