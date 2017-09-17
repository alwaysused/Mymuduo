#ifndef LOGGING_H
#define LOGGING_H



#include <stdio.h>
#include <unistd.h>
#include <algorithm>
#include <string.h>
#include <string>
#include <time.h>
#include <sys/time.h>
using std::string;
namespace {
const int dataSize = 4000;
char digitch[]= "0123456789ABCDEF";
int formatInt(char* buff, const int val){
    char* p = buff;
    int t = val;
    do{
        int i = t % 10;
        t /= 10;
        *p++ = digitch[i];

    }while(t > 0);
    *p = '\0';
    std::reverse(buff,p);
    return p-buff;

}
}

class LogStream{
public:
LogStream(){curr_ = data_;}

void append(const char* ptch, size_t size){
    if(avail() > size){
        memcpy(curr_, ptch, size);
        curr_ += size;
    }
}
int avail(){return data_ + dataSize - curr_;}

LogStream& operator<<(int i){
    int len = formatInt(curr_, i);
    curr_ += len;
    return *this;
}
const char* data(){return data_;}
int size(){return curr_-data_;}
    char data_[dataSize];
    char* curr_;

};
namespace {
LogStream& operator<<(LogStream& s, const string& str){
    s.append(str.c_str(), str.size());
    return s;
}


typedef void (*Output)(const char* ,int );
void defaultOp(const char* p, int len){fwrite(p, 1, len, stdout);}
Output op = defaultOp;

void setOutput(Output op_){op = op_;}
typedef void (*Flush)();

void defaultFlush(){fflush(stdout);}
Flush fh = defaultFlush;

}
class Logging{
public:

enum LogLevel
  {
    TRACE,
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL,
    NUM_LOG_LEVELS,
  };
Logging(const char* p , int line , LogLevel level):
    stream_(),filename_(p),line_(line),level_(level){formatTime();}
LogStream& stream(){return stream_;}
~Logging(){stream_<<filename_<<line_;op(stream_.data(), stream_.size()); if (level_ == FATAL) {fh();abort();}}
void formatTime(){
    struct timeval tv;
  gettimeofday(&tv, NULL);
  time_t seconds = tv.tv_sec;
  struct tm tm;
  gmtime_r(&seconds, &tm);
  char chtime[30];
  strftime(chtime, sizeof chtime, "%Y%m%d-%H%M%S.", &tm);
  stream_<<chtime;
  char useconds[10];
  snprintf(useconds, 10, "%06ld", tv.tv_usec);
  stream_<<useconds;
  stream_<<"  ";
  stream_<<LogLevelName[level_];




}
const char* LogLevelName[Logging::NUM_LOG_LEVELS] =
{
  "TRACE ",
  "DEBUG ",
  "INFO  ",
  "WARN  ",
  "ERROR ",
  "FATAL ",
};
LogStream stream_;
const char* filename_;
const int line_;
LogLevel level_;

};
namespace{
Logging::LogLevel initLogLevel(){
    if (::getenv("MUDUO_LOG_TRACE"))
    return Logging::TRACE;
  else if (::getenv("MUDUO_LOG_DEBUG"))
    return Logging::DEBUG;
  else
    return Logging::INFO;

}
Logging::LogLevel g_LogLevel = initLogLevel();
Logging::LogLevel logLevel(){return g_LogLevel;}
}
#define LOG_INFO if ( logLevel() <= Logging::INFO) Logging(__FILE__,__LINE__,Logging::INFO).stream()
#define LOG_FATAL Logging(__FILE__,__LINE__,Logging::FATAL).stream()

#endif // LOGGING_H

