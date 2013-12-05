#ifndef __UFW__ITASK__H__
#define __UFW__ITASK__H__

#include <cstddef>

namespace UFW {

class ITask
{
	public:
		virtual void operator()(void *protocol, std::size_t size, int fd) = 0;
};
	
} // namespace UFW

#endif
