#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include <ev.h>

#include <iostream>
#include <boost/thread.hpp>

#include "concurrency_model.h"


namespace UFW {


ConcurrencyModel::ConcurrencyModel(const unsigned int &port, const unsigned int &worker_threads, UFW::ITask *task) :
	port_(port),
	worker_threads_(worker_threads),
	task_(task),
	last_thread_(0)
{
}


int
ConcurrencyModel::start()
{
	struct ev_loop 		*loop;
	int 				sd;
	struct sockaddr_in 	addr;
	int 				addr_len;
	struct ev_io 		w_accept;

	loop 		= ev_default_loop(EVFLAG_AUTO);
	addr_len 	= sizeof(addr);

	// Start threads
	start_threads_();

	// Create server socket
	if( (sd = socket(PF_INET, SOCK_STREAM, 0)) < 0 ) {
		perror("socket error");
		return -1;
	}

	bzero(&addr, sizeof(addr));
	addr.sin_family 		= AF_INET;
	addr.sin_port 			= htons(port_);
	addr.sin_addr.s_addr 	= INADDR_ANY;

	// Bind socket to address
	if (bind(sd, (struct sockaddr*) &addr, sizeof(addr)) != 0) {
		perror("bind error");
	}

    // fcntl(sd, F_SETFL, fcntl(sd, F_GETFL, 0) | O_NONBLOCK); 

	// Start listing on server socket
	if (listen(sd, 2) < 0) { // TODO: qual valor backlog???
		perror("listen error");
		return -1;
	}

	// Initialize and start a watcher to accepts client requests
	ev_io_init(&w_accept, ConcurrencyModel::accept_cb_wrapper_, sd, EV_READ);
	w_accept.data = this;
	ev_io_start(loop, &w_accept);

	// Start infinite loop
	while(1) {
		// ev_loop(loop, 0);
		ev_run(loop, 0);
	}
}


int
ConcurrencyModel::wait_requests()
{
	return 0;
}


// Accept client requests
void
ConcurrencyModel::accept_cb_wrapper_(struct ev_loop *loop, struct ev_io *watcher, int revents)
{
	std::cout << "Iniciando " << __FUNCTION__ << std::endl;

	ConcurrencyModel *concurrency_model = static_cast<ConcurrencyModel*>(watcher->data);
	concurrency_model->accept_cb_(loop, watcher, revents);
}


void
ConcurrencyModel::accept_cb_(struct ev_loop *loop, struct ev_io *watcher, int revents)
{
	std::cout << "Iniciando " << __FUNCTION__ << std::endl;

	struct sockaddr_in 	client_addr;
	socklen_t 			client_len;
	int 				client_sd;

	client_len 	= sizeof(client_addr);

	if(EV_ERROR & revents) {
	  perror("got invalid event");
	  return;
	}

	// Accept client request
	client_sd = accept(watcher->fd, (struct sockaddr *)&client_addr, &client_len);

	if (client_sd < 0) {
	  perror("accept error");
	  return;
	}

	// total_clients ++; // Increment total_clients count
	printf("Successfully connected with client.\n");
	// printf("%d client(s) connected.\n", total_clients);

	// Initialize and start watcher to read client requests
	worker_thread_list_[last_thread_]->add_watcher(client_sd);
	last_thread_ = (last_thread_ + 1) % worker_thread_list_.size();
}


int
ConcurrencyModel::start_threads_()
{
	std::cout << "Iniciando " << __FUNCTION__ << std::endl;

	std::cout << "Worker threads: " << worker_threads_ << std::endl;
	for(unsigned int thread_number = 0; thread_number < worker_threads_; thread_number++) {
		WorkerThread 	*worker_thread = new WorkerThread(task_);
		worker_thread->start();
		worker_thread_list_.push_back(worker_thread);
	}

	return 0;
}


} // namespace UFW
