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
    places_a_.clear();
    places_c_.clear();
    alphabetically_.clear();
    coord_order_.clear();
    area_list_.clear();
    areas_.clear();   
}

std::vector<PlaceID> Datastructures::all_places()
{
    std::vector<PlaceID> ids = {};
    for ( auto & place : places_ )
    {
        ids.push_back(place.first);
    }
    return ids;
}

bool Datastructures::add_place(PlaceID id, const Name& name, PlaceType type, Coord xy)
{
    if ( places_.find(id) == places_.end() )
    {
        alphabet_sorted_ = false;
        coord_sorted_ = false;
        std::shared_ptr<Place> info(new Place{id, name, type, xy});
        std::shared_ptr<Place> info_2 = info;
        std::shared_ptr<Place> info_3 = info;
        places_.insert(last_added_,{id, info});
        places_a_.insert({name, info_2});
        places_c_.insert({xy, info_3});
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
        std::vector<std::shared_ptr<Area>> sub_areas = {};
        std::shared_ptr<Area> info(new Area{id, name, coords, sub_areas, nullptr});
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
        alphabetically_.clear();
        for ( auto &place : places_a_ )
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
        for ( auto &place : places_c_ )
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
    auto it_pair = places_a_.equal_range(places_.at(id)->place_name);
    auto it = std::find_if(it_pair.first, it_pair.second,
              [=](auto id_info){return id_info.second->id == id;});
    if ( it != places_a_.end() )
    {
        (*places_.at(id)).place_name = newname;
        places_a_.erase(it);
        places_a_.insert({newname, places_.at(id)});
        alphabet_sorted_ = false;
        return true;
    }
    return false;
}

bool Datastructures::change_place_coord(PlaceID id, Coord newcoord)
{
    auto it_pair = places_c_.equal_range(places_.at(id)->coordinate);
    auto it = std::find_if(it_pair.first, it_pair.second,
              [=](auto id_info){return id_info.second->id == id;});
    if ( it != places_c_.end() )
    {
        (*places_.at(id)).coordinate = newcoord;
        places_c_.erase(it);
        places_c_.insert({newcoord, places_.at(id)});
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
    std::priority_queue<std::pair<double, PlaceID>,
            std::vector<std::pair<double, PlaceID>>,
            std::greater<std::pair<double, PlaceID>> > closest = {};
    bool t_specific = true;
    if ( type == PlaceType::NO_TYPE )
    {
        t_specific = false;
    }
    for ( auto &place : places_ )
    {
        double dist = pow((place.second->coordinate.x) - xy.x, 2) +
                      pow((place.second->coordinate.y) - xy.y, 2);
        if ( t_specific )
        {
            if ( place.second->place_type == type )
            {
                closest.push({dist, place.first});
            }
        }
        else
        {
            closest.push({dist, place.first});
        }
    }
    if ( closest.empty() )
    {
        return closest_places;
    }
    for ( int i = 1; i <= 3; ++i )
    {
        auto element = closest.top();
        closest_places.push_back(element.second);
        closest.pop();
        if ( closest.size() == 0 )
        {
            break;
        }
    }
    return closest_places;

}

bool Datastructures::remove_place(PlaceID id)
{
    auto it = places_.find(id);
    if ( it != places_.end() )
    {
        auto it_pair = places_a_.equal_range(places_.at(id)->place_name);
        auto it1 = std::find_if(it_pair.first, it_pair.second,
                  [=](auto id_info){return id_info.second->id == id;});
        places_a_.erase(it1);
        auto it_pair2 = places_c_.equal_range(places_.at(id)->coordinate);
        auto it2 = std::find_if(it_pair2.first, it_pair2.second,
                  [=](auto id_info){return id_info.second->id == id;});
        places_c_.erase(it2);
        places_.erase(it);
        coord_sorted_ = false;
        alphabet_sorted_ = false;
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
    if ( itr_1 == areas_.end() or itr_2 == areas_.end() )
    {
        return NO_AREA;
    }
    auto area_1 = itr_1->second;
    auto area_2 = itr_2->second;
    std::map<std::shared_ptr<Area>, bool> ancestors = {};
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
