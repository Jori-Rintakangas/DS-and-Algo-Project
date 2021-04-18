// Datastructures.hh

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <string>
#include <vector>
#include <tuple>
#include <utility>
#include <limits>
#include <functional>
#include <memory>
#include <algorithm>
#include <map>
#include <QDebug>
#include <cmath>
#include <queue>
#include <QSharedPointer>
#include <stack>
#include <iostream>
// Types for IDs
using PlaceID = long long int;
using AreaID = long long int;
using Name = std::string;
using WayID = std::string;

// Return values for cases where required thing was not found
PlaceID const NO_PLACE = -1;
AreaID const NO_AREA = -1;
WayID const NO_WAY = "!!No way!!";

// Return value for cases where integer values were not found
int const NO_VALUE = std::numeric_limits<int>::min();

// Return value for cases where name values were not found
Name const NO_NAME = "!!NO_NAME!!";

int const MAX_AMOUNT = 3;
int const WHITE = 1;
int const GREY = 0;
int const BLACK = -1;
// Enumeration for different place types
// !!Note since this is a C++11 "scoped enumeration", you'll have to refer to
// individual values as PlaceType::SHELTER etc.
enum class PlaceType { OTHER=0, FIREPIT, SHELTER, PARKING, PEAK, BAY, AREA, NO_TYPE };

// Type for a coordinate (x, y)
struct Coord
{
    int x = NO_VALUE;
    int y = NO_VALUE;
};

// Example: Defining == and hash function for Coord so that it can be used
// as key for std::unordered_map/set, if needed
inline bool operator==(Coord c1, Coord c2) { return c1.x == c2.x && c1.y == c2.y; }
inline bool operator!=(Coord c1, Coord c2) { return !(c1==c2); } // Not strictly necessary

struct CoordHash
{
    std::size_t operator()(Coord xy) const
    {
        auto hasher = std::hash<int>();
        auto xhash = hasher(xy.x);
        auto yhash = hasher(xy.y);
        // Combine hash values (magic!)
        return xhash ^ (yhash + 0x9e3779b9 + (xhash << 6) + (xhash >> 2));
    }
};

// Example: Defining < for Coord so that it can be used
// as key for std::map/set
inline bool operator<(Coord c1, Coord c2)
{
    int first = pow(c1.x, 2) + pow(c1.y, 2);
    int second = pow(c2.x, 2) + pow(c2.y, 2);
    if ( first == second )
    {
        return c1.y < c2.y;
    }
    return first < second;
}

// Return value for cases where coordinates were not found
Coord const NO_COORD = {NO_VALUE, NO_VALUE};

// Type for a distance (in metres)
using Distance = int;

// Return value for cases where Duration is unknown
Distance const NO_DISTANCE = NO_VALUE;



// This is the class you are supposed to implement

class Datastructures
{
public:
    Datastructures();
    ~Datastructures();

    // Estimate of performance: Theta(1)
    // Short rationale for estimate: size()-opearation complexity constant
    int place_count();

    // Estimate of performance: Theta(n+m), n == number of places m == number of areas
    // Short rationale for estimate: clear()-operation complexity Theta(n)
    void clear_all();

    // Estimate of performance: Best: Theta(1), Worst: O(n)
    // Short rationale for estimate: Best if all places in vector, worst if need to traverse all places.
    std::vector<PlaceID> all_places();

    // Estimate of performance: Best: Theta(1), Worst: O(n)
    // Short rationale for estimate: Best if place not added, unordered_map find() is Theta(1)
    // on average. Worst if unordered_map insert() or find() takes O(n).
    bool add_place(PlaceID id, Name const& name, PlaceType type, Coord xy);

    // Estimate of performance: Average: Theta(1), Worst: O(n)
    // Short rationale for estimate: unordered_map find()-operation.
    std::pair<Name, PlaceType> get_place_name_type(PlaceID id);

    // Estimate of performance: Average: Theta(1), Worst: O(n)
    // Short rationale for estimate: unordered_map find()-operation.
    Coord get_place_coord(PlaceID id);

    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: Best: Theta(1), Worst: O(n)
    // Short rationale for estimate: Best if no need to sort, worst when traversing all places.
    std::vector<PlaceID> places_alphabetically();

    // Estimate of performance: Best: Theta(1), Worst: O(n)
    // Short rationale for estimate: Best if no need to sort, worst when traversing all places.
    std::vector<PlaceID> places_coord_order();

    // Estimate of performance: Theta(n)
    // Short rationale for estimate: Always traversing all places.
    std::vector<PlaceID> find_places_name(Name const& name);

    // Estimate of performance: Theta(n)
    // Short rationale for estimate: Always traversing all places.
    std::vector<PlaceID> find_places_type(PlaceType type);

    // Estimate of performance: Average O(m + log n), Worst: O(n)
    // m == number of places with same name, n == number of all places
    // Short rationale for estimate: multimap insert(), equal_range() operations are log n,
    // std::find_if is O(m). Worst case if unorderd_map at()-operation takes O(n) or if n == m.
    bool change_place_name(PlaceID id, Name const& newname);

    // Estimate of performance: Average O(m + log n), Worst: O(n)
    // m == number of places with same coordinates, n == number of all places
    // Short rationale for estimate: multimap insert(), equal_range() operations are log n,
    // std::find_if is O(m). Worst case if unorderd_map at()-operation takes O(n) or if n == m.
    bool change_place_coord(PlaceID id, Coord newcoord);

    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: Best: Theta(1), Worst: O(n)
    // Short rationale for estimate: Best if area not added, unordered_map find() is Theta(1)
    // on average. Worst if unordered_map insert() or find() takes O(n).
    bool add_area(AreaID id, Name const& name, std::vector<Coord> coords);

