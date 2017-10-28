#ifndef __TIMESTAMP_HPP__
#define __TIMESTAMP_HPP__
#include <ostream>

#include <time.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <stdint.h>

#define TIMESTAMP_LEN sizeof(timestamp_encode)
#define TIMEVAL_ACCESS 0
#define TIMEVAL_MODIFICATION 1

/* Used for transmission data */
typedef struct _codTimesStamp{
	uint64_t seconds;
	uint64_t microseconds;
}timestamp_encode;


class Timestamp
{
	private:
		uint64_t seconds;
		uint64_t microseconds;

	public:
		Timestamp();
		Timestamp(timestamp_encode info);
		Timestamp(struct timespec info);
		Timestamp(struct timeval info);
		Timestamp(long int seconds, long int microseconds);

		timestamp_encode encode(void);
		struct timeval toTimeval(void);

		Timestamp operator + (const Timestamp& other);
		Timestamp operator - (const Timestamp& other);
		bool operator == (const Timestamp& other);
		bool operator < (const Timestamp& other);
		bool operator > (const Timestamp& other);
		bool operator <= (const Timestamp& other);
		bool operator >= (const Timestamp& other);

		std::string toString(void) const;
		std::string toDate(void) const;

};

#endif
