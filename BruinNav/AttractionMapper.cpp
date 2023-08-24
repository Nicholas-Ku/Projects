#include "MyMap.h"
#include "provided.h"
#include <iostream>
#include <string>
using namespace std;

class AttractionMapperImpl
{
public:
	AttractionMapperImpl();
	~AttractionMapperImpl();
	void init(const MapLoader& map_loader);
	bool get_geo_coord(string attr, GeoCoord& geo_coord);

	void test_print();

private:
	MyMap<string, GeoCoord> attr_map_;
	int num_attrs_;
	string to_lowercase(const string& convert) const;
};

///////////////////////////////////////////////////////////////////////////
//  Implementation
///////////////////////////////////////////////////////////////////////////

void AttractionMapperImpl::test_print()
{
	cout << num_attrs_;
}

string AttractionMapperImpl::to_lowercase(const string& convert) const
{
	string lower_case;
	for (int i = 0; i < convert.size(); i++) {
		lower_case += tolower(convert[i]);
	}
	return lower_case;
}

AttractionMapperImpl::AttractionMapperImpl()
{
	num_attrs_ = 0;
}

AttractionMapperImpl::~AttractionMapperImpl()
{

}

void AttractionMapperImpl::init(const MapLoader& map_loader)
{
	StreetSegment seg;
	size_t num_seg = map_loader.getNumSegments();
	for (int i = 0; i < num_seg; i++) {
		if (map_loader.getSegment(i, seg)) {
			const size_t attr_num = seg.attractions.size();
			for (int j = 0; j < attr_num; j++) {
				const string attr_name = to_lowercase(seg.attractions[j].name);
				const auto attr_coord = seg.attractions[j].geocoordinates;
				attr_map_.associate(attr_name, attr_coord);
				num_attrs_++;
			}
		}
	}
}

bool AttractionMapperImpl::get_geo_coord(string attr, GeoCoord& geo_coord)
{
	const string attr_lowercase = to_lowercase(attr);
	const GeoCoord* temp_geo_coord = attr_map_.find(attr_lowercase);

	if (temp_geo_coord != nullptr) {
		geo_coord = *temp_geo_coord;
		return true;
	}

	return false;
}

///////////////////////////////////////////////////////////////////////////
//  Attraction Mapper Functions
///////////////////////////////////////////////////////////////////////////

AttractionMapper::AttractionMapper()
{
	m_impl = new AttractionMapperImpl;
}

AttractionMapper::~AttractionMapper()
{
	delete m_impl;
}

void AttractionMapper::init(const MapLoader& ml)
{
	m_impl->init(ml);
}

bool AttractionMapper::getGeoCoord(string attraction, GeoCoord& gc) const
{
	return m_impl->get_geo_coord(attraction, gc);
}
