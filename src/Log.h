#ifndef LOG_H
#define LOG_H
#pragma once


#include <spdlog/spdlog.h>
#include <memory>
#include <stdarg.h>
#include <ostream>
#include <sstream>
namespace GOSM {



class Log
{
public:
	static std::stringbuf m_Buffer;
public:
	static void Init();
	inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
	inline static std::shared_ptr<spdlog::logger>& GetSimpleLogger() { return s_SimpleLogger; }
private:
	static std::shared_ptr<spdlog::logger> s_CoreLogger;
	static std::shared_ptr<spdlog::logger> s_SimpleLogger;
};

}

#ifndef NO_SPDLOG
	#define LOG_TRACE(...) GOSM::Log::GetSimpleLogger()->trace(__VA_ARGS__);
	#define LOG_INFO(...) SPDLOG_INFO(__VA_ARGS__)
	#define LOG_WARN(...) SPDLOG_WARN(__VA_ARGS__)
	#define LOG_ERROR(...) SPDLOG_ERROR(__VA_ARGS__)
	#define LOG_CRITICAL(...) SPDLOG_CRITICAL(__VA_ARGS__)
#else
	#define LOG_TRACE(...) 
	#define LOG_INFO(...) 
	#define LOG_WARN(...) 
	#define LOG_ERROR(...) 
	#define LOG_CRITICAL(...) 
#endif

#endif /* LOG_H */