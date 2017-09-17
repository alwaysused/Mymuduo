#ifndef TIMESTAMP_H_
#define TIMESTAMP_H_
#include <stdint.h>
#include <string>

class Timestamp{
 public:
  Timestamp(): microSecondsSinceEpoch_(0){}
  
  explicit Timestamp(int64_t microSecondsSinceEpochArg)
    : microSecondsSinceEpoch_(microSecondsSinceEpochArg)
  {
  }
  static Timestamp delay(double sec);
  std::string toString();
	int64_t microSecondsSinceEpoch()const {return microSecondsSinceEpoch_;}
  	static Timestamp now();
    std::string toFormattedString(bool showKmicro = true);
	bool valid(){return microSecondsSinceEpoch_>0;}
	static Timestamp invalid();
	static const int kMicroSecondsPerSecond = 1000* 1000;
 private:

  int64_t microSecondsSinceEpoch_;

	


};
//bool operator<(TimeStamp ,Timestamp);
inline bool operator<(Timestamp lhs, Timestamp rhs)
{
  return lhs.microSecondsSinceEpoch() < rhs.microSecondsSinceEpoch();
}
#endif
