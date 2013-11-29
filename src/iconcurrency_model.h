#ifndef __UFW__ICONCURRENCY_MODEL__H__
#define __UFW__ICONCURRENCY_MODEL__H__

namespace UFW {

class IConcurrencyModel
{
	public:
		virtual int start() = 0;
		virtual int wait_requests() = 0;
};

}

#endif
