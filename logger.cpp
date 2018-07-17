/*================================================================
*   Copyright (C) 2018 xuboxuan. All rights reserved.
*   
*   文件名称：logger.cpp
*   创 建 者： xuboxuan
*   创建日期：2018年07月17日
*   描    述：
*
================================================================*/


#include "logger.h"

const std::string PRIORITY_STRING[] = {
	"DEBUG",
	"CONFIG",
	"INFO",
	"WARNING",
	"ERROR"
};

Logger *Logger::Get(){
	static Logger logger(DEBUG);
	return &logger;
}

Logger::Logger(Priority priority) : _queue(),_filestream(nullptr),_shutdown(false){
	_priority = priority;
	InitializeFileStream();
	auto func = std::bind(&Logger::WriteThread, this);
	std::thread writeThread(func);
	writeThread.detach();	
} 

Logger::~Logger(){
	_shutdown = true;
	if(_filestream){
		_filestream->close();
	}
}

void Logger::SetPriority(Priority priority){
	_priority = priority;
}

Priority Logger::GetPriority(){
	return _priority;
}

void Logger::initLogger(const std::string& info_log_filename){
	filename = info_log_filename;
}

void Logger::InitializeFileStream(){
	//Initialize file stream
	_filestream = std::make_shared<std::ofstream>();
	std::ios_base::openmode mode = std::ios_base::out;
	mode |= std::ios_base::trunc;
	_filestream->open(filename, mode);
	
	if(!_filestream->is_open()){
		std::ostringstream ss_error;
		ss_error << "FATAL ERROR: could not open log file: [" << filename << "]";
		ss_error << "\n\t\t std::ops_base state = " << _filestream->rdstate();
		std::cerr << ss_error.str().c_str() << std::endl << std::flush;
		
		_filestream->close();
	}

	
}


void Logger::WriteLog(Priority priority, const int line, const std::string &function,const std::string &log_content){
	if(priority < _priority)
		return;
	
	std::stringstream stream;
	time_t tm;
	time(&tm);
	char time_string[128];
	ctime_r(&tm, time_string);	
	

	stream << time_string << " [" <<  PRIORITY_STRING[priority]<< "]" <<  " line " << line << " :" << log_content;

	_queue.Put(stream.str());
	std::cout << stream.str() << std::endl;
}


void Logger::WriteThread(){
	while(!_shutdown){
		std::string log;
		_queue.Pop(log);
		std::cout << log << std::endl;
		
		if(_filestream){
			*_filestream << log << std::endl;
		}
	}
}


