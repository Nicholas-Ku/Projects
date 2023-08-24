#include "MyMap.h"
#include "Support.h"
#include "Provided.h"

#include <vector>
using namespace std;

class SegmentMapperImpl
{
public:
	SegmentMapperImpl();
	~SegmentMapperImpl();
	void init(const MapLoader& ml);
	vector<StreetSegment> get_segments(const GeoCoord& gc) const;

private:
	MyMap<GeoCoord, vector<StreetSegment>> seg_map_;
};

///////////////////////////////////////////////////////////////////////////
//  Implementation
///////////////////////////////////////////////////////////////////////////

SegmentMapperImpl::SegmentMapperImpl()
{

}

SegmentMapperImpl::~SegmentMapperImpl()
{

}

void SegmentMapperImpl::init(const MapLoader& ml)
{
	const auto num_segments = ml.getNumSegments();
	for (int i = 0; i < num_segments; i++) {
		StreetSegment current_seg;
		ml.getSegment(i, current_seg);
		
		// If segment's start coordinates are not in map
		vector<StreetSegment>* found_seg_start = seg_map_.find(current_seg.segment.start);
		if (found_seg_start == nullptr) {
			// Add to map
			vector<StreetSegment> seg_vector;
			seg_vector.push_back(current_seg);
			seg_map_.associate(current_seg.segment.start, seg_vector);
		}
		else {
			found_seg_start->push_back(current_seg);
		}

		// If segment's end coordinates are not in map
		vector<StreetSegment>* found_seg_end = seg_map_.find(current_seg.segment.end);
		if (found_seg_end == nullptr) {
			vector<StreetSegment> seg_vector;
			seg_vector.push_back(current_seg);
			seg_map_.associate(current_seg.segment.start, seg_vector);
		}
		else {
			found_seg_end->push_back(current_seg);
		}

		// Check all attractions
		vector<Attraction> attr_vector = current_seg.attractions;
		for (int i = 0; i < attr_vector.size(); i++) {
			vector<StreetSegment>* found_attr = seg_map_.find(attr_vector[i].geocoordinates);

			// If it's not in the map
			if (found_attr == nullptr) {
				vector<StreetSegment> seg_vector;
				seg_vector.push_back(current_seg);
				seg_map_.associate(attr_vector[i].geocoordinates, seg_vector);
			}

			// If it is in the map already
			else {
				found_attr->push_back(current_seg);
			}
		}
	}
}

vector<StreetSegment> SegmentMapperImpl::get_segments(const GeoCoord& gc) const
{
	if (seg_map_.find(gc) != nullptr) {
		return *(seg_map_.find(gc));
	}
	
	vector<StreetSegment> empty;
	return empty;
}

///////////////////////////////////////////////////////////////////////////
//  SegmentMapper Functions
///////////////////////////////////////////////////////////////////////////

SegmentMapper::SegmentMapper()
{
	m_impl = new SegmentMapperImpl;
}

SegmentMapper::~SegmentMapper()
{
	delete m_impl;
}

void SegmentMapper::init(const MapLoader& ml)
{
	m_impl->init(ml);
}

vector<StreetSegment> SegmentMapper::getSegments(const GeoCoord& gc) const
{
	return m_impl->get_segments(gc);
}
