
#include <stdio.h>
#include <stdlib.h>
#include <list>
#include <iostream>

#include "vector3.hpp"


// Class used to describe body entries (position) in the octree structure
template <class T>
struct OctreeEntry {
    T object;
    Vec3 pos;

    OctreeEntry(T obj, Vec3 p) : object(obj), pos(p) {}

};

// Class describing Octree node and related operations such body insertion and content processing
template <class T>
class OctreeNode {
public:
    OctreeNode() {}
    OctreeNode(Vec3 pos, Vec3 volume) {
        position = pos;
        volume = volume;
        children = nullptr;
    }

    ~OctreeNode() {delete[] children;}
    void Insert(T& object, const Vec3& objectPos, const Vec3& objectSize, int depthLeft, int maxVolume) {
        // Implementation of the insert function
        // This is a placeholder implementation; actual logic will depend on the octree design

        if (children) {
            for(int i=0; i<8; ++i) {
                // Determine which child node to insert into based on objectPos and volume
                // This is a simplified example; actual implementation will vary
                //Vec3 childPos = pos; // Calculate child position based on i
                children[i].Insert(object, objectPos, objectSize , depthLeft - 1, maxVolume);
            }
        } else {
            contents.push_back(OctreeEntry<T>(object, objectPos));
            if(contents.size() > maxVolume && depthLeft > 0) {
                Split();
                // Subdivide and reinsert contents into children
                // This is a placeholder; actual subdivision logic will vary
                children = new OctreeNode<T>[8];
                for(auto& entry : contents) {
                    for(int i=0; i<8; ++i) {
                        // Determine which child node to insert into based on entry.pos and volume
                        children[i].Insert(entry.object, entry.pos, objectSize, depthLeft - 1, maxVolume);
                    }
                }
                contents.clear();
            }
        }
    }

    typedef std::function<void(std::list<OctreeEntry<T>>&)> OctreeFunc; 

    void OperateOnContents(const typename OctreeNode<T>::OctreeFunc& func) {

        if(children) {
            for(int i=0; i<8; ++i) {
                children[i].OperateOnContents(func);
            }
        } else {
            func(contents);
        }
    }

    // Split leaf into 8 children   
    void Split() {
        // Implementation of the Split function
        // This is a placeholder implementation; actual logic will depend on the octree design
        // Initialize child nodes with appropriate positions
        Vec3 halfSize = volume / 2.0f;
        Vec3 quarterSize = halfSize / 2.0f;  // Offset = size/4

        children = new OctreeNode<T>[8];

        children[0] = OctreeNode(position + Vec3(-quarterSize.x,  quarterSize.y,  quarterSize.z), halfSize);  
        children[1] = OctreeNode(position + Vec3( quarterSize.x,  quarterSize.y,  quarterSize.z), halfSize);  
        children[2] = OctreeNode(position + Vec3(-quarterSize.x,  quarterSize.y, -quarterSize.z), halfSize);  
        children[3] = OctreeNode(position + Vec3( quarterSize.x,  quarterSize.y, -quarterSize.z), halfSize); 

        children[4] = OctreeNode(position + Vec3(-quarterSize.x, -quarterSize.y,  quarterSize.z), halfSize);  
        children[5] = OctreeNode(position + Vec3( quarterSize.x, -quarterSize.y,  quarterSize.z), halfSize);  
        children[6] = OctreeNode(position + Vec3(-quarterSize.x, -quarterSize.y, -quarterSize.z), halfSize); 
        children[7] = OctreeNode(position + Vec3( quarterSize.x, -quarterSize.y, -quarterSize.z), halfSize); 
    }

    std::list<OctreeEntry<T>> contents;

    private:

    Vec3 position;
    Vec3 volume;
    OctreeNode<T>* children;

};

// Template class describing an octree for spatial partitioning, used in broad phase collision detection
template <class T>
class Octree {
public: 
    Octree(Vec3 volume, int maxDepth) {
        root = new OctreeNode<T>(Vec3(0,0,0), volume);
        //this->maxDepth = maxDepth;
        this->volume = volume;

    } 

    void Insert(T object, const Vec3& pos, Vec3& size) {
        root->Insert(object, pos, size, depthLeft, maxVolume);
    }

    void OperateOnContents(const typename OctreeNode<T>::OctreeFunc& func) {
        root->OperateOnContents(func);
    }             
private:
    OctreeNode<T>* root;
    
    int depthLeft;
    int maxVolume;
    Vec3 volume;

protected:
    friend class OctreeNode<T>;

};