#ifndef __UFW__ITASK__H__
#define __UFW__ITASK__H__

#include <cstddef>

namespace UFW {

class ITask
{
	public:
		virtual void execute(void *protocol, std::size_t size, int fd) = 0;
};
	
}

#endif
