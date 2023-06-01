#pragma once
#include <memory>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Wyvern {

class Log
{
public:
	static void Init();

	inline static std::shared_ptr<spdlog::logger>& getConsoleLogger() { return s_consoleLogger; }
	inline static std::shared_ptr<spdlog::logger>& getRenderAPILogger() { return s_renderAPILogger; }

private:
	static std::shared_ptr<spdlog::logger> s_consoleLogger;
	static std::shared_ptr<spdlog::logger> s_renderAPILogger;
};

}

#define WYVERN_LOG_TRACE(...) ::Wyvern::Log::getConsoleLogger()->trace(__VA_ARGS__)
#define WYVERN_LOG_INFO(...) ::Wyvern::Log::getConsoleLogger()->info(__VA_ARGS__)
#define WYVERN_LOG_WARN(...) ::Wyvern::Log::getConsoleLogger()->warn(__VA_ARGS__)
#define WYVERN_LOG_ERROR(...) ::Wyvern::Log::getConsoleLogger()->error(__VA_ARGS__)
#define WYVERN_LOG_RENDER_API(...) ::Wyvern::Log::getRenderAPILogger()->warn(__VA_ARGS__)