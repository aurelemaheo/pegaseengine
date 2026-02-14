
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

        void addBody(RigidBody* body) {
            bodies.push_back(body);
            body->islandId = id;
        }

        void clear() {
            bodies.clear();
        }
};


class IslandDetector
{
    public:
        std::vector<Island> detectIslands(std::vector<std::unique_ptr<RigidBody*>>& bodies, 
                                            const std::vector<RigidBody*>& bodies) {

            islands.clear();
            for(auto& body : bodies) {
                body->islandId = -1; // Reset island ID
            }

            std::map<size_t, std::set<size_t>> graph;
            for(const auto& collision: broadphaseCollisions) {
                size_t idA = collision.bodyA->id;
                size_t idB = collision.bodyB->id;
                graph[idA].insert(idB);
                graph[idB].insert(idA);
            }

            std::set<size_t> visited;
            int islandCounter = 0;

            for(auto& body: bodies) {

                if(visited.find(body->id) == visited.end()) {
                    Island island(islandCounter++);
                    exploreIsland(body, island);
                    islands.push_back(island);
                }
            }

        }

        std::vector<Island> islands;

    private:
        void exploreIsland(RigidBody* body, std::map<size_t, std::set<size_t>>& graph,
                                std::set<size_t>& visited, Island& island,
                                Island& island, std::vector<std::unique_ptr<RigidBody*>>& bodies) {

            if(visited.find(body->id) != visited.end()) return;

            visited.insert(body->id);
            island.addBody(body);

            for(size_t neighborId : graph[body->id]) {
                RigidBody* neighbor = bodies[neighborId].get();
                exploreIsland(neighbor, graph, visited, island, bodies);
            }
};