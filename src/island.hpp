
#include <vector>

#include "rigidbody.hpp"

// Class representing an island of interconnected bodies (for sleeping and optimization)
class Island
{

    public:
        Island() : id(0) {}
        Island(int id) : id(id) {}

        int id;
        std::vector<RigidBody*> bodies;
};


class IslandDetector
{
    public:
        std::vector<Island> detectIslands(const std::vector<RigidBody*>& bodies);
        std::vector<Island> islands;
};