#include <iostream>
#include <cassert>
#include <stdio.h>
//#include <conio.h>

#include <time.h>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <Python.h>

#include "readerjson.hpp"

namespace pt = boost::property_tree;

pt::ptree root;

ReaderJson::ReaderJson()
{

  LOG(DEBUG) << "ReaderJson Constructor" << std::endl;

  char filename[] = "readerjson.py";
	FILE* fp;

	Py_Initialize();

  LOG(DEBUG) << "Python initialized" << std::endl;

	fp = _Py_fopen(filename, "r");
	//PyRun_SimpleFile(fp, filename);
  PyRun_SimpleString("import readerjson;");
  PyRun_SimpleString("readerjson.parsejson()");

	Py_Finalize();

  LOG(DEBUG) << "Python finalized" << std::endl;
}

ReaderJson::~ReaderJson()
{

}

/*
* Load JSON Stream 
*/
void ReaderJson::loadJsonStream(std::string jsonStreamName)
{
  LOG(DEBUG) << "Load JON source from Python script" << std::endl;
  
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

  for(pt::ptree::value_type &solid : root.get_child("solids"))
  {

      /*
    int Id = rand() % 1000; 
    std::cout << "ID generated for a solid: " << Id << std::endl; 
    Sphere s(Id);

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

    s.setSize(dsize);
    s.setWeight(dweight);
    s.setPosition(posx,posy,posz);
    s.setLinVelocity(linvelx,linvely,linvelz);
    s.setAngVelocity(angvel);
    
    ss.addBody(s);
       */
  }
  
}

