#include "Provided.h"

#include <string>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

class MapLoaderImpl
{
public:
	MapLoaderImpl();
	~MapLoaderImpl();
	bool load(string map_file);
	size_t get_num_segments() const;
	bool get_segment(size_t seg_num, StreetSegment& seg) const;

private:
	size_t size_;
	vector<StreetSegment*> segments_;
};

MapLoaderImpl::MapLoaderImpl()
	:size_(0)
{

}

MapLoaderImpl::~MapLoaderImpl()
{
	for (int i = 0; i < size_; i++) {
		delete segments_[i];
	}
}

bool MapLoaderImpl::load(string map_file)
{
	ifstream infile(map_file);
	if (!infile) return false;

	string temp_string;
	while (getline(infile, temp_string)) {
		// Name of Segment
		segments_.push_back(new StreetSegment);
		segments_[size_]->streetName = temp_string;

		// Start and End Coords
		string coords[4];
		getline(infile, temp_string, ',');
		coords[0] = temp_string;
		getline(infile, temp_string, ' ');
		getline(infile, temp_string, ' ');
		coords[1] = temp_string;
		getline(infile, temp_string, ',');
		coords[2] = temp_string;
		getline(infile, temp_string);
		coords[3] = temp_string;

		GeoCoord start_coord(coords[0], coords[1]);
		GeoCoord end_coord(coords[2], coords[3]);
		segments_[size_]->segment = GeoSegment(start_coord, end_coord);

		// Getting Attractions
		getline(infile, temp_string);
		int num_attr = stoi(temp_string);
		for (int i = 0; i < num_attr; i++) {
			getline(infile, temp_string, '|');
			string attr_name = temp_string;
			getline(infile, temp_string, ',');
			string lat = temp_string;
			getline(infile, temp_string, ' ');
			getline(infile, temp_string);
			string lon = temp_string;
			GeoCoord location(lat, lon);

			Attraction a;
			a.name = attr_name;
			a.geocoordinates = location;
			segments_[size_]->attractions.push_back(a);
		}
		size_++;
	}
	return true;
}

size_t MapLoaderImpl::get_num_segments() const
{
	return size_;
}

bool MapLoaderImpl::get_segment(size_t seg_num, StreetSegment& seg) const
{
	if (seg_num >= size_) return false;
	seg = *segments_[seg_num];
	return true;
}

//******************** MapLoader functions ************************************

// These functions simply delegate to MapLoaderImpl's functions.
// You probably don't want to change any of this code.

MapLoader::MapLoader()
{
	m_impl = new MapLoaderImpl;
}

MapLoader::~MapLoader()
{
	delete m_impl;
}

bool MapLoader::load(string mapFile)
{
	return m_impl->load(mapFile);
}

size_t MapLoader::getNumSegments() const
{
	return m_impl->get_num_segments();
}

bool MapLoader::getSegment(size_t segNum, StreetSegment& seg) const
{
	return m_impl->get_segment(segNum, seg);
}
