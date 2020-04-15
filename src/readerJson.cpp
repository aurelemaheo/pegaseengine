#ifndef READERJSON_H
#define READERJSON_H

#include <iostream>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "readerjson.hpp"

namespace pt = boost::property_tree;

ReaderJson::ReaderJson()
{
}

void ReaderJson::loadJsonStream(std::string jsonStreamName)
{
  pt::ptree_root;
  pt::read_json(jsonStreamName, root);

}

void ReaderJson::parseJsonStream()
{
  string msgSolid = root.get<std::string>("scene.objects.type.sphere");  

  Sphere s;

  for(pt::ptree::value_type &object : root::get_child("objects"))
  {
    pt::ptree::value_type &size = root.get_child("size");
    s._radius = size.second.data();
    
    listSpheres.insert(s);    
  }
  
}

#endif //READERJSON_H
