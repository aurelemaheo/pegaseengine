#include <iostream>
#include <cassert>
#include <stdio.h>
//#include <conio.h>

#include <time.h>
#include <string>
#include <fstream>

//#include <boost/property_tree/ptree.hpp>
//#include <boost/property_tree/json_parser.hpp>
#include <json/json.h>

#include <Python.h>

#include "readerjson.hpp"
#include "config.hpp"

//pt::ptree root;

ReaderJson::ReaderJson()
{

  LOG(DEBUG) << "ReaderJson Constructor" << std::endl;

  char filename[] = "readerjson.py";
	//FILE* fp;

  std::ifstream t("scene01.json");
  std::stringstream buffer;
  buffer << t.rdbuf();
  
  std::string jsondata = buffer.str();
  
  LOG(DEBUG) << "JSON FILE: " << jsondata << std::endl;

  Json::Value json;
  Json::Reader reader;
	//Py_Initialize();

  reader.parse(jsondata, json, false);

  LOG(DEBUG) << "JSON DATA: " << json << std::endl;

  LOG(DEBUG) << "print JSON bodies: " << json["bodies"] << std::endl;
  
  const Json::Value bodies = json["bodies"];

   
  for(int iter=0 ; iter < bodies.size() ; iter ++) 
  {
    Body* itemBody = objpoolSingleton::getInstance()->getResource();
    const Json::Value x_pos = bodies[iter]["position"]["x"];
    const Json::Value y_pos = bodies[iter]["position"]["y"];
    const Json::Value z_pos = bodies[iter]["position"]["z"];
    
    const Json::Value x_lvel = bodies[iter]["linvelocity"]["x"];
    const Json::Value y_lvel = bodies[iter]["linvelocity"]["y"];
    const Json::Value z_lvel = bodies[iter]["linvelocity"]["z"];

    itemBody->setPosition(x_pos.asDouble(), y_pos.asDouble(), z_pos.asDouble());
    LOG(DEBUG) << "read body [ " << iter << " ] : " << bodies[iter] << " ; position: x = " << x_pos << ", y = " << y_pos << ", z = " << z_pos << std::endl;
  }

  LOG(DEBUG) << "Python initialized" << std::endl;

	//fp = _Py_fopen(filename, "r");
	//PyRun_SimpleFile(fp, filename);
  //PyRun_SimpleString("import readerjson;");
  //PyRun_SimpleString("readerjson.parsejson()");

  //PyObject* myModuleString = PyString_FromString((char*)"readerjson");
  //PyObject* myModule = PyImport_Import(myModuleString);

  //PyObject* myFunction = PyObject_GetAttrString(myModule,(char*)"parsejson");
  //PyObject* args = PyTuple_Pack(1,PyFloat_FromDouble(2.0));

  //PyObject* myResult = PyObject_CallObject(myFunction, args);

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
  //pt::read_json("scene.json", root);

}

/*
*  Parse JSON stream and populate Solid content
*/
void ReaderJson::parseJsonStream()
{
  std::cout << "Parse JSON source " << std::endl;
  
  //StoreSolids ss;

  //for(pt::ptree::value_type &solid : root.get_child("solids"))
  //{

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
  //}
  
}

