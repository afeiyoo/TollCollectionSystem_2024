#include "CommLogger.h"
#include <QThread>
#include <QDateTime>

#ifndef __int64
typedef long long __int64;
#endif
//---------------------------------------------------------------------------

void CommLogger::GetFmtDate(int nType,char *fmtDate)
{
   /*time_t now=time(NULL);
   struct tm *nowinfo=localtime(&now);
   if (nType == 0)
      strftime(fmtDate,20,".%m%d",nowinfo);
   else
      strftime(fmtDate,30,"%Y-%m-%d %H:%M:%S",nowinfo);*/
   /*SYSTEMTIME systm;
   GetLocalTime(&systm);
   if(nType==0)
      sprintf(fmtDate,".%02d%02d",systm.wMonth,systm.wDay);
   else
      sprintf(fmtDate,"%02d:%02d:%02d.%03d",systm.wHour,systm.wMinute,systm.wSecond,systm.wMilliseconds);*/
   if(nType==0)
      strcpy(fmtDate,QDateTime::currentDateTime().toString(".MMdd").toStdString().c_str());
   else
       strcpy(fmtDate,QDateTime::currentDateTime().toString("HH:mm:ss.zzz").toStdString().c_str());
}

void CommLogger::getlogfile(char *logFile)
{
   char *ext = strstr(m_log_file,".");
   int nExtLen=0;
   if(ext) nExtLen=strlen(ext);
   strncpy(logFile,m_log_file,strlen(m_log_file) - nExtLen);
   char fmtday[30]={0};
   GetFmtDate(0,fmtday);
   strcat(logFile,fmtday);
   strcat(logFile,".log");
}

void CommLogger::DeleteHisLogFile(int nType)
{
   //计算历史日期
   char fmtDate[30]={0};
   time_t hisTime=time(NULL) - m_save_days*24*3600;
   struct tm *nowinfo=localtime(&hisTime);
   if (nType == 0)
      strftime(fmtDate,20,".%m%d",nowinfo);
   else
      strftime(fmtDate,30,"%Y-%m-%d %H:%M:%S",nowinfo);
   //组合历史文件
   char hisFile[1024]={0};
   char *ext = strstr(m_log_file,".");
   int nExtLen = 0;
   if(ext) nExtLen = strlen(ext);
   strncpy(hisFile,m_log_file,strlen(m_log_file) - nExtLen);
   strcat(hisFile,fmtDate);
   strcat(hisFile,".log");
   //删除
   remove(hisFile);
}

void CommLogger::setlogfile(const char *logfile)
{
   memset(m_log_file,0,1024);
   if(NULL == logfile) return ;
   strcpy(m_log_file,logfile);
}

CommLogger::CommLogger(const char *log_file,int def_log_level)
{
   m_fp = NULL;
   m_def_log_level = def_log_level;
   setlogfile(log_file);
   memset(m_CurLogFile,0,1024);
   m_save_days = 60;
}

void CommLogger::setlevel(int nLevel)
{
   m_def_log_level = nLevel;
}

void CommLogger::WriteLogToFile(const char *level,char *log)
{
   m_mutex.lock();
   try{
      char sTemp[1024]={0};
      getlogfile(sTemp);
      if(strcmp(m_CurLogFile,sTemp)!=0){
         if(m_fp) { fclose(m_fp); m_fp = NULL;}
         m_fp = fopen(sTemp,"a+");
         memset(m_CurLogFile,0,1024);
         strcpy(m_CurLogFile,sTemp);
         DeleteHisLogFile(0);
      }
      char fmtDate[50]={0};
      GetFmtDate(1,fmtDate);
      fprintf(m_fp,"%s [%s:%lld] %s\r\n",fmtDate,level,(long long)QThread::currentThreadId(),log);
      fflush(m_fp);
   }
   catch(...){
   }
   m_mutex.unlock();
   /*if(strcmpi(m_CurLogFile,sTemp)!=0){
       if(strlen(m_CurLogFile)>0) m_of.close();
       m_of.open(sTemp,ios::out|ios::app);
       memset(m_CurLogFile,0,1024);
       strcpy(m_CurLogFile,sTemp);
   }
   char fmtDate[50]={0};
   GetFmtDate(1,fmtDate);
   m_of<<fmtDate<<" ["<<level<<"]  "<<log<<endl;
   m_of.flush();*/
   //m_of.close();
}

void CommLogger::trace(const char *fmt,...)
{
   if(m_def_log_level>COMM_LOG_TRACE) return;
   char sLog[MAX_LOG_LEN+1]={0};
   va_list  argp;
   va_start(argp,fmt);
   vsnprintf(sLog,MAX_LOG_LEN,fmt,argp);
   va_end(argp);
   WriteLogToFile("TRACE",sLog);
}

void CommLogger::debug(const char *fmt,...)
{
   if(m_def_log_level>COMM_LOG_DEBUG) return;
   char sLog[MAX_LOG_LEN+1]={0};
   va_list  argp;
   va_start(argp,fmt);
   vsnprintf(sLog,MAX_LOG_LEN,fmt,argp);
   va_end(argp);
   WriteLogToFile("DEBUG",sLog);
}

void CommLogger::info(const char *fmt,...)
{
   if(m_def_log_level>COMM_LOG_INFO)
        return;
   char sLog[MAX_LOG_LEN+1]={0};
   va_list  argp;
   va_start(argp,fmt);
   vsnprintf(sLog,MAX_LOG_LEN,fmt,argp);
   va_end(argp);
   WriteLogToFile("INFO",sLog);
}

void CommLogger::warn(const char *fmt,...)
{
   if(m_def_log_level>COMM_LOG_WARN) return;
   char sLog[MAX_LOG_LEN+1]={0};
   va_list  argp;
   va_start(argp,fmt);
   vsnprintf(sLog,MAX_LOG_LEN,fmt,argp);
   va_end(argp);
   WriteLogToFile("WARN",sLog);
}

void CommLogger::error(const char *fmt,...)
{
   if(m_def_log_level>COMM_LOG_ERROR) return;
   char sLog[MAX_LOG_LEN+1]={0};
   va_list  argp;
   va_start(argp,fmt);
   vsnprintf(sLog,MAX_LOG_LEN,fmt,argp);
   va_end(argp);
   WriteLogToFile("ERROR",sLog);
}

void CommLogger::WriteLog(int nLevel,const char *fmt,...)
{
    if(m_def_log_level>COMM_LOG_ERROR)
        return;
    char sLog[MAX_LOG_LEN+1]={0};
    va_list  argp;
    va_start(argp,fmt);
    vsnprintf(sLog,MAX_LOG_LEN,fmt,argp);
    va_end(argp);

    switch(nLevel){
       case COMM_LOG_DEBUG:
          WriteLogToFile("DEBUG",sLog);
        break;
    case COMM_LOG_INFO:
       WriteLogToFile("INFO",sLog);
     break;
    case COMM_LOG_WARN:
       WriteLogToFile("WARN",sLog);
     break;
    case COMM_LOG_ERROR:
       WriteLogToFile("ERROR",sLog);
     break;
    }
}

void CommLogger::setsavedays(int days)
{
    if(days>360) days=30;
    m_save_days = days;
}

CommLogger gLogger;

void LOG_SET_LEVEL(int nLevel)
{
    gLogger.setlevel(nLevel);
}

void LOG_SET_FILE(const char *logfile)
{
    gLogger.setlogfile(logfile);
}

void LOG_SET_SAVEDAYS(int days)
{
    gLogger.setsavedays(days);
}
void LOG_TRACE(const char *fmt,...)
{
    char sLog[MAX_LOG_LEN+1]={0};
    va_list  argp;
    va_start(argp,fmt);
    vsnprintf(sLog,MAX_LOG_LEN,fmt,argp);
    gLogger.trace("%s",sLog);
    va_end(argp);

}
void LOG_ERR(const char *fmt,...)
{
    char sLog[MAX_LOG_LEN+1]={0};
    va_list  argp;
    va_start(argp,fmt);
    vsnprintf(sLog,MAX_LOG_LEN,fmt,argp);
    gLogger.error("%s",sLog);
    va_end(argp);

}
void LOG_WARN(const char *fmt,...)
{
    char sLog[MAX_LOG_LEN+1]={0};
    va_list  argp;
    va_start(argp,fmt);
    vsnprintf(sLog,MAX_LOG_LEN,fmt,argp);
    gLogger.warn("%s",sLog);
    va_end(argp);

}
void LOG_INFO(const char *fmt,...)
{
    char sLog[MAX_LOG_LEN+1]={0};
    va_list  argp;
    va_start(argp,fmt);
    vsnprintf(sLog,MAX_LOG_LEN,fmt,argp);
    gLogger.info("%s",sLog);
    va_end(argp);
}
void LOG_DEBUG(const char *fmt,...)
{
    char sLog[MAX_LOG_LEN+1]={0};
    va_list  argp;
    va_start(argp,fmt);
    vsnprintf(sLog,MAX_LOG_LEN,fmt,argp);
    gLogger.debug("%s",sLog);
    va_end(argp);
}
