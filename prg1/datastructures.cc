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
    places_name_order_.clear();
    places_coord_order_.clear();
    place_list_.clear();
    alphabetically_.clear();
    coord_order_.clear();
    area_list_.clear();
    areas_.clear();
}

std::vector<PlaceID> Datastructures::all_places()
{
    if ( places_valid_ )
    {
        return place_list_;
    }
    place_list_.clear();
    for ( auto &place : places_ )
    {
        place_list_.push_back(place.first);
    }
    places_valid_ = true;
    return place_list_;
}

bool Datastructures::add_place(PlaceID id, const Name& name, PlaceType type, Coord xy)
{
    if ( places_.find(id) == places_.end() )
    {
        std::shared_ptr<Place> info = std::make_shared<Place>(Place {id, name, type, xy});
        std::shared_ptr<Place> info_2 = info;
        std::shared_ptr<Place> info_3 = info;
        places_.insert({id, info});
        places_name_order_.insert({name, info_2});
        places_coord_order_.insert({xy, info_3});
        alphabet_sorted_ = false;
        coord_sorted_ = false;
        places_valid_ = false;
        return true;
    }
    return false;
}

std::pair<Name, PlaceType> Datastructures::get_place_name_type(PlaceID id)
{
    auto place = places_.find(id);
    if ( place != places_.end() )
    {
        return {(*place->second).place_name, (*place->second).place_type};
    }
    return {NO_NAME, PlaceType::NO_TYPE};
}

Coord Datastructures::get_place_coord(PlaceID id)
{
    auto place = places_.find(id);
    if ( place != places_.end() )
    {
        return (*place->second).coordinate;
    }
    return NO_COORD;
}

bool Datastructures::add_area(AreaID id, const Name &name, std::vector<Coord> coords)
{
    if ( areas_.find(id) == areas_.end() )
    {
        std::vector<std::shared_ptr<Area>> sub_areas = {};
        std::shared_ptr<Area> info = std::make_shared<Area>(Area{id, name, coords, sub_areas, nullptr});
        areas_.insert({id, info});
        area_list_.push_back(id);
        return true;
    }
    return false;
}

Name Datastructures::get_area_name(AreaID id)
{
    auto area = areas_.find(id);
    if ( area != areas_.end() )
    {
        return (*area->second).area_name;
    }
    return NO_NAME;
}

std::vector<Coord> Datastructures::get_area_coords(AreaID id)
{
    auto area = areas_.find(id);
    if ( area != areas_.end() )
    {
        return (*area->second).coords;
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
        alphabetically_.clear();
        for ( auto &place : places_name_order_ )
        {
            alphabetically_.push_back(place.second->id);
        }
        alphabet_sorted_ = true;
        return alphabetically_;
    }
    return alphabetically_;
}

std::vector<PlaceID> Datastructures::places_coord_order()
{
    if ( coord_sorted_ == false )
    {
        coord_order_.clear();
        for ( auto &place : places_coord_order_ )
        {
            coord_order_.push_back(place.second->id);
        }
        coord_sorted_ = true;
        return coord_order_;
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
    auto name_range = places_name_order_.equal_range(places_.at(id)->place_name);
    auto name = std::find_if(name_range.first, name_range.second,//find correct multimap elements
                [=](auto id_info){return id_info.second->id == id;});
    if ( name != places_name_order_.end() )
    {
        (*places_.at(id)).place_name = newname;
        places_name_order_.erase(name);
        places_name_order_.insert({newname, places_.at(id)});
        alphabet_sorted_ = false;
        return true;
    }
    return false;
}

bool Datastructures::change_place_coord(PlaceID id, Coord newcoord)
{
    auto coord_range = places_coord_order_.equal_range(places_.at(id)->coordinate);
    auto coord = std::find_if(coord_range.first, coord_range.second,//find correct multimap elements
                 [=](auto id_info){return id_info.second->id == id;});
    if ( coord != places_coord_order_.end() )
    {
        (*places_.at(id)).coordinate = newcoord;
        places_coord_order_.erase(coord);
        places_coord_order_.insert({newcoord, places_.at(id)});
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
            auto area_ptr = area->second;
            auto parent_ptr = parent_a->second;
            (*parent_a->second).sub_areas.push_back(area_ptr);
            (*area->second).parent_area = parent_ptr;
            return true;
        }
        return false;
    }
    return false;
}

