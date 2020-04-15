#ifndef READERJSON_H
#define READERJSON_H

#include <iostream>

#include "reader.hpp"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace pt = boost::property_tree;

/* 
*  Class in charge of reading JSON stream input and populating classes and structures
*/
class ReaderJson : public Reader
{
  public:
   void loadJsonStream(std::string JsonStreamName); 
   void parseJsonStream();

  private:
   //pt::ptree root;
   //std::list<Sphere> listSpheres;
};

#endif //READERJSON_H
