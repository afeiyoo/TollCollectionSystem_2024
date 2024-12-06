/*---------------------------------------------------------------------------
Module:通用日志工具
Author:Huanglr
Version:
V0.1 2015-04-06 开始版本
V0.2 2015-08-11 增加删除历史日志,创建log目录
---------------------------------------------------------------------------*/
#ifndef Comm_Logger_H
#define Comm_Logger_H
#include <QtCore>
#include <QMutex>
#include "stdio.h"
#include <string.h>
#include <time.h>
#include <iostream>
#include <fstream>
using namespace std;

#define COMM_LOG_TRACE  -1
#define COMM_LOG_DEBUG  0
#define COMM_LOG_INFO   1
#define COMM_LOG_WARN   2
#define COMM_LOG_ERROR  3
#define COMM_LOG_FATA   4
#define MAX_LOG_LEN     4096

//---------------------------------------------------------------------------
class CommLogger{
private:
   //ofstream m_of;
   FILE *m_fp;
   QMutex m_mutex;
   char m_CurLogFile[1024];
   char m_log_file[1024];
   int  m_def_log_level;
   int  m_save_days;
   void getlogfile(char *logFile);
   void GetFmtDate(int nType,char *fmtDate);
   void WriteLogToFile(const char *level,char *log);
   void DeleteHisLogFile(int nType);
public:
   CommLogger(const char *log_file="dtslog",int def_log_level=COMM_LOG_TRACE);
   void setlevel(int nLevel);
   void setlogfile(const char *logfile);
   void setsavedays(int days);
   void trace(const char *fmt,...);
   void debug(const char *fmt,...);
   void info(const char *fmt,...);
   void warn(const char *fmt,...);
   void error(const char *fmt,...);
   void WriteLog(int nLevel,const char *fmt,...);
};


void LOG_SET_LEVEL(int nLevel);
void LOG_SET_FILE(const char *logfile);
void LOG_SET_SAVEDAYS(int days);

void LOG_TRACE(const char *fmt,...) ;
void LOG_ERR(const char *fmt,...);
void LOG_WARN(const char *fmt,...);
void LOG_INFO(const char *fmt,...);
void LOG_DEBUG(const char *fmt,...);

#endif
