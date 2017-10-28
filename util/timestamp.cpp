#include "timestamp.hpp"
#include <iostream>

#define USEC_PER_SEC 1000000
#define NSEC_PER_USEC 1000

Timestamp::Timestamp(){
	this->seconds = 0;
	this->microseconds = 0;
};

Timestamp::Timestamp(struct tsencode info)
{
	this->seconds = info.seconds;
	this->microseconds = info.microseconds;
}

Timestamp::Timestamp(struct timespec info)
{
	this->seconds = info.tv_sec;
	this->microseconds = info.tv_nsec/NSEC_PER_USEC;
}

Timestamp::Timestamp(struct timeval info)
{
	this->seconds = info.tv_sec;
	this->microseconds = info.tv_usec;
}

Timestamp::Timestamp(long int seconds, long int microseconds)
{
	this->seconds = seconds;
	this->microseconds = microseconds;
}

struct tsencode Timestamp::encode(void)
{
	struct tsencode info;
	info.seconds = this->seconds;
	info.microseconds = this->microseconds;

	return info;
}

struct timeval Timestamp::toTimeval(void)
{
	struct timeval _time;
	_time.tv_sec = this->seconds;
	_time.tv_usec = this->microseconds;
	return _time;
}

Timestamp Timestamp::operator + (const Timestamp& other)
{
	Timestamp res;
	uint64_t u_sec = this->microseconds + other.microseconds;
	res.seconds = (this->seconds + other.seconds) + u_sec/USEC_PER_SEC;
	res.microseconds =  u_sec % USEC_PER_SEC;
	return res;
}

Timestamp Timestamp::operator - (const Timestamp& other){
	Timestamp res;
	uint64_t u_sec = (this->microseconds - other.microseconds);
	res.seconds = (this->seconds - other.seconds);
	if(u_sec < 0){
	res.microseconds = u_sec;
		res.seconds -= 1;
		res.microseconds += USEC_PER_SEC;
	}
	return res;
}

bool Timestamp::operator == (const Timestamp& other)
{
	return 	(this->seconds == other.seconds) &&
			(this->microseconds == other.microseconds);
}

bool Timestamp::operator < (const Timestamp& other)
{
	if(this->seconds < other.seconds)
	{
		return true;
	}
	else if(this->seconds == other.seconds)
	{
		return this->microseconds < other.microseconds;
	}
	return false;
}

bool Timestamp::operator > (const Timestamp& other)
{
	if(this->seconds > other.seconds)
	{
		return true;
	}
	else if(this->seconds == other.seconds)
	{
		return this->microseconds > other.microseconds;
	}
	return false;
}

bool Timestamp::operator <= (const Timestamp& other)
{
	if(this->seconds < other.seconds)
	{
		return true;
	}
	else if(this->seconds == other.seconds)
	{
		return this->microseconds <= other.microseconds;
	}
	return false;
}

bool Timestamp::operator >= (const Timestamp& other)
{
	if(this->seconds > other.seconds)
	{
		return true;
	}
	else if(this->seconds == other.seconds)
	{
		return this->microseconds >= other.microseconds;
	}
	return false;
}

std::string Timestamp::toString(void) const{
	return std::string(std::to_string(this->seconds) + "." + std::to_string(this->microseconds));
}

std::string Timestamp::toDate(void) const{
	char            fmt[64], buf[64];
    struct timeval  tv;
    struct tm       *tm;
	tv.tv_sec = this->seconds;
	tv.tv_usec = this->microseconds;

    if((tm = localtime(&tv.tv_sec)) != NULL)
    {
            strftime(fmt, sizeof fmt, "%Y-%m-%d %H:%M:%S.%%06u %z", tm);
            snprintf(buf, sizeof buf, fmt, tv.tv_usec);
            return std::string(buf);
    }
	else {
		return std::string("");
	}
}
