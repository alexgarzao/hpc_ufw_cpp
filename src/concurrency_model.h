#ifndef __UFW__CONCURRENCY_MODEL__H__
#define __UFW__CONCURRENCY_MODEL__H__

#include <sys/types.h>
#include <stdint.h>
#include "iconcurrency_model.h"
#include "itask.h"

namespace UFW {

class ConcurrencyModel : public UFW::IConcurrencyModel // cria threads, cria epolls, faz bind/listen/accept, repassa socket para uma worker thread
{
	public:
		ConcurrencyModel(const unsigned int &port, const unsigned int &worker_threads, UFW::ITask *task);
		int start();
		int wait_requests();
		static void accept_cb(struct ev_loop *loop, struct ev_io *watcher, int revents);
		static void read_cb(struct ev_loop *loop, struct ev_io *watcher, int revents);
		void run_task(void *buffer, ssize_t read, int fd);
	private:
		static ConcurrencyModel 	*instance_;
		unsigned int 				port_;
		unsigned int				worker_threads_;
		UFW::ITask 					*task_;
};

} // namespace UFW

#endif
