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

void ReaderJson::loadJsonStream(std::string jsonStreamName)
{
  std::cout << "Load Json source " << std::endl;

  //pt::ptree root;
  //pt::read_json(jsonStreamName, root);
  pt::read_json("scene.json", root);

}

void ReaderJson::parseJsonStream()
{
  std::cout << "Parse JSON source " << std::endl;
  
  StoreSolids ss;  
  Sphere s;

  for(pt::ptree::value_type &solid : root.get_child("solids"))
  {
    double fsize = solid.second.get<double>("size");
    double fweight = solid.second.get<double>("weight");
    double posx = solid.second.get<double>("position.x");
    double posy = solid.second.get<double>("position.y");
    double posz = solid.second.get<double>("position.z");
    std::cout << "Read size: " << fsize << ", Read weight: " << fweight << std::endl;
    std::cout << "Position (x: " << posx << ", y: " << posy << ", z: " << posz << ")" << std::endl;
    //pt::ptree::value_type ssize = root.get_child("size");
    //std::string ssize = root.get_child("size");
    //s._radius = ssize.second.data();
    
    ss.addSolid(s); 
  }
  
}

//#endif //READERJSON_H
