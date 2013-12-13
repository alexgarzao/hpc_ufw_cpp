#ifndef __UFW__MULTIPLY_NUMBERS__H__
#define __UFW__MULTIPLY_NUMBERS__H__

#include "itask.h"

class MultiplyNumbers : public UFW::ITask
{
	public:
		size_t operator()(UFW::TaskInfo *, int);
		unsigned int packet_size();
};

#endif
