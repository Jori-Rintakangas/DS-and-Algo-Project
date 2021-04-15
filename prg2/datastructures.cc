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
    auto name = std::find_if(name_range.first, name_range.second,//find correct multimap element
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
    auto coord = std::find_if(coord_range.first, coord_range.second,//find correct multimap element
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
    if ( id == parentid )
    {
        return false;
    }
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
    std::queue<std::shared_ptr<Area>> areas;
    areas.push(areas_.at(id));
    while ( !areas.empty() )
    {
        int amount = areas.size();
        while ( amount > 0 )
        {
            std::shared_ptr<Area> area = areas.front();
            areas.pop();
            if ( area->id != id )
            {
                sub_areas.push_back(area->id);
            }
            for ( unsigned long long i = 0; i < area->sub_areas.size(); i++ )
            {
                areas.push(area->sub_areas.at(i));
            }
            amount--;
        }
    }
    return sub_areas;
}

AreaID Datastructures::common_area_of_subareas(AreaID id1, AreaID id2)
{
    if ( id1 == id2 )
    {
        return NO_AREA;
    }
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

std::vector<WayID> Datastructures::all_ways()
{
    return way_ids_;
}

bool Datastructures::add_way(WayID id, std::vector<Coord> coords)
{
    if ( ways_.find(id) != ways_.end() )
    {
        return false;
    }
    auto coord_1 = crossroads_.find(coords.front());
    auto coord_2 = crossroads_.find(coords.back());
    std::shared_ptr<Way> way = std::make_shared<Way>(Way{coords, 0, false});
    std::shared_ptr<Way> way_ptr_1 = way;
    std::shared_ptr<Way> way_ptr_2 = way;
    std::vector<std::pair<std::shared_ptr<Way>,std::shared_ptr<Crossroad>>> neighbours;
    std::shared_ptr<Crossroad> crossroad = std::make_shared<Crossroad>
                   (Crossroad{neighbours, 0, 0, nullptr, nullptr});
    std::shared_ptr<Crossroad> crossroad_ptr_1 = crossroad;
    // if no crossroad exist in way starting and ending points
    if ( coord_1 == crossroads_.end() && coord_2 == crossroads_.end() )
    {
        std::shared_ptr<Crossroad> crossroad_end = std::make_shared<Crossroad>
                                 (Crossroad{neighbours, 0, 0, nullptr, nullptr});
        std::shared_ptr<Crossroad> crossroad_ptr_2 = crossroad;
        crossroad->neighbours.push_back({way_ptr_1, crossroad_ptr_1});
        crossroad_end->neighbours.push_back({way_ptr_2, crossroad_ptr_2});

        crossroads_.insert({coords.front(), crossroad});
        crossroads_.insert({coords.back(), crossroad_end});
    }
    else if ( coord_1 == crossroads_.end() )// if no crossroad in way starting point
    {
        std::shared_ptr<Crossroad> crossroad_ptr_2 = coord_2->second;
        crossroad->neighbours.push_back({way_ptr_1, crossroad_ptr_1});
        coord_2->second->neighbours.push_back({way_ptr_2, crossroad_ptr_2});
        crossroads_.insert({coords.front(), crossroad});
    }
    else if ( coord_2 == crossroads_.end() )// if no crossroad in way ending point
    {
        std::shared_ptr<Crossroad> crossroad_ptr_2 = coord_1->second;
        crossroad->neighbours.push_back({way_ptr_1, crossroad_ptr_1});
        coord_1->second->neighbours.push_back({way_ptr_2, crossroad_ptr_2});
        crossroads_.insert({coords.back(), crossroad});
    }
    else // if crossroads exist in way starting and ending points
    {
        std::shared_ptr<Crossroad> crossroad_ptr_1 = coord_1->second;
        std::shared_ptr<Crossroad> crossroad_ptr_2 = coord_2->second;
        coord_1->second->neighbours.push_back({way_ptr_2, crossroad_ptr_2});
        coord_2->second->neighbours.push_back({way_ptr_1, crossroad_ptr_1});
    }
    ways_.insert({id, way});
    way_ids_.push_back(id);
    return true;
}

std::vector<std::pair<WayID, Coord>> Datastructures::ways_from(Coord xy)
{
    // Replace this comment with your implementation
    return {{NO_WAY, NO_COORD}};
}

std::vector<Coord> Datastructures::get_way_coords(WayID id)
{
    auto way = ways_.find(id);
    if ( way != ways_.end() )
    {
        return way->second->coordinates;
    }
    return {NO_COORD};
}

void Datastructures::clear_ways()
{
    ways_.clear();
    crossroads_.clear();
    way_ids_.clear();
}

std::vector<std::tuple<Coord, WayID, Distance> > Datastructures::route_any(Coord fromxy, Coord toxy)
{
    // Replace this comment with your implementation
    return {{NO_COORD, NO_WAY, NO_DISTANCE}};
}

bool Datastructures::remove_way(WayID id)
{
    // Replace this comment with your implementation
    return false;
}

std::vector<std::tuple<Coord, WayID, Distance> > Datastructures::route_least_crossroads(Coord fromxy, Coord toxy)
{
    // Replace this comment with your implementation
    return {{NO_COORD, NO_WAY, NO_DISTANCE}};
}

std::vector<std::tuple<Coord, WayID> > Datastructures::route_with_cycle(Coord fromxy)
{
    // Replace this comment with your implementation
    return {{NO_COORD, NO_WAY}};
}

std::vector<std::tuple<Coord, WayID, Distance> > Datastructures::route_shortest_distance(Coord fromxy, Coord toxy)
{
    // Replace this comment with your implementation
    return {{NO_COORD, NO_WAY, NO_DISTANCE}};
}

Distance Datastructures::trim_ways()
{
    // Replace this comment with your implementation
    return NO_DISTANCE;
}
