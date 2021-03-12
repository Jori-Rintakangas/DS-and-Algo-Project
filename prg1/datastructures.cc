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
    // Replace this comment with your implementation
    return false;
}

Name Datastructures::get_area_name(AreaID id)
{
    // Replace this comment with your implementation
    return NO_NAME;
}

std::vector<Coord> Datastructures::get_area_coords(AreaID id)
{
    // Replace this comment with your implementation
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
    // Replace this comment with your implementation
    return false;
}

bool Datastructures::change_place_coord(PlaceID id, Coord newcoord)
{
    // Replace this comment with your implementation
    return false;
}

std::vector<AreaID> Datastructures::all_areas()
{
    // Replace this comment with your implementation
    return {};
}

bool Datastructures::add_subarea_to_area(AreaID id, AreaID parentid)
{
    // Replace this comment with your implementation
    return false;
}

std::vector<AreaID> Datastructures::subarea_in_areas(AreaID id)
{
    // Replace this comment with your implementation
    return {NO_AREA};
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

