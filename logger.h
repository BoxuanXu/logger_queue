/*================================================================
*   Copyright (C) 2018 xuboxuan. All rights reserved.
*   
*   文件名称：logger.h
*   创 建 者：  xuboxuan
*   创建日期：2018年07月17日
*   描    述：
*
================================================================*/


#ifndef _LOGGER_H
#define _LOGGER_H
#include "syncqueue.h"

#include <memory>
#include <fstream>
#include <string>
#include <ctime>
#include <iostream>
#include <stdint.h>
#include <sstream>
namespace{
	std::string filename = "info.log";
}

enum Priority{
	DEBUG,
	CONFIG,
	INFO,
	WARNING,
	ERROR
};

class Logger{
	Logger(const Logger& g_tmp) = delete;
	Logger &operator=(const Logger&) = delete;
public:
	static void initLogger(const std::string& info_log_filename);

	static Logger *Get();
	
	void SetPriority(Priority priority);
	
	Priority GetPriority();

	
	void WriteLog(Priority priority, const int line, const std::string &function,const std::string &log_content);

	void SetFilename(std::string & info_log_filename);
private:
	Logger(Priority priority);
	
	virtual ~Logger();
	
	void InitializeFileStream();

	void WriteThread();

private:
	SyncQueue<std::string> _queue;
	std::shared_ptr<std::ofstream> _filestream;
	Priority _priority;
	bool _shutdown;
};

#define LOG_DEBUG(LOG_CONTENT) \
	Logger::Get()->WriteLog(DEBUG, __LINE__,__FUNCTION__,(std::string)LOG_CONTENT)

#define LOG_INFO(LOG_CONTENT) \
	Logger::Get()->WriteLog(INFO, __LINE__,__FUNCTION__,(std::string)LOG_CONTENT)

#define LOG_WARNING(LOG_CONTENT) \
	Logger::Get()->WriteLog(WARNING, __LINE__,__FUNCTION__,(std::string)LOG_CONTENT)

#define LOG_ERROR(LOG_CONTENT) \
	Logger::Get()->WriteLog(ERROR, __LINE__,__FUNCTION__,(std::string)LOG_CONTENT)

#define LOG_CONFIG(LOG_CONTENT) \
	Logger::Get()->WriteLog(CONFIG, __LINE__,__FUNCTION__,(std::string)LOG_CONTENT)
#endif //LOGGER_H
