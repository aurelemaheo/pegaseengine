#ifndef OBJECTPOOL_H
#define OBJECTPOOL_H

#include <list>

#include "body.hpp"
#include "logger.hpp"

class ObjectPool
{
    private:
        std::list<Body*> bodies;

    public:
        ObjectPool(){}
        ~ObjectPool(){}

        Body* createBody()
        {   
            LOG(DEBUG) << "Create body" << std::endl;

            return new Body;            
        }

        void storeBody(Body* body)
        {
            LOG(DEBUG) << "Creating new body" << std::endl;

            bodies.push_back(body); 
        }

        int getPoolSize()
        {
            return bodies.size();
        }

        Body* getResource()
        {
            if(bodies.empty())
            {
                std::cout << "Creating new body" << std::endl;
                return new Body;
            }
            else 
            {
                std::cout << "Reusing existing" << std::endl;
            }
        }

};

#endif //OBJECTPOOL_H