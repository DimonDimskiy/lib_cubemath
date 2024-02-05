#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <vector>
#include <tuple>
#include <algorithm>


using Cell = std::tuple<int, int, int>;
using Offset = std::tuple<int, int, int>;

int cubeDistance(Cell a, Cell b){
    int res = std::max({
        std::abs(std::get<0>(a) - std::get<0>(b)), 
        std::abs(std::get<1>(a) - std::get<1>(b)),
        std::abs(std::get<2>(a) - std::get<2>(b))
});
    return res;
}

Cell offset(Cell a, Offset da){
    int dx = std::get<0>(a) + std::get<0>(da);
    int dy = std::get<1>(a) + std::get<1>(da);
    int dz = std::get<2>(a) + std::get<2>(da);
    return std::make_tuple(dx, dy, dz);
}

std::vector<Cell> getNeighbours(Cell a){
    std::vector<Cell> result;
    std::vector<int> deltas({-1, 0, 1});

    do {
        result.push_back(offset(a, std::make_tuple(deltas[0], deltas[1], deltas[2])));
    } while (std::next_permutation(deltas.begin(), deltas.end()));
 
    return result;
}

std::vector<Cell> inRadius(Cell a, int radius){
    std::vector<Cell> result;
    for(int dx{-radius}; dx <= radius; dx++){
        for(
        int dy = std::max(-radius, -dx - radius); 
        dy <= std::min(radius, radius - dx);
        dy++
        ){
            int dz = - dx - dy;
            result.push_back(offset(a, std::make_tuple(dx, dy, dz)));
        }
    }
    return result;
}

std::vector<Cell> normalDirections(Cell a, int radius){
    std::vector<Cell> result;
    for (int i{1}; i <= radius; i ++){
        std::vector<int> deltas({-i, 0, i});
        do{
            result.push_back(offset(a, std::make_tuple(deltas[0], deltas[1], deltas[2])));
        }while(std::next_permutation(deltas.begin(), deltas.end()));
    }
    return result;
}

std::vector<Cell> inRadiusExcl(Cell a, int radius, int eRadius){
    std::vector<Cell> result;
    std::vector<Cell> large = inRadius(a, radius);
    std::vector<Cell> small = inRadius(a, eRadius);
    std::set_difference(
        large.begin(), large.end(), 
        small.begin(), small.end(),
        std::inserter(result, result.end())
        );
    return result;
}

PYBIND11_MODULE(cubemath, m) {
    m.def("cube_distance", &cubeDistance, "Calculate cube distance between two given cells");
    m.def("offset", &offset, "Returns new cell by given offsset and given start cell");
    m.def("get_neighbours", &getNeighbours, "Returns list of neighbours cells");
    m.def("in_radius", &inRadius, "returns all cells in given radius including given one");
    m.def("normal_directions", &normalDirections,
    "Returns cells in given radius that have one common coordinate with given cell");
    m.def("in_radius_excl", &inRadiusExcl, "Get cells in radius excluding some");
}

