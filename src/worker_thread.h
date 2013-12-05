#ifndef __UFW__WORKER_THREAD__H__
#define __UFW__WORKER_THREAD__H__

#include <ev.h>
#include <boost/thread.hpp>
#include "itask.h"

namespace UFW {

class WorkerThread
{
	public:
		WorkerThread(UFW::ITask *task);
		int start();
		void operator()();
		int stop();
		void add_watcher(int fd);
		void join();
	private:
		boost::thread 			*thread_;
		static WorkerThread 	*instance_;
		UFW::ITask 				*task_;
		struct ev_loop 			*loop_;

		static void read_cb_wrapper_(struct ev_loop *loop, struct ev_io *watcher, int revents);
		void read_cb_(struct ev_loop *loop, struct ev_io *watcher, int revents);
};

} // namespace UFW

#endif
