#include "provided.h"
#include "MyMap.h"
#include "support.h"

#include <string>
#include <iostream>
#include <cassert>

using namespace std;

void test_mymap()
{
    //Testing Empty MyMap
    MyMap<string, int> empty;
    assert(empty.size() == 0);
    assert(empty.find("YoonA") == nullptr);

    //Testing Insertion and size after Insertion
    MyMap<string, int> name_to_rank;
    assert(name_to_rank.size() == 0);
    name_to_rank.associate("YoonA", 9);
    name_to_rank.associate("Taeyeon", 8);
    name_to_rank.associate("Jessica", 7);
    assert(name_to_rank.size() == 3);

    // Testing Find
    int* taeyeons_rank = name_to_rank.find("Taeyeon");
    assert(*taeyeons_rank == 8);

    // Testing Value Association Changes
    *taeyeons_rank = 5;
    assert(*taeyeons_rank == 5);

    // Testing find after value association change
    assert(*(name_to_rank.find("Taeyeon")) == 5);

    // Finding Non-existant key
    assert(name_to_rank.find("Hyuna") == nullptr);

    // Testing deletion
    int* jessica_rank = name_to_rank.find("Jessica");
}

void stress_mymap(int num_iterations)
{
    // Memory Leak Test
    // Can handle 10,000 iterations
    for (int i = 0; i < num_iterations; i++) {
        MyMap<int, int>* map = new MyMap<int, int>();
        delete map;
    }

    // Stress Test
    // Cannot handle 10,000 iterations
    // Most likely because of being an unbalanced tree
    MyMap<int, int> stress_test;
    for (int i = 0; i < num_iterations; i++) {
        stress_test.associate(i, i * 2);
    }

    // Destructor Test
    for (int i = 0; i < num_iterations; i++) {
        MyMap<int, int> map;
    }
}

bool approx_double(double retrieved, double rounded)
{
    if (abs(rounded - retrieved) < 0.01) return true;
    return false;
}

void test_map_loader(MapLoader& ml)
{
    // Testing Size
    assert(ml.getNumSegments() == 19641);

    // Testing that it can retrieve segments properly
    StreetSegment temp_ss;
    assert(ml.getSegment(2, temp_ss));
    assert(temp_ss.streetName == "12th Helena Drive");
    assert(temp_ss.segment.start.latitudeText == "34.0552554");
    assert(temp_ss.segment.end.longitudeText == "-118.4803801");
}

void stress_maploader(MapLoader& ml, int num_iterations)
{
    // Finding a bunch of random segments within MapLoader
    // Make sure it doesn't take forever
    for (int i = 0; i < num_iterations; i++) {
        size_t rand_seg_num = rand() % ml.getNumSegments();
        StreetSegment temp_segment;
        assert(ml.getSegment(rand_seg_num, temp_segment));
    }
}

void test_attraction_mapper(AttractionMapper& am)
{
    // Testing that it can retrieve geocoords, even in incorrect camelcase input
    GeoCoord temp_gc;
    assert(am.getGeoCoord("ThAlians MentAl HeaLth CenTer", temp_gc));
    assert(temp_gc.latitudeText == "34.0751447");
    assert(temp_gc.longitudeText == "-118.3830838");

    // Testing location that does not exist
    assert(!am.getGeoCoord("Does Not Exist", temp_gc));

    // Making sure that the coordinates aren't overwritten if the previous does not exist
    assert(temp_gc.latitudeText == "34.0751447");
    assert(temp_gc.longitudeText == "-118.3830838");
}

void test_segment_mapper(SegmentMapper& sm)
{
    vector<StreetSegment> vs;
    GeoCoord idk("34.0617768", "-118.4466596");
    vs = sm.getSegments(idk);
    assert(vs[0].streetName == "Broxton Avenue");

    GeoCoord empty;
    vs = sm.getSegments(empty);
    assert(vs.empty());
}

void test_angle_of_line()
{
    GeoCoord g1("34.0613269", "-118.4462765");
    GeoCoord g2("34.0613323", "-118.4461140");
    assert(approx_double(angleOfLine(GeoSegment(g1, g2)), 1.90));
}

void test_navigator(MapLoader& ml, AttractionMapper& am, SegmentMapper& sm, Navigator& nav)
{
    vector<NavSegment> directions;

    cout << "TEST 1" << endl;

    // Standard Navigation
    nav.navigate("De Neve Plaza", "Hedrick Hall", directions);
    cout << "TEST 2" << endl;
    nav.navigate("1061 Broxton Avenue", "Headlines!", directions);
    cout << "TEST 3" << endl;
    nav.navigate("Headlines", "Fatburger", directions);
    cout << "TEST 4" << endl;
    nav.navigate("Rieber Terrace", "Hedrick Summit", directions);

    // Same GeoSegment Navigation
    cout << "TEST 5" << endl;
    nav.navigate("1031 Broxton Avenue", "1061 Broxton Avenue", directions);

    // Same Location Navigation
    cout << "TEST 6" << endl;
    nav.navigate("Getty Center Car Parking Garage", "Getty Center Car Parking Garage", directions);

    // Invalid Location Start
    cout << "TEST 7" << endl;
    nav.navigate("idk", "Hedrick Hall", directions);

    // Invalid Location End
    cout << "TEST 8" << endl;
    nav.navigate("Hedrick Hall", "idk", directions);

    // No Route Navigation
    cout << "TEST 9" << endl;
    nav.navigate("1031 Broxton Avenue", "The Maltz Park", directions);
    cout << "TEST 10" << endl;
    nav.navigate("Ami Sushi", "The Annenberg Space for Photography", directions);
    nav.navigate("Whisky a Go Go", "Flame International", directions);
    cout << "START OF OTHER TESTS: " << endl;
}

int main()
{
    const int num_iterations = 1000;

    test_mymap();
    stress_mymap(1000);

    const std::string MAP_DATA = "/Users/nicho/source/repos/BruinNav/BruinNav/mapdata.txt";
    MapLoader ml;
    assert(ml.load(MAP_DATA));
    
    test_map_loader(ml);
    stress_maploader(ml, num_iterations);

    AttractionMapper am;
    am.init(ml);
    test_attraction_mapper(am);

    SegmentMapper sm;
    sm.init(ml);
    test_segment_mapper(sm);

    test_angle_of_line();

    cerr << "ABOUT TO TAKE FOREVER" << endl;

    Navigator nav;
    nav.loadMapData(MAP_DATA);
    vector<NavSegment> directions;
    
    test_navigator(ml, am, sm, nav);
    
    cout << "TEST 9" << endl;
    nav.navigate("1031 Broxton Avenue", "The Maltz Park", directions);
    cout << "TEST 10" << endl;
    nav.navigate("Ami Sushi", "The Annenberg Space for Photography", directions);
    nav.navigate("Whisky a Go Go", "Flame International", directions);
    cout << "START OF OTHER TESTS: " << endl;

    cerr << "All tests succeeded" << endl;
}
