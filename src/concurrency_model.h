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
	private:
		unsigned int 	port_;
		unsigned int	worker_threads_;
		UFW::ITask 		*task_;
};

}

#endif
