#include "Timestamp.h"
#include <stdio.h>
#include <sys/time.h>
#include <inttypes.h>
Timestamp Timestamp::now()
{
	struct timeval tv;
    gettimeofday(&tv, NULL);
	int64_t seconds = tv. tv_sec;
	return Timestamp(seconds* kMicroSecondsPerSecond + tv.tv_usec);

}

Timestamp Timestamp::invalid(){return Timestamp();}
std::string Timestamp::toString()
{
    char buf[32] = {0};
      int64_t seconds = microSecondsSinceEpoch_ / kMicroSecondsPerSecond;
      int64_t microseconds = microSecondsSinceEpoch_ % kMicroSecondsPerSecond;
      snprintf(buf, sizeof(buf)-1, "%" PRId64 ".%06" PRId64 "", seconds, microseconds);
      return buf;
}

std::string Timestamp::toFormattedString(bool showKmicro)
{
	char buf[32];
	time_t seconds = static_cast<time_t> (microSecondsSinceEpoch_/kMicroSecondsPerSecond);
    struct tm tm_time;
	gmtime_r(&seconds, &tm_time);
    if (showKmicro)
	{
		int microseconds = static_cast<int> (microSecondsSinceEpoch_ % kMicroSecondsPerSecond);
		snprintf(buf, sizeof(buf), "%4d%02d%02d %02d:%02d:%02d.%06d",
             tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
             tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec,
             microseconds);
  }
  else
  {
    snprintf(buf, sizeof(buf), "%4d%02d%02d %02d:%02d:%02d",
             tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
             tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);
  }
  return buf;
	}

Timestamp Timestamp::delay(double sec){return Timestamp(Timestamp::now(). microSecondsSinceEpoch() + sec*kMicroSecondsPerSecond);}




