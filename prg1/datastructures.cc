// Datastructures.cc

#include "datastructures.hh"

#include <random>

#include <cmath>

std::minstd_rand rand_engine; // Reasonably quick pseudo-random generator

template <typename Type>
Type random_in_range(Type start, Type end)
{
    auto range = end-start;
    ++range;

    auto num = std::uniform_int_distribution<unsigned long int>(0, range-1)(rand_engine);

    return static_cast<Type>(start+num);
}

// Modify the code below to implement the functionality of the class.
// Also remove comments from the parameter names when you implement
// an operation (Commenting out parameter name prevents compiler from
// warning about unused parameters on operations you haven't yet implemented.)

Datastructures::Datastructures()
{
    // Replace this comment with your implementation
}

Datastructures::~Datastructures()
{
    // Replace this comment with your implementation
}

int Datastructures::place_count()
{
    return places_.size();
}

void Datastructures::clear_all()
{
    places_.clear();
    alphabetically_.clear();
    coord_order_.clear();
    area_list_.clear();
    areas_.clear();
}

std::vector<PlaceID> Datastructures::all_places()
{
    return alphabetically_;
}

bool Datastructures::add_place(PlaceID id, const Name& name, PlaceType type, Coord xy)
{
    if ( places_.find(id) == places_.end() )
    {
        alphabet_sorted_ = false;
        coord_sorted_ = false;
        std::shared_ptr<Place> info(new Place{name, type, xy});
        auto value = places_.insert(last_added_, {id, info});
        last_added_ = value;
        alphabetically_.push_back(id);
        coord_order_.push_back(id);
        return true;
    }
    return false;
}

std::pair<Name, PlaceType> Datastructures::get_place_name_type(PlaceID id)
{
    if ( places_.find(id) != places_.end() )
    {
        Place info = *places_.at(id);
        return {info.place_name, info.place_type};
    }
    return {NO_NAME, PlaceType::NO_TYPE};
}

Coord Datastructures::get_place_coord(PlaceID id)
{
    if ( places_.find(id) != places_.end() )
    {
        Place info = *places_.at(id);
        return info.coordinate;
    }
    return NO_COORD;
}

bool Datastructures::add_area(AreaID id, const Name &name, std::vector<Coord> coords)
{
    if ( areas_.find(id) == areas_.end() )
    {
        std::shared_ptr<Area> info(new Area{name, coords, nullptr, nullptr});
        areas_.insert({id, info});
        area_list_.push_back(id);
        return true;
    }
    return false;
}

Name Datastructures::get_area_name(AreaID id)
{
    auto it = areas_.find(id);
    if ( it != areas_.end() )
    {
        return (*it->second).area_name;
    }
    return NO_NAME;
}

std::vector<Coord> Datastructures::get_area_coords(AreaID id)
{
    auto it = areas_.find(id);
    if ( it != areas_.end() )
    {
        return (*it->second).coords;
    }
    return {NO_COORD};
}

void Datastructures::creation_finished()
{
    // Replace this comment with your implementation
    // NOTE!! It's quite ok to leave this empty, if you don't need operations
    // that are performed after all additions have been done.
}

std::vector<PlaceID> Datastructures::places_alphabetically()
{
    if ( alphabet_sorted_ == false )
    {
        std::sort(alphabetically_.begin(), alphabetically_.end(),
        [=](PlaceID a, PlaceID b){return (*places_.at(a)).place_name < (*places_.at(b)).place_name;});
        alphabet_sorted_ = true;
    }
    return alphabetically_;
}

std::vector<PlaceID> Datastructures::places_coord_order()
{
    if ( coord_sorted_ == false )
    {
        std::sort(coord_order_.begin(), coord_order_.end(), [=](PlaceID a, PlaceID b)
        {
            return sqrt(pow((*places_.at(a)).coordinate.x, 2) + pow((*places_.at(a)).coordinate.y, 2)) <
                   sqrt(pow((*places_.at(b)).coordinate.x, 2) + pow((*places_.at(b)).coordinate.y, 2));
        });
        coord_sorted_ = true;
    }
    return coord_order_;
}

std::vector<PlaceID> Datastructures::find_places_name(Name const& name)
{
    std::vector<PlaceID> places_with_name = {};
    for ( auto &place : places_ )
    {
        if ( (place.second)->place_name == name )
        {
            places_with_name.push_back(place.first);
        }
    }
    return places_with_name;
}

std::vector<PlaceID> Datastructures::find_places_type(PlaceType type)
{
    std::vector<PlaceID> places_with_type = {};
    for ( auto &place : places_ )
    {
        if ( (place.second)->place_type == type )
        {
            places_with_type.push_back(place.first);
        }
    }
    return places_with_type;
}

bool Datastructures::change_place_name(PlaceID id, const Name& newname)
{
    if ( places_.find(id) != places_.end() )
    {
        (*places_.at(id)).place_name = newname;
        alphabet_sorted_ = false;
        return true;
    }
    return false;
}

bool Datastructures::change_place_coord(PlaceID id, Coord newcoord)
{
    if ( places_.find(id) != places_.end() )
    {
        (*places_.at(id)).coordinate = newcoord;
        coord_sorted_ = false;
        return true;
    }
    return false;
}

std::vector<AreaID> Datastructures::all_areas()
{
    return area_list_;
}

bool Datastructures::add_subarea_to_area(AreaID id, AreaID parentid)
{
    auto area = areas_.find(id);
    auto parent_a = areas_.find(parentid);
    if ( area != areas_.end() && parent_a != areas_.end() )
    {
        if ( (*area->second).parent_area == nullptr )
        {
            auto area_ptr = std::make_shared<std::pair<AreaID, std::shared_ptr<Area>>>(*area);
            auto parent_ptr = std::make_shared<std::pair<AreaID, std::shared_ptr<Area>>>(*parent_a);
            (*parent_a->second).sub_area = area_ptr;
            (*area->second).parent_area = parent_ptr;
            return true;
        }
        return false;
    }
    return false;
}

std::vector<AreaID> Datastructures::subarea_in_areas(AreaID id)
{
    if ( areas_.find(id) == areas_.end() )
    {
        return {NO_AREA};
    }
    std::vector<AreaID> areas = {};
    std::vector<AreaID> parent_areas = find_parent_areas(id, areas);
    return parent_areas;
}

std::vector<PlaceID> Datastructures::places_closest_to(Coord xy, PlaceType type)
{
    // Replace this comment with your implementation
    return {};
}

bool Datastructures::remove_place(PlaceID id)
{
    // Replace this comment with your implementation
    return false;
}

std::vector<AreaID> Datastructures::all_subareas_in_area(AreaID id)
{
    // Replace this comment with your implementation
    return {NO_AREA};
}

AreaID Datastructures::common_area_of_subareas(AreaID id1, AreaID id2)
{
    // Replace this comment with your implementation
    return NO_AREA;
}

std::vector<AreaID> Datastructures::find_parent_areas(AreaID id, std::vector<AreaID> areas)
{
    auto area = areas_.find(id);
    auto p_area = (*area->second).parent_area;
    if ( p_area != nullptr )
    {
        AreaID new_id = p_area->first;
        areas.push_back(new_id);
        return find_parent_areas(new_id, areas);
    }
    else
    {
        return areas;
    }
}

