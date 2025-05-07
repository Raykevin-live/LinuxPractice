#pragma once

#include <iostream>
#include <stdarg.h>
#include <time.h>
#include <string>
#include "comm.hpp"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define SIZE 1024

#define Info 0
#define Debug 1
#define Warning 2
#define Error 3
#define Fatal 4

#define Screen 1
#define Onefile 2
#define Classfile 3

#define LogFile "log.txt"

class Log
{
private:
    int printMethod;

public:
    Log()
    {
        printMethod = Screen;
    }
    void Enable(int method)
    {
        printMethod = method;
    }
    std::string levelToString(int level)
    {
        switch (level)
        {
        case Info:
            return "Info";
        case Debug:
            return "Debug";
        case Warning:
            return "Warning";
        case Error:
            return "Error";
        case Fatal:
            return "Fatal";
        default:
            return "No";
        }
    }

    void logmessage(int level, const char *format, ...)
    {
        char leftbuffer[SIZE];
        time_t t = time(nullptr);
        struct tm *ctime = localtime(&t);

        snprintf(leftbuffer, sizeof(leftbuffer), "[%s][%d-%d-%d %d:%d:%d]", levelToString(level).c_str(),
                 ctime->tm_year + 1900, ctime->tm_mon + 1, ctime->tm_mday,
                 ctime->tm_hour, ctime->tm_min, ctime->tm_sec);

        va_list s;
        va_start(s, format);
        char rightbuffer[SIZE];
        vsnprintf(rightbuffer, sizeof(rightbuffer), format, s);
        va_end(s);

        // 格式：默认部分+自定义部分
        char logtxt[SIZE * 2];
        snprintf(logtxt, sizeof(logtxt), "%s %s\n", leftbuffer, rightbuffer);
        // printf("%s", logtxt); // 打印
        printLog(level, logtxt);
    }

    void printLog(int level, const std::string &logtxt)
    {
        switch (printMethod)
        {
        case Screen:
            std::cout << logtxt << std::endl;
            break;
        case Onefile:
            printOneFile(LogFile, logtxt);
            break;
        case Classfile:
            printClassFile(level, logtxt);
            break;
        default:
            break;
        }
    }
    void printOneFile(const std::string &logname, const std::string &logtxt)
    {
        int fd = open(logname.c_str(), O_WRONLY | O_CREAT | O_APPEND, 0666); // log.txt
        if (fd < 0)
            return;
        write(fd, logtxt.c_str(), logtxt.size());
        close(fd);
    }
    void printClassFile(int level, const std::string &logtxt)
    {
        std::string filename = LogFile;
        filename += ".";
        filename += levelToString(level); // log.txt.Debug
        printOneFile(filename, logtxt);
    }
    void operator()(int level, const char *format, ...)
    {
        char leftbuffer[SIZE];
        time_t t = time(nullptr);
        struct tm *ctime = localtime(&t);

        snprintf(leftbuffer, sizeof(leftbuffer), "[%s][%d-%d-%d %d:%d:%d]", levelToString(level).c_str(),
                 ctime->tm_year + 1900, ctime->tm_mon + 1, ctime->tm_mday,
                 ctime->tm_hour, ctime->tm_min, ctime->tm_sec);

        va_list s;
        va_start(s, format);
        char rightbuffer[SIZE];
        vsnprintf(rightbuffer, sizeof(rightbuffer), format, s);
        va_end(s);

        // 格式：默认部分+自定义部分
        char logtxt[SIZE * 2];
        snprintf(logtxt, sizeof(logtxt), "%s %s\n", leftbuffer, rightbuffer);
        // printf("%s", logtxt); // 打印
        printLog(level, logtxt);
    }
    ~Log() {}
};

// 可变参数列表的使用
int sum(int n, ...)
{              // 可变参数列表，至少要有一个具体的参数
    va_list s; // char *
    va_start(s, n);

    int sum = 0;
    while (n)
    {
        sum += va_arg(s, int);
    }
    va_end(s);
    return sum;
}