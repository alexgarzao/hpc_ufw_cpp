#ifndef __UFW__ITASK__H__
#define __UFW__ITASK__H__

#include <ev.h>

#include <cstddef>

namespace UFW {

const unsigned int 	BUFFER_SIZE				= 1024U;

struct TaskInfo {
		struct ev_io 		watcher;
		char				buffer[BUFFER_SIZE];
		size_t 				data_len;
};

class ITask
{
	public:
		virtual size_t operator()(TaskInfo *, int) = 0;
		virtual unsigned int packet_size() = 0;
};
	
} // namespace UFW

#endif
