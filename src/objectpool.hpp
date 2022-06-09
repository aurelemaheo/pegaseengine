#ifndef OBJECTPOOL_H
#define OBJECTPOOL_H

#include <list>

#include "body.hpp"

class ObjectPool
{
    private:
        std::list<Body*> bodies;

    public:
        ObjectPool(){}
        ~ObjectPool(){}

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