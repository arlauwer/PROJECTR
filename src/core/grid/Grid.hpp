#pragma once
//
#include "Config.hpp"
#include <concepts>
#include <cstddef>
#include <optional>
//
class WavGrid;
class RadiationField;
//
template <typename G>
concept Grid = requires(G g) {
    // grid batch
    typename G::Batch;
    //
    // batch functions
    { g.initialize(std::declval<typename G::Batch&>()) };
    { g.propagate(std::declval<typename G::Batch&>()) };
    //
    // member variables
    { g.N } -> std::convertible_to<size_t&>;
    { g.kappa } -> std::same_as<vector<real>&>;
    { g.albedo } -> std::same_as<vector<real>&>;
    //
    // radiation field support
    { g.radField } -> std::same_as<std::optional<RadiationField>&>;
    { g.addRadiationField(std::declval<const vector<real>&>()) };
};

// #pragma once

// #include "../batch/Batch.hpp"

// class Grid
// {
//   public:
//     virtual ~Grid() = default;

//     virtual void initialize(Batch& batch) = 0;
//     virtual void propagate(Batch& batch)  = 0;
// };