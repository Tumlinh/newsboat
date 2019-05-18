#include "scopemeasure.h"

#include <sys/time.h>

namespace newsboat {

ScopeMeasure::ScopeMeasure(const std::string& func, Level ll)
	: funcname(func)
	, lvl(ll)
{
	gettimeofday(&tv1, nullptr);
}

void ScopeMeasure::stopover(const std::string& son)
{
	gettimeofday(&tv2, nullptr);
	unsigned long diff =
		(((tv2.tv_sec - tv1.tv_sec) * 1000000) + tv2.tv_usec) -
		tv1.tv_usec;
	LOG(lvl,
		"ScopeMeasure: function `%s' (stop over `%s') took %lu.%06lu "
		"s so "
		"far",
		funcname,
		son,
		diff / 1000000,
		diff % 1000000);
}

ScopeMeasure::~ScopeMeasure()
{
	gettimeofday(&tv2, nullptr);
	unsigned long diff =
		(((tv2.tv_sec - tv1.tv_sec) * 1000000) + tv2.tv_usec) -
		tv1.tv_usec;
	LOG(Level::INFO,
		"ScopeMeasure: function `%s' took %lu.%06lu s",
		funcname,
		diff / 1000000,
		diff % 1000000);
}

} // namespace newsboat
