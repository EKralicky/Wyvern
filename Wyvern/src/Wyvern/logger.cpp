#include "logger.h"

namespace Wyvern {

std::shared_ptr<spdlog::logger> Logger::s_consoleLogger;
std::shared_ptr<spdlog::logger> Logger::s_renderAPILogger;


void Logger::init()
{
	s_consoleLogger = spdlog::stdout_color_mt("Console");
	s_renderAPILogger = spdlog::stdout_color_mt("Render API");
	WYVERN_LOG_INFO("Successfully initialized logger");
}

}

 
