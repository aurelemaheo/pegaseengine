#ifndef CONFIG_H
#define CONFIG_H

#include "singleton.hpp"
#include "pegaseengine.hpp"
#include "renderer.hpp"
#include "logger.hpp"
#include "memorymanager.hpp"

typedef Singleton<PegaseEngine>  pgSingleton;
//typedef Singleton<Renderer>  	 rendererSingleton;
typedef Singleton<Logger>  	 loggerSingleton;
typedef Singleton<MemoryManager> memmanagerSingleton;
typedef Singleton<ObjectPool> objpoolSingleton;

#endif //CONFIG_H
