//#ifndef READERJSON_H
//#define READERJSON_H

#include <iostream>
#include <cassert>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "readerjson.hpp"

namespace pt = boost::property_tree;

pt::ptree root;

//ReaderJson::ReaderJson()
//{
//}

/*
* Load JSON Stream 
*/
void ReaderJson::loadJsonStream(std::string jsonStreamName)
{
  std::cout << "Load Json source " << std::endl;

  //pt::ptree root;
  //pt::read_json(jsonStreamName, root);
  pt::read_json("scene.json", root);

}

/*
*  Parse JSON stream and populate Solid content
*/
void ReaderJson::parseJsonStream()
{
  std::cout << "Parse JSON source " << std::endl;
  
  //StoreSolids ss;  
  Sphere s;

  for(pt::ptree::value_type &solid : root.get_child("solids"))
  {
    double dsize = solid.second.get<double>("size");
    double dweight = solid.second.get<double>("weight");
    double posx = solid.second.get<double>("position.x");
    double posy = solid.second.get<double>("position.y");
    double posz = solid.second.get<double>("position.z");
    double linvelx = solid.second.get<double>("linvelocity.x");
    double linvely = solid.second.get<double>("linvelocity.y");
    double linvelz = solid.second.get<double>("linvelocity.z");
    double angvel = solid.second.get<double>("angvelocity");
   
    std::cout << "Read size: " << dsize << ", Read weight: " << dweight << std::endl;
    std::cout << "Position (x: " << posx << ", y: " << posy << ", z: " << posz << ")" << std::endl;
    std::cout << "Linear velocity (x: " << linvelx << ", y: " << linvely << ", z: " << linvelz << ")" << std::endl;
    std::cout << "Angular velocity: " << angvel << std::endl;
    //std::string ssize = root.get_child("size");
    s.setSize(dsize);
    s.setWeight(dweight);
    s.setPosition(posx,posy,posz);
    s.setLinVelocity(linvelx,linvely,linvelz);
    s.setAngVelocity(angvel);
    
    ss.addSolid(s); 
  }
  
}

//#endif //READERJSON_H
