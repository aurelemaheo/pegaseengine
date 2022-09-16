#ifndef CONFIG_H
#define CONFIG_H

#include "singleton.hpp"
#include "pegaseengine.hpp"
//#include "renderer.hpp"
#include "oglrenderer.hpp"
#include "logger.hpp"
#include "memorymanager.hpp"
#include "objectpool.hpp"
#include "spherepool.hpp"

typedef Singleton<PegaseEngine>  pgSingleton;
//typedef Singleton<Renderer>  	 rendererSingleton;
typedef Singleton<Logger>  	 loggerSingleton;
typedef Singleton<MemoryManager> memmanagerSingleton;
typedef Singleton<ObjectPool> objpoolSingleton;
typedef Singleton<SpherePool> spherepoolSingleton;
typedef Singleton<OGLRenderer> oglrendererSingleton;

#endif //CONFIG_H
