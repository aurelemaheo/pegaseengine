#ifndef CONFIG_H
#define CONFIG_H

#include "singleton.hpp"
#include "pegaseengine.hpp"
#include "renderer.hpp"
#include "logger.hpp"

typedef Singleton<PegaseEngine>  pgSingleton;
typedef Singleton<Renderer>  	 rendererSingleton;
typedef Singleton<Logger>  	 loggerSingleton;

#endif //CONFIG_H
