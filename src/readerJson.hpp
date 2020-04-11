#include <iostream>

#include "../thirdparty/boost_1_72_0/boost/property_tree/ptree.hpp"
#include "../thirdparty/boost_1_72_0/boost/property_tree/json_parser.hpp"


class ReaderJson : public Reader
{
  public:
   void loadJsonStream(string JsonStreamName); 
  private:
};
