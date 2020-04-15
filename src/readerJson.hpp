#ifndef READERJSON_H
#define READERJSON_H

#include <iostream>

#include "reader.hpp"

//#include "../thirdparty/boost_1_72_0/boost/property_tree/ptree.hpp"
//#include "../thirdparty/boost_1_72_0/boost/property_tree/json_parser.hpp"


/* 
*  Class in charge of reading JSON stream input and populating classes and structures
*/
class ReaderJson : public Reader
{
  public:
   void loadJsonStream(string JsonStreamName); 
   void parseJsonStream();

  private:
   pt::ptree root;
   std::list<Solid> listSolids;
};

#endif //READERJSON_H
