#include "Log.h"

namespace Wyvern {

std::shared_ptr<spdlog::logger> Log::s_consoleLogger;
std::shared_ptr<spdlog::logger> Log::s_renderAPILogger;


void Log::Init()
{
	s_consoleLogger = spdlog::stdout_color_mt("Console");
	s_renderAPILogger = spdlog::stdout_color_mt("Render API");

}

}

 
