#pragma once
#include <memory>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Wyvern {

class Logger
{
public:
	static void init();

	inline static std::shared_ptr<spdlog::logger>& getConsoleLogger() { return s_consoleLogger; }
	inline static std::shared_ptr<spdlog::logger>& getRenderAPILogger() { return s_renderAPILogger; }

private:
	static std::shared_ptr<spdlog::logger> s_consoleLogger;
	static std::shared_ptr<spdlog::logger> s_renderAPILogger;
};

}

#define WYVERN_LOG_TRACE(...) ::Wyvern::Logger::getConsoleLogger()->trace(__VA_ARGS__)
#define WYVERN_LOG_INFO(...) ::Wyvern::Logger::getConsoleLogger()->info(__VA_ARGS__)
#define WYVERN_LOG_WARN(...) ::Wyvern::Logger::getConsoleLogger()->warn(__VA_ARGS__)
#define WYVERN_LOG_ERROR(...) ::Wyvern::Logger::getConsoleLogger()->error(__VA_ARGS__)
#define WYVERN_LOG_RENDER_API(...) ::Wyvern::Logger::getRenderAPILogger()->warn(__VA_ARGS__)