#include <iostream>

#include "JsonReader.hpp"

namespace pt = boost::property_tree;

readerJson::readerJson()
{
}

void readerJson::loadJsonStream(string jsonStreamName)
{
  pt::ptree_root;
  pt::read_json(jsonStreamName, root);

}

void readerJson::parseJsonStream()
{
  string msgSolid = root.get<std::string>("scene.objects.type.sphere");  
  
}
