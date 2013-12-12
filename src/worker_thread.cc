#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include <ev.h>

#include <iostream>

#include "concurrency_model.h"


namespace UFW {

const unsigned int 	BUFFER_SIZE				= 1024U;


WorkerThread::WorkerThread(UFW::ITask *task) :
	task_(task)
{
	std::cout << "Iniciando " << __FUNCTION__ << std::endl;

	loop_ 		= ev_loop_new(EVFLAG_AUTO);
	assert(loop_ != NULL);
}


int
WorkerThread::start()
{
	// Cria a thread
	thread_ = new boost::thread(*this);

	return 0;
}


void
WorkerThread::operator()()
{
	std::cout << "Iniciando " << __FUNCTION__ << std::endl;

	// Start infinite loop
	while(1) {
		ev_run(loop_, 0);
		// ev_loop(loop_, 0);
	}
}


int
WorkerThread::stop()
{
	assert(0); // TODO

	return 0;
}


void
WorkerThread::join()
{
	thread_->join();
}


// Accept client requests
void
WorkerThread::add_watcher(int fd)
{
	std::cout << "Iniciando " << __FUNCTION__ << std::endl;

	struct ev_io 		*watcher;

	watcher 	= (struct ev_io*) malloc (sizeof(struct ev_io));
	assert(watcher != NULL);

	ev_io_init(watcher, WorkerThread::read_cb_wrapper_, fd, EV_READ);
	watcher->data = this;

	if (read_cb_(loop_, watcher, EV_READ) == 0) {
		ev_io_start(loop_, watcher);
	}
}


// Read client message (wrapper)
void
WorkerThread::read_cb_wrapper_(struct ev_loop *loop, struct ev_io *watcher, int revents)
{
	WorkerThread *worker_thread = static_cast<WorkerThread*>(watcher->data);
	worker_thread->read_cb_(loop, watcher, revents);
}


// Read client message
int
WorkerThread::read_cb_(struct ev_loop *loop, struct ev_io *watcher, int revents)
{
	std::cout << "Iniciando " << __FUNCTION__ << std::endl;

	char 	buffer[task_->packet_size()];
	ssize_t read;

	if(EV_ERROR & revents) {
		ev_io_stop(loop,watcher);
		close(watcher->fd);
		free(watcher);
		perror("got invalid event");
		return 1;
	} else if(EV_READ & revents) {
		do {
			// Receive message from client socket
			read = recv(watcher->fd, buffer, task_->packet_size(), 0);

			if(read < 0) {
				if (errno != EAGAIN && errno != EINTR) {
					ev_io_stop(loop,watcher);
					close(watcher->fd);
					free(watcher);
					perror("read error");
					break;
				}

				return 0;
			} else if(read == 0) {
				// Stop and free watchet if client socket is closing
				ev_io_stop(loop,watcher);
				close(watcher->fd);
				free(watcher);
				perror("peer might closing");
				break;
			} else {
				assert(read == task_->packet_size());
				// printf("message:%s\n",buffer);
				(*task_)(buffer, read, watcher->fd);
			}
		} while(true);
	}

	return 1;
}


} // namespace UFW