    // Estimate of performance: Average: Theta(1), Worst: O(n)
    // Short rationale for estimate: unordered_map find()-operation
    Name get_area_name(AreaID id);

    // Estimate of performance: Average: Theta(1), Worst: O(n)
    // Short rationale for estimate: unordered_map find()-operation
    std::vector<Coord> get_area_coords(AreaID id);

    // Estimate of performance: Theta(1)
    // Short rationale for estimate: Simply returning a vector.
    std::vector<AreaID> all_areas();

    // Estimate of performance: Average: Theta(1), Worst: O(n)
    // Short rationale for estimate: unordered_map find() operation
    bool add_subarea_to_area(AreaID id, AreaID parentid);

    // Estimate of performance: Average: O(m), Worst: O(n)
    // m == number of ancestor areas of given area. n == number of all areas
    // Short rationale for estimate: Traversing all ancestor areas in while-loop.
    // Worst case if unordered_map find() operation takes O(n) or if m == n.
    std::vector<AreaID> subarea_in_areas(AreaID id);

    // Non-compulsory operations

    // Estimate of performance:
    // Short rationale for estimate:
    void creation_finished();

    // Estimate of performance: Average: O(m), Worst: O(n)
    // m == number of subareas of given area. n == number of all areas
    // Short rationale for estimate: While loop traversing all subareas of given area -> O(m)
    // Worst if unordered_map find() or at() operation takes O(n), or if m == n.
    std::vector<AreaID> all_subareas_in_area(AreaID id);

    // Estimate of performance: Average: O(n), Worst: O(n log n)
    // Short rationale for estimate: For loop traversing all places -> O(n)
    // If calculated distances happen to be in order "nearest-furthest", then
    // push() to priority queue needed every time -> O(n log n)
    std::vector<PlaceID> places_closest_to(Coord xy, PlaceType type);

    // Estimate of performance: Best: Theta(1), Worst: O(n)
    // Short rationale for estimate: Best if place not removed, unordered_map find() opeartion
    // is constant on average. Worst case if unordered_map find() or erase takes O(n).
    bool remove_place(PlaceID id);

    // Estimate of performance: Average: O(h), Worst: O(h*log h)
    // h == length of area-subarea tree. (from given area to closest common area)
    // Short rationale for estimate: While loop traversing all parent areas -> O(h)
    // Worst case happens when unordered_map insert takes O(h) -> O(h*log h)
    AreaID common_area_of_subareas(AreaID id1, AreaID id2);

    // Phase 2 operations

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<WayID> all_ways();

    // Estimate of performance:
    // Short rationale for estimate:
    bool add_way(WayID id, std::vector<Coord> coords);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<std::pair<WayID, Coord>> ways_from(Coord xy);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<Coord> get_way_coords(WayID id);

    // Estimate of performance:
    // Short rationale for estimate:
    void clear_ways();

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<std::tuple<Coord, WayID, Distance>> route_any(Coord fromxy, Coord toxy);

    // Non-compulsory operations

    // Estimate of performance:
    // Short rationale for estimate:
    bool remove_way(WayID id);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<std::tuple<Coord, WayID, Distance>> route_least_crossroads(Coord fromxy, Coord toxy);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<std::tuple<Coord, WayID>> route_with_cycle(Coord fromxy);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<std::tuple<Coord, WayID, Distance>> route_shortest_distance(Coord fromxy, Coord toxy);

    // Estimate of performance:
    // Short rationale for estimate:
    Distance trim_ways();

private:

    Distance dist_so_far = 0;

    bool crossroads_clear = true;
    bool add_first_ = true;
    bool alphabet_sorted_ = false;
    bool coord_sorted_ = false;
    bool places_valid_ = true;

    struct Place
    {
        PlaceID id;
        Name place_name;
        PlaceType place_type;
        Coord coordinate;
    };

    struct Area
    {
        AreaID id;
        Name area_name;
        std::vector<Coord> coords;
        std::vector<std::shared_ptr<Area>> sub_areas;
        std::shared_ptr<Area> parent_area;
    };

    std::unordered_map<PlaceID, std::shared_ptr<Place>> places_;
    std::unordered_map<AreaID, std::shared_ptr<Area>> areas_;

    std::vector<PlaceID> alphabetically_;
    std::vector<PlaceID> coord_order_;
    std::vector<AreaID> area_list_;
    std::vector<PlaceID> place_list_;

    std::multimap<Name, std::shared_ptr<Place>> places_name_order_;
    std::multimap<Coord, std::shared_ptr<Place>> places_coord_order_;

    struct Way
    {
        WayID id;
        std::vector<Coord> coordinates;
        Distance length;
        bool length_valid;
    };

    struct Crossroad
    {
        Coord location;
        std::vector<std::pair<std::shared_ptr<Way>,std::shared_ptr<Crossroad>>> neighbours;
        Distance dist_from_start;
        int steps_from_start;
        int colour;
        std::pair<std::shared_ptr<Way>,std::shared_ptr<Crossroad>> arrived_from;
    };

    std::unordered_map<Coord, std::shared_ptr<Crossroad>, CoordHash, std::equal_to<Coord>> crossroads_;
    std::unordered_map<WayID, std::shared_ptr<Way>> ways_;
    std::vector<WayID> way_ids_;

    std::vector<std::tuple<Coord, WayID, Distance>> route_;

    void store_path(Coord fromxy, Coord toxy, Distance dist, WayID id);
    Distance calculate_way_length(std::shared_ptr<Way> way);
    void clear_crossroads();
};

#endif // DATASTRUCTURES_HH