std::vector<AreaID> Datastructures::subarea_in_areas(AreaID id)
{
    auto area = areas_.find(id);
    if ( area == areas_.end() )
    {
        return {NO_AREA};
    }
    std::vector<AreaID> sub_areas = {};
    auto p_area = (*area->second).parent_area;
    while ( p_area != nullptr )
    {
        AreaID new_id = p_area->id;
        sub_areas.push_back(new_id);
        p_area = p_area->parent_area;
    }
    return sub_areas;
}

std::vector<PlaceID> Datastructures::places_closest_to(Coord xy, PlaceType type)
{
    std::vector<PlaceID> closest_places = {};
    std::priority_queue<std::pair<Coord, PlaceID>,
            std::vector<std::pair<Coord, PlaceID>> >closest = {};
    bool type_specific = true;
    if ( type == PlaceType::NO_TYPE )
    {
        type_specific = false;
    }
    for ( auto &place : places_ )
    {   // scaling place coordinates to determine distance by comparing them with each other
        int new_x = (*place.second).coordinate.x - xy.x;
        int new_y = (*place.second).coordinate.y - xy.y;
        Coord new_xy = {new_x, new_y};
        if ( place.second->place_type != type )
        {
            if ( type_specific )
            {
                continue;
            }
        }
        if ( closest.size() == MAX_AMOUNT )
        {
            if ( new_xy < closest.top().first )
            {
                closest.push({new_xy, (*place.second).id});
                closest.pop();
            }
        }
        else
        {
            closest.push({new_xy, (*place.second).id});
        }
    }
    if ( closest.empty() )
    {
        return closest_places;
    }
    for ( int i = 1; i <= MAX_AMOUNT; ++i )
    {
        auto element = closest.top();
        closest_places.push_back(element.second);
        closest.pop();
        if ( closest.empty() )
        {
            break;
        }
    }
    //needed since max-heap used
    std::reverse(closest_places.begin(), closest_places.end());
    return closest_places;
}

bool Datastructures::remove_place(PlaceID id)
{
    auto place = places_.find(id);
    if ( place != places_.end() )
    {
        auto name_range = places_name_order_.equal_range(places_.at(id)->place_name);
        auto coord_range = places_coord_order_.equal_range(places_.at(id)->coordinate);
        // find correct multimap elements
        auto name = std::find_if(name_range.first, name_range.second,
                    [=](auto id_info){return id_info.second->id == id;});
        auto coord = std::find_if(coord_range.first, coord_range.second,
                     [=](auto id_info){return id_info.second->id == id;});
        places_name_order_.erase(name);
        places_coord_order_.erase(coord);
        places_.erase(place);
        coord_sorted_ = false;
        alphabet_sorted_ = false;
        places_valid_ = false;
        return true;
    }
    return false;
}

std::vector<AreaID> Datastructures::all_subareas_in_area(AreaID id)
{
    std::vector<AreaID> sub_areas = {};
    auto area = areas_.find(id);
    if ( area == areas_.end() )
    {
        return {NO_AREA};
    }
    std::queue<std::shared_ptr<Area>> q;
    q.push(areas_.at(id));
    while (!q.empty())
    {
        int n = q.size();

        while (n > 0)
        {
            std::shared_ptr<Area> area = q.front();
            q.pop();
            if ( area->id != id )
            {
                sub_areas.push_back(area->id);
            }
            for ( unsigned long long i = 0; i < area->sub_areas.size(); i++ )
            {
                q.push(area->sub_areas.at(i));
            }
            n--;
        }
    }
    return sub_areas;
}

AreaID Datastructures::common_area_of_subareas(AreaID id1, AreaID id2)
{
    auto itr_1 = areas_.find(id1);
    auto itr_2 = areas_.find(id2);
    if ( itr_1 == areas_.end() || itr_2 == areas_.end() )
    {
        return NO_AREA;
    }
    auto area_1 = itr_1->second->parent_area;
    auto area_2 = itr_2->second->parent_area;
    std::unordered_map<std::shared_ptr<Area>, bool> ancestors = {};
    while ( area_1 != nullptr )
    {
        ancestors[area_1] = true;
        area_1 = area_1->parent_area;
    }
    while ( area_2 != nullptr )
       {
           if ( ancestors.find(area_2) != ancestors.end() )
           {
               return area_2->id;
           }
           area_2 = area_2->parent_area;
       }
    return NO_AREA;
}
