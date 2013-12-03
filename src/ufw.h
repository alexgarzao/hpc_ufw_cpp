#ifndef __UFW__UFW__H__
#define __UFW__UFW__H__

#include "itask.h"
#include "concurrency_model.h"

namespace UFW {

class UFW
{
	public:
		UFW();
		void SetConcurrencyModel(::UFW::IConcurrencyModel *concurrency_model);
		void SetListenPort(int port);
		void WaitRequests();
	private:
		::UFW::IConcurrencyModel 	*concurrency_model_;
};
	
} // namespace UFW

#endif
