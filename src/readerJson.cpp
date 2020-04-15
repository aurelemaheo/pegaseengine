//#ifndef READERJSON_H
//#define READERJSON_H

#include <iostream>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "readerjson.hpp"

namespace pt = boost::property_tree;

//ReaderJson::ReaderJson()
//{
//}

void ReaderJson::loadJsonStream(std::string jsonStreamName)
{
  pt::ptree root;
  pt::read_json(jsonStreamName, root);

}

void ReaderJson::parseJsonStream()
{
  std::string msgSolid = root.get<std::string>("scene.objects.type.sphere");  

  Sphere s;

  for(pt::ptree::value_type &object : root.get_child("objects"))
  {
    float fsize = root.get<float>("size");
    float fweight = root.get<float>("weight");
    //pt::ptree::value_type ssize = root.get_child("size");
    //std::string ssize = root.get_child("size");
    //s._radius = ssize.second.data();
    
    //listSpheres.insert(0,s);    
  }
  
}

//#endif //READERJSON_H
