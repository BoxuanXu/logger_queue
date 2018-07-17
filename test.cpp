/*================================================================
*   Copyright (C) 2018 xuboxuan. All rights reserved.
*   
*   文件名称：test.cpp
*   创 建 者： xuboxuan
*   创建日期：2018年07月17日
*   描    述：
*
================================================================*/


#include "logger.h"
#include <chrono>
int main(){
	Logger::initLogger("test.log");
	LOG_INFO("this is first info test");
	LOG_WARNING("this is first warning test");
	LOG_ERROR("this is first error test");
	std::this_thread::sleep_for(std::chrono::seconds(1));
	
}
