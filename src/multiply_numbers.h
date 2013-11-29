#ifndef __UFW__MULTIPLY_NUMBERS__H__
#define __UFW__MULTIPLY_NUMBERS__H__

#include "itask.h"

class MultiplyNumbers : public UFW::ITask
{
	public:
		void execute(void *protocol, std::size_t size);
};

#endif
