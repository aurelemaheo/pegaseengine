#ifndef READERJSON_H
#define READERJSON_H

#include <iostream>

//#include <boost/property_tree/ptree.hpp>
//#include <boost/property_tree/json_parser.hpp>

#include "reader.hpp"
#include "storebodies.hpp"

//namespace pt = boost::property_tree;

/* 
*  Class in charge of reading JSON stream input and populating classes and structures
*/
class ReaderJson : public Reader
{
  public:
   ReaderJson();
   ~ReaderJson();
   void loadJsonStream(std::string JsonStreamName); 
   void parseJsonStream();

  private:
   //pt::ptree root;
};

#endif //READERJSON_H
