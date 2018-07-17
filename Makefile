

#================================================================
#   Copyright (C) 2018 xuboxuan. All rights reserved.
#   
#   文件名称：Makefile
#   创 建 者： xuboxuan
#   创建日期：2018年07月17日
#   描    述：
#
#================================================================

all:
	g++ -std=c++11 -o test test.cpp logger.cpp -lpthread
clean:
	rm -rf test test.log
