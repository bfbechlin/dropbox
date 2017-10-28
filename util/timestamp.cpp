#include "timestamp.hpp"

#define USEC_PER_SEC 1000000

Timestamp::Timestamp(){
	this->seconds = 0;
	this->microseconds = 0;
};

Timestamp::Timestamp(timestamp_encode info)
{
	this->seconds = info.seconds;
	this->microseconds = info.microseconds;
}

Timestamp::Timestamp(struct timespec info)
{
	this->seconds = info.tv_sec;
	this->microseconds = info.tv_nsec/100;
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

timestamp_encode Timestamp::encode(void)
{
	timestamp_encode info;
	info.seconds = this->seconds;
	info.microseconds = this->microseconds;

	return info;
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
	res.microseconds = u_sec;
	if(u_sec < 0){
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
