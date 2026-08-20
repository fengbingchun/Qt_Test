#ifndef FBC_LOGGER_HPP_
#define FBC_LOGGER_HPP_

#ifdef FBC_LOGGER
#include <string>
#include <memory>
#include <filesystem>
#include <chrono>
#include <vector>
#include <iterator>
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include <spdlog/spdlog.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace fbc {

class Logger {
public:
	Logger(const Logger&) = delete;
	Logger& operator=(const Logger&) = delete;

	static Logger& get_instance()
	{
		static Logger logger;
		return logger;
	}

	void init(const std::string& dir = "logs", const std::string& name = "fbc", bool console = true, unsigned short max_files = 0, spdlog::level::level_enum level = spdlog::level::info)
	{
		if (initialized_) return;

		namespace fs = std::filesystem;
		if (!fs::exists(dir))
			fs::create_directories(dir);

		std::vector<spdlog::sink_ptr> sinks;

		auto daily_sink = std::make_shared<spdlog::sinks::daily_file_sink_mt>(dir + "/" + name + ".log", 0, 0, false, max_files);
		sinks.push_back(daily_sink);

		if (console) {
			auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
			sinks.push_back(console_sink);
		}

		logger_ = std::make_shared<spdlog::logger>("fbc-logger", std::begin(sinks), std::end(sinks));
		logger_->set_level(level);
		logger_->flush_on(spdlog::level::warn);

		logger_->set_pattern("%^[%Y-%m-%d %H:%M:%S.%e] [%l] [thread %t] [%s:%#]%$ %v");

		initialized_ = true;
	}

	std::shared_ptr<spdlog::logger> get_logger()
	{
		if (!initialized_)
			init();
		return logger_;
	}

private:
	Logger() = default;
	~Logger()
	{
		if (logger_)
			logger_->flush();
		spdlog::shutdown();
	}

	std::shared_ptr<spdlog::logger> logger_{};
	bool initialized_{ false };
};

} // namespace fbc

#define FBC_LOG_TRACE(...)		SPDLOG_LOGGER_TRACE(fbc::Logger::get_instance().get_logger(), __VA_ARGS__)
#define FBC_LOG_DEBUG(...)		SPDLOG_LOGGER_DEBUG(fbc::Logger::get_instance().get_logger(), __VA_ARGS__)
#define FBC_LOG_INFO(...)		SPDLOG_LOGGER_INFO(fbc::Logger::get_instance().get_logger(), __VA_ARGS__)
#define FBC_LOG_WARN(...)		SPDLOG_LOGGER_WARN(fbc::Logger::get_instance().get_logger(), __VA_ARGS__)
#define FBC_LOG_ERROR(...)		SPDLOG_LOGGER_ERROR(fbc::Logger::get_instance().get_logger(), __VA_ARGS__)
#define FBC_LOG_CRITICAL(...)	SPDLOG_LOGGER_CRITICAL(fbc::Logger::get_instance().get_logger(), __VA_ARGS__)

#else

#define FBC_LOG_TRACE(...)
#define FBC_LOG_DEBUG(...)
#define FBC_LOG_INFO(...)
#define FBC_LOG_WARN(...)
#define FBC_LOG_ERROR(...)
#define FBC_LOG_CRITICAL(...)

#endif // FBC_LOGGER

#endif // FBC_LOGGER_HPP_
