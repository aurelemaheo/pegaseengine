#ifndef SPHEREPOOL_H
#define SPHEREPOOL_H

#define "oglsphere.hpp"

class SpherePool
{
private:
        std::list<OGLSphere*> spheres;

    public:
        ObjectPool(){}
        ~ObjectPool(){}

        OGLSphere* createSphere()
        {   
            LOG(DEBUG) << "Create sphere" << std::endl;

            return new OGLSphere;            
        }

        void storeSphere(OGLSphere* sphere)
        {
            LOG(DEBUG) << "Creating new sphere" << std::endl;

            spheres.push_back(sphere); 
        }

        int getPoolSize()
        {
            return spheres.size();
        }

        Body* getResource()
        {
            if(spheres.empty())
            {
                std::cout << "Creating new sphere" << std::endl;
                return new Sphere;
            }
            else 
            {
                std::cout << "Reusing existing" << std::endl;
            }
        }

        std::list<OGLSphere*> getListSpheres()
        {
            return spheres;
        }
};

#endif // SPHEREPOOL_H
