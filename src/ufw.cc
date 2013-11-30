#include "ufw.h"

namespace UFW {

UFW::UFW()
{
}


void
UFW::SetConcurrencyModel(IConcurrencyModel *concurrency_model)
{
	concurrency_model_ = concurrency_model;
}


void
UFW::SetListenPort(int port)
{

}


void
UFW::WaitRequests()
{
	// Instancia o modelo de concorrencia
	concurrency_model_->start();
	concurrency_model_->wait_requests();
}

} // namespace UFW
