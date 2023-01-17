#ifndef OBJECTPOOL_H
#define OBJECTPOOL_H

#include <list>

#include "basebody.hpp"
#include "logger.hpp"

class ObjectPool
{
    private:
        std::list<BaseBody*> bodies;

    public:
        ObjectPool(){}
        ~ObjectPool(){}

        BaseBody* createBaseBody()
        {   
            LOG(DEBUG) << "Create body" << std::endl;

            return new BaseBody;            
        }

        void storeBaseBody(BaseBody* body)
        {
            LOG(DEBUG) << "Creating new body" << std::endl;

            bodies.push_back(body); 
        }

        int getPoolSize()
        {
            return bodies.size();
        }

        BaseBody* getResource()
        {
            if(bodies.empty())
            {
                std::cout << "Creating new body" << std::endl;
                return new BaseBody;
            }
            else 
            {
                std::cout << "Reusing existing" << std::endl;
            }
        }

        std::list<BaseBody*> getListBodies()
        {
            return bodies;
        }

};

#endif //OBJECTPOOL_H
