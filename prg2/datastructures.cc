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
    for ( auto &way : ways_ )
    {
        delete way.second;
    }
    for ( auto &crossroad : crossroads_ )
    {
        delete crossroad.second;
    }
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
    std::vector<WayID> ways = {};
    for ( auto &w : ways_ )
    {
        ways.push_back(w.first);
    }
    return ways;
}

bool Datastructures::add_way(WayID id, std::vector<Coord> coords)
{
    if ( ways_.find(id) != ways_.end() )
    {
        return false;
    }
    auto coord_1 = crossroads_.find(coords.front());
    auto coord_2 = crossroads_.find(coords.back());
    Way* way_info = new Way{id, coords, std::numeric_limits<int>::max(), false};
    Way* way = way_info;
    Way* way_ptr_1 = way_info;
    Way* way_ptr_2 = way_info;

    std::vector<std::pair<Way*,Crossroad*>> neighbours;

    Crossroad* crossroad_info = new Crossroad{coords.front(), neighbours, 0, 0, WHITE, {}, false};
    Crossroad* crossroad = crossroad_info;
    Crossroad* crossroad_ptr_1 = crossroad_info;

    // if no crossroad exist in way starting and ending points
    if ( coord_1 == crossroads_.end() && coord_2 == crossroads_.end() )
    {
        if ( coords.front() == coords.back() )
        {
            crossroad->neighbours.push_back({way_ptr_2, crossroad_ptr_1});
            crossroads_.insert({coords.front(), crossroad});
        }
        else
        {
            Crossroad* crossroad_end_info = new Crossroad{coords.back(), neighbours, 0, 0, WHITE, {}, false};
            Crossroad* crossroad_end = crossroad_end_info;
            Crossroad* crossroad_ptr_2 = crossroad_end_info;
            crossroad->neighbours.push_back({way_ptr_1, crossroad_ptr_2});
            crossroad_end->neighbours.push_back({way_ptr_2, crossroad_ptr_1});
            crossroads_.insert({coords.front(), crossroad});
            crossroads_.insert({coords.back(), crossroad_end});
        }
    }
    else if ( coord_1 == crossroads_.end() )// if no crossroad in way starting point
    {
        Crossroad* crossroad_ptr_2 = coord_2->second;
        crossroad->neighbours.push_back({way_ptr_1, crossroad_ptr_2});
        coord_2->second->neighbours.push_back({way_ptr_2, crossroad_ptr_1});
        crossroad->location = coords.front();
        crossroads_.insert({coords.front(), crossroad});
    }
    else if ( coord_2 == crossroads_.end() )// if no crossroad in way ending point
    {
        Crossroad* crossroad_ptr_2 = coord_1->second;
        crossroad->neighbours.push_back({way_ptr_1, crossroad_ptr_2});
        coord_1->second->neighbours.push_back({way_ptr_2, crossroad_ptr_1});
        crossroad->location = coords.back();
        crossroads_.insert({coords.back(), crossroad});
    }
    else // if crossroads exist in way starting and ending points
    {
        Crossroad* crossroad_ptr_1 = coord_1->second;
        if ( coords.front() == coords.back() )
        {
            coord_1->second->neighbours.push_back({way_ptr_2, crossroad_ptr_1});
        }
        else
        {
            Crossroad* crossroad_ptr_2 = coord_2->second;
            coord_1->second->neighbours.push_back({way_ptr_2, crossroad_ptr_2});
            coord_2->second->neighbours.push_back({way_ptr_1, crossroad_ptr_1});
        }
        delete crossroad_info;

    }
    ways_.insert({id, way});
    return true;
}

std::vector<std::pair<WayID, Coord>> Datastructures::ways_from(Coord xy)
{
    std::vector<std::pair<WayID, Coord>> ways_from_crossroad = {};
    auto crossroad = crossroads_.find(xy);
    if ( crossroad != crossroads_.end() )
    {
        for ( auto &crossroad : crossroads_.at(xy)->neighbours )
        {
            ways_from_crossroad.push_back({crossroad.first->id, crossroad.second->location});
        }
    }
    return ways_from_crossroad;
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
    for ( auto &way : ways_ )
    {
        delete way.second;
    }
    for ( auto &crossroad : crossroads_ )
    {
        delete crossroad.second;
    }
    ways_.clear();
    crossroads_.clear();
}

std::vector<std::tuple<Coord, WayID, Distance>> Datastructures::route_any(Coord fromxy, Coord toxy)
{
    auto coord_1 = crossroads_.find(fromxy);
    auto coord_2 = crossroads_.find(toxy);
    if ( coord_1 == crossroads_.end() || coord_2 == crossroads_.end() )
    {
        return {{NO_COORD, NO_WAY, NO_DISTANCE}};
    }
    route_.clear();
    if ( !crossroads_clear )
    {
        reset_crossroads();
    }
    std::queue<std::pair<Way*, Crossroad*>> queue = {};
    queue.push({nullptr, crossroads_.at(fromxy)});
    std::pair<Way*, Crossroad*> crossroad;
    bool route_found = false;
    while ( !queue.empty() )
    {
        crossroad = queue.front();
        queue.pop();
        if ( crossroad.second->location == toxy )
        {
            route_found = true;
            break;
        }
        for ( auto &neighbour : crossroad.second->neighbours )
        {
            if ( neighbour.second->colour == WHITE )
            {
                crossroad.first = neighbour.first;
                neighbour.second->arrived_from = crossroad;
                neighbour.second->colour = GREY;
                queue.push(neighbour);
            }
        }
        crossroad.second->colour = BLACK;
    }
    if ( route_found )
    {
        store_path(fromxy, toxy, 0, NO_WAY);
        dist_so_far_ = 0;
    }
    crossroads_clear = false;
    return route_;
}

bool Datastructures::remove_way(WayID id)
{
    auto way = ways_.find(id);
    if ( way != ways_.end() )
    {
        Crossroad* crossroad_begin = crossroads_.at(way->second->coordinates.front());
        Crossroad* crossroad_end = crossroads_.at(way->second->coordinates.back());

        crossroad_begin->neighbours.erase(std::remove_if(crossroad_begin->neighbours.begin(),
        crossroad_begin->neighbours.end(),
        [id](std::pair<Way*, Crossroad*> p){return p.first->id == id;}),
        crossroad_begin->neighbours.end());

        crossroad_end->neighbours.erase(std::remove_if(crossroad_end->neighbours.begin(),
        crossroad_end->neighbours.end(),
        [id](std::pair<Way*, Crossroad*> p){return p.first->id == id;}),
        crossroad_end->neighbours.end());
        if ( crossroad_begin->neighbours.size() == 0 )
        {
            crossroads_.erase(crossroad_begin->location);
            delete crossroad_begin;
        }
        if ( crossroad_end->neighbours.size() == 0 )
        {
            crossroads_.erase(crossroad_end->location);
            delete crossroad_end;
        }
        delete ways_.at(id);
        ways_.erase(id);

        return true;
    }
    return false;
}

std::vector<std::tuple<Coord, WayID, Distance> > Datastructures::route_least_crossroads(Coord fromxy, Coord toxy)
{
    auto coord_1 = crossroads_.find(fromxy);
    auto coord_2 = crossroads_.find(toxy);
    if ( coord_1 == crossroads_.end() || coord_2 == crossroads_.end() )
    {
        return {{NO_COORD, NO_WAY, NO_DISTANCE}};
    }
    route_.clear();
    if ( !crossroads_clear )
    {
        reset_crossroads();
    }
    std::queue<std::pair<Way*, Crossroad*>> queue = {};
    queue.push({nullptr, crossroads_.at(fromxy)});
    std::pair<Way*, Crossroad*> crossroad;
    bool route_found = false;
    while ( !queue.empty() )
    {
        crossroad = queue.front();
        queue.pop();
        if ( crossroad.second->location == toxy )
        {
            route_found = true;
            break;
        }
        for ( auto &neighbour : crossroad.second->neighbours )
        {
            if ( neighbour.second->colour == WHITE )
            {
                crossroad.first = neighbour.first;
                neighbour.second->arrived_from = crossroad;
                neighbour.second->colour = GREY;
                neighbour.second->steps_from_start += 1;
                queue.push(neighbour);
            }
        }
        crossroad.second->colour = BLACK;
    }
    if ( route_found )
    {
        store_path(fromxy, toxy, 0, NO_WAY);
        dist_so_far_ = 0;
    }
    crossroads_clear = false;
    return route_;
}

std::vector<std::tuple<Coord, WayID>> Datastructures::route_with_cycle(Coord fromxy)
{
    auto coord = crossroads_.find(fromxy);
    if ( coord == crossroads_.end() )
    {
        return {{NO_COORD, NO_WAY}};
    }
    cycle_route_.clear();
    if ( !crossroads_clear )
    {
        reset_crossroads();
    }
    std::pair<Way*, Crossroad*> crossroad = {nullptr, crossroads_.at(fromxy)};
    execute_dfs_search(crossroad);
    if ( cycle_found_ )
    {
        std::reverse(cycle_route_.begin(), cycle_route_.end());
        cycle_route_.push_back({cycle_coord_, NO_WAY});
    }
    cycle_found_ = false;
    crossroads_clear = false;
    return cycle_route_;
}

std::vector<std::tuple<Coord, WayID, Distance> > Datastructures::route_shortest_distance(Coord fromxy, Coord toxy)
{
    auto coord_1 = crossroads_.find(fromxy);
    auto coord_2 = crossroads_.find(toxy);
    if ( coord_1 == crossroads_.end() || coord_2 == crossroads_.end() )
    {
        return {{NO_COORD, NO_WAY, NO_DISTANCE}};
    }
    route_.clear();
    if ( !crossroads_clear )
    {
        reset_crossroads();
    }
    auto comp = [](std::pair<Way*, Crossroad*> a,
                   std::pair<Way*, Crossroad*> b)
                   {return a.first->length > b.first->length;};

    std::priority_queue<std::pair<Way*, Crossroad*>,
    std::vector<std::pair<Way*, Crossroad*>>, decltype(comp)> p_queue(comp);

    crossroads_.at(fromxy)->colour = GREY;
    crossroads_.at(fromxy)->dist_from_start = 0;
    p_queue.push({nullptr, crossroads_.at(fromxy)});

    std::pair<Way*, Crossroad*> crossroad;
    bool route_found = false;
    while ( !p_queue.empty() )
    {
        crossroad = p_queue.top();
        p_queue.pop();
        if ( crossroad.second->colour == BLACK )
        {
            continue;
        }
        if ( crossroad.second->location == toxy )
        {
            route_found = true;
            break;
        }
        for ( auto &neighbour : crossroad.second->neighbours )
        {
            neighbour.first->length = calculate_way_length(neighbour.first);
            Distance d = neighbour.first->length;
            Crossroad* n = neighbour.second;
            crossroad.first = neighbour.first;
            if ( neighbour.second->colour == WHITE )
            {
                neighbour.second->colour = GREY;
                p_queue.push(neighbour);
            }
            if ( n->dist_from_start > crossroad.second->dist_from_start + d )
            {
                n->dist_from_start = crossroad.second->dist_from_start + d;
                n->arrived_from = crossroad;
            }
        }
        crossroad.second->colour = BLACK;
    }
    if ( route_found )
    {
        store_path(fromxy, toxy, 0, NO_WAY);
        dist_so_far_ = 0;
    }
    crossroads_clear = false;
    return route_;
}

Distance Datastructures::trim_ways()
{
    if ( !crossroads_clear )
    {
        reset_crossroads();
    }
    auto crossroads_to_check = crossroads_;
    auto comp = [](std::pair<Way*, Crossroad*> a,
                   std::pair<Way*, Crossroad*> b)
                   {return a.first->length > b.first->length;};
    std::vector<WayID> ways_to_remove = {};
    std::priority_queue<std::pair<Way*, Crossroad*>,
    std::vector<std::pair<Way*, Crossroad*>>, decltype(comp)> p_queue(comp);

    auto start_crossroad = crossroads_.begin()->second;
    start_crossroad->colour = GREY;
    start_crossroad->dist_from_start = 0;
    p_queue.push({nullptr, start_crossroad});
    std::pair<Way*, Crossroad*> crossroad;
    Distance mst_length = 0;
    bool first = true;
    while ( !p_queue.empty() )
    {
        crossroad = p_queue.top();
        p_queue.pop();
        crossroads_to_check.erase(crossroad.second->location);
        Distance cost = 0;
        if ( first )
        {
            cost = 0;
            first = false;
        }
        else
        {
            cost = crossroad.first->length;
        }
        if ( crossroad.second->in_mst == false )
        {
            mst_length += cost;
            crossroad.second->in_mst = true;
            for ( auto &n : crossroad.second->neighbours )
            {
                if ( n.second->in_mst == false )
                {
                    n.first->length = calculate_way_length(n.first);
                    p_queue.push(n);
                }
                else if ( crossroad.first->id != n.first->id )
                {
                    ways_to_remove.push_back(n.first->id);
                }
            }
        }
        if ( p_queue.empty() && !crossroads_to_check.empty() )
        {
            auto start_crossroad = crossroads_to_check.begin()->second;
            start_crossroad->colour = GREY;
            start_crossroad->dist_from_start = 0;
            p_queue.push({nullptr, start_crossroad});
            first = true;
        }
    }
    for ( auto &way : ways_to_remove )
    {
        remove_way(way);
    }
    crossroads_clear = false;
    return mst_length;
}

void Datastructures::store_path(Coord fromxy, Coord toxy, Distance dist, WayID id)
{
    if ( crossroads_.at(fromxy) == crossroads_.at(toxy) )
    {
        route_.push_back({fromxy, id, dist});
    }
    else if ( crossroads_.at(toxy)->arrived_from.second == nullptr )
    {
        return;
    }
    else
    {
        if ( crossroads_.at(toxy)->arrived_from.first == nullptr )
        {
            store_path(fromxy, crossroads_.at(toxy)->arrived_from.second->location,
                    dist, id);
            route_.push_back({toxy, id, dist_so_far_});
        }
        else
        {
            Distance distance = calculate_way_length(ways_.at(crossroads_.at(toxy)->arrived_from.first->id));
            store_path(fromxy, crossroads_.at(toxy)->arrived_from.second->location,
                    dist, crossroads_.at(toxy)->arrived_from.first->id);
            dist_so_far_ += distance;
            route_.push_back({toxy, id, dist_so_far_});
        }
    }
}

Distance Datastructures::calculate_way_length(Way* way)
{
    Distance dist = 0;
    std::vector<Coord> v = way->coordinates;
    for ( unsigned long long int i = 0; i < v.size() - 1; ++i )
    {
        dist += std::floor(sqrt(pow((v.at(i).x - v.at(i+1).x), 2) +
                               pow((v.at(i).y - v.at(i+1).y), 2)));
    }
    return dist;
}

void Datastructures::reset_crossroads()
{
    for ( auto &crossroad : crossroads_ )
    {
        crossroad.second->arrived_from = {};
        crossroad.second->colour = WHITE;
        crossroad.second->steps_from_start = 0;
        crossroad.second->dist_from_start = std::numeric_limits<int>::max();
        crossroad.second->in_mst = false;
    }
    crossroads_clear = true;
}

void Datastructures::execute_dfs_search(std::pair<Way*, Crossroad*> crossroad)
{
    crossroad.second->colour = GREY;
    for ( auto &neighbour : crossroad.second->neighbours )
    {
        if ( cycle_found_ )
        {
            return;
        }
        if ( neighbour.second->colour == WHITE )
        {
            crossroad.first = neighbour.first;
            neighbour.second->arrived_from = crossroad;
            execute_dfs_search(neighbour);
            if ( cycle_found_ )
            {
                cycle_route_.push_back({crossroad.second->location, crossroad.first->id});
            }
        }
        else if ( neighbour.second->colour == GREY && crossroad.second->arrived_from.first != neighbour.first )
        {
            cycle_coord_ = neighbour.second->location;
            cycle_found_ = true;
            cycle_route_.push_back({crossroad.second->location, neighbour.first->id});
        }
    }
    crossroad.second->colour = BLACK;
}


