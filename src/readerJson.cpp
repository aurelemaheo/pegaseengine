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
