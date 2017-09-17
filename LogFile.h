#ifndef LOGFILE_H
#define LOGFILE_H

#include <assert.h>
#include <time.h>
#include <boost/scoped_ptr.hpp>
#include <string.h>
#include <stdio.h>
#include <libgen.h>
#include <string>


using std::string;



class AppendFile
{
public:
	AppendFile(const string& filename): 
	fp_(fopen(filename.c_str(), "ae")), writtenBytes_(0)
		{
			assert(fp_);
			setbuffer(fp_, buffer_, sizeof buffer_);}
	~AppendFile(){fclose(fp_);}
	void append(const char* ptch, int len){fwrite(ptch, 1, len, fp_);}
 private:
 	 char buffer_[60*1020];
 	 FILE* fp_;
 	 int writtenBytes_;
	
};
class LogFile
{
 public:
	 LogFile(const string& name,int num): basename_(name), lastRoll_(0), 
										lastFlush_(0), startOfPeriod_(0),
										rollBytes_(num){
										rollFile();}
	 ~ LogFile(){}
	void append(const char* ptch, int len){file_-> append(ptch, len);}
 private:
 	 bool rollFile(){
 	 	time_t now;
 	 	string filename = getLogName(basename_, &now);;
 	 	time_t start = now/ secondsPerRoll * secondsPerRoll;
 	 	if (now > lastRoll_){
 	 		lastRoll_ = now;
 	 		lastFlush_ = now;
 	 		startOfPeriod_ = start;
 	 		file_. reset(new AppendFile(filename));
			return true;
 	 	}
		return false;
 	 }
 	 string getLogName(const string& basename, time_t* now){
 	 	 string filename;
 	 	 filename. reserve((sizeof basename) +64);
 	 	 filename = basename;
 	 	 struct tm tm;
		*now = time(NULL);
  		gmtime_r(now, &tm);
  		char chtime[30];
  		strftime(chtime, sizeof chtime, ".%Y%m%d-%H%M%S.", &tm);
		filename += chtime;
  	
		//filename += PidInfo::hostname();
		filename += ".log";
		return filename;

 	 }
	 int lastRoll_;
	 int lastFlush_;
	 time_t startOfPeriod_;
 	 string basename_;
 	 int rollBytes_;
 	 boost::scoped_ptr<AppendFile> file_;

 	 const static int secondsPerRoll = 60 * 60 * 24; 
	
};


#endif
