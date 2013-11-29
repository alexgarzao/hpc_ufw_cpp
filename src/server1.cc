#include "ufw.h"
#include "multiply_numbers.h"

const unsigned int 	PORT			= 9999;
const unsigned int 	TOTAL_THREADS	= 1;

int main()
{
	UFW::UFW ufw;
	// Os set's indicam os .so para serem carregados. Cada .so e' um plugin que implementa uma interface...
//	ufw.SetSocketInterface("StandardSocketInterface");
	// ufw.SetServerModel("StandardServerModel");
	ufw.SetListenPort(1234); //  ou server_model->SetListenPort(xxx); ???
	ufw.SetConcurrencyModel(new UFW::ConcurrencyModel(PORT, TOTAL_THREADS, new MultiplyNumbers()));
	ufw.WaitRequests(); // Mas assim bloqueia.... nao seria melhor algo como start/stop, onde no start ele cria uma thread? Se bem que, por enqto, assim ta bom... KISS :-)
}
