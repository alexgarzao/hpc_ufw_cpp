#include <ev.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <strings.h>
#include <stdlib.h>

#include "concurrency_model.h"


namespace UFW {

const unsigned int 	BUFFER_SIZE = 1024U;


ConcurrencyModel* ConcurrencyModel::instance_ = NULL;


ConcurrencyModel::ConcurrencyModel(const unsigned int &port, const unsigned int &worker_threads, UFW::ITask *task) :
	port_(port),
	worker_threads_(worker_threads),
	task_(task)
{
	instance_ = this;
}


int ConcurrencyModel::start()
{
	struct ev_loop 		*loop;
	int 				sd;
	struct sockaddr_in 	addr;
	int 				addr_len;
	struct ev_io 		w_accept;

	loop 		= ev_default_loop(0);
	addr_len 	= sizeof(addr);

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

	// Start listing on server socket
	if (listen(sd, 2) < 0) { // TODO: qual valor backlog???
	  perror("listen error");
	  return -1;
	}

	// Initialize and start a watcher to accepts client requests
	ev_io_init(&w_accept, ConcurrencyModel::accept_cb, sd, EV_READ);
	ev_io_start(loop, &w_accept);

	// Start infinite loop
	while (1)
	{
	  ev_loop(loop, 0);
	}




	// listen/accept
	// repassa socket para uma worker thread (pelo epoll da thread)
		// acha thread para enviar
		// agenda timeout da operacao
		// envia para thread






	// Cria threads
	// cria epolls



}


int ConcurrencyModel::wait_requests()
{
	return 0;
}


// Accept client requests
void ConcurrencyModel::accept_cb(struct ev_loop *loop, struct ev_io *watcher, int revents)
{
	struct sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);
	int client_sd;
	struct ev_io *w_client = (struct ev_io*) malloc (sizeof(struct ev_io));

	if(EV_ERROR & revents)
	{
	  perror("got invalid event");
	  return;
	}

	// Accept client request
	client_sd = accept(watcher->fd, (struct sockaddr *)&client_addr, &client_len);

	if (client_sd < 0)
	{
	  perror("accept error");
	  return;
	}

	// total_clients ++; // Increment total_clients count
	printf("Successfully connected with client.\n");
	// printf("%d client(s) connected.\n", total_clients);

	// Initialize and start watcher to read client requests
	ev_io_init(w_client, ConcurrencyModel::read_cb, client_sd, EV_READ);
	ev_io_start(loop, w_client);
}


// Read client message
void ConcurrencyModel::read_cb(struct ev_loop *loop, struct ev_io *watcher, int revents)
{
	char buffer[BUFFER_SIZE];
	ssize_t read;

	if(EV_ERROR & revents)
	{
	  perror("got invalid event");
	  return;
	}

	// Receive message from client socket
	read = recv(watcher->fd, buffer, BUFFER_SIZE, 0);

	if(read < 0)
	{
	  perror("read error");
	  return;
	}

	if(read == 0)
	{
	  // Stop and free watchet if client socket is closing
	  ev_io_stop(loop,watcher);
	  free(watcher);
	  perror("peer might closing");
	  // total_clients --; // Decrement total_clients count
	  // printf("%d client(s) connected.\n", total_clients);
	  return;
	}
	else
	{
	  printf("message:%s\n",buffer);
	  ConcurrencyModel *concurrency_model = static_cast<ConcurrencyModel*>(instance_);
	  concurrency_model->run_task(buffer, read, watcher->fd);
	}

	bzero(buffer, read);
}


void ConcurrencyModel::run_task(void *buffer, ssize_t read, int fd)
{
	task_->execute(buffer, read, fd);
}


/* CODIGO COMPLETO...

#include <stdio.h>
#include <netinet/in.h>
#include <ev.h>

#define PORT_NO 3033
#define BUFFER_SIZE 1024

int total_clients = 0;  // Total number of connected clients

void accept_cb(struct ev_loop *loop, struct ev_io *watcher, int revents);
void read_cb(struct ev_loop *loop, struct ev_io *watcher, int revents);

int main()
{
	struct ev_loop *loop = ev_default_loop(0);
	int sd;
	struct sockaddr_in addr;
	int addr_len = sizeof(addr);
	struct ev_io w_accept;

	// Create server socket
	if( (sd = socket(PF_INET, SOCK_STREAM, 0)) < 0 )
	{
	  perror("socket error");
	  return -1;
	}

	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT_NO);
	addr.sin_addr.s_addr = INADDR_ANY;

	// Bind socket to address
	if (bind(sd, (struct sockaddr*) &addr, sizeof(addr)) != 0)
	{
	  perror("bind error");
	}

	// Start listing on the socket
	if (listen(sd, 2) < 0)
	{
	  perror("listen error");
	  return -1;
	}

	// Initialize and start a watcher to accepts client requests
	ev_io_init(&w_accept, accept_cb, sd, EV_READ);
	ev_io_start(loop, &w_accept);

	// Start infinite loop
	while (1)
	{
	  ev_loop(loop, 0);
	}

	return 0;
}

// Accept client requests
void accept_cb(struct ev_loop *loop, struct ev_io *watcher, int revents)
{
	struct sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);
	int client_sd;
	struct ev_io *w_client = (struct ev_io*) malloc (sizeof(struct ev_io));

	if(EV_ERROR & revents)
	{
	  perror("got invalid event");
	  return;
	}

	// Accept client request
	client_sd = accept(watcher->fd, (struct sockaddr *)&client_addr, &client_len);

	if (client_sd < 0)
	{
	  perror("accept error");
	  return;
	}

	total_clients ++; // Increment total_clients count
	printf("Successfully connected with client.\n");
	printf("%d client(s) connected.\n", total_clients);

	// Initialize and start watcher to read client requests
	ev_io_init(w_client, read_cb, client_sd, EV_READ);
	ev_io_start(loop, w_client);
}

// Read client message
void read_cb(struct ev_loop *loop, struct ev_io *watcher, int revents){
	char buffer[BUFFER_SIZE];
	ssize_t read;

	if(EV_ERROR & revents)
	{
	  perror("got invalid event");
	  return;
	}

	// Receive message from client socket
	read = recv(watcher->fd, buffer, BUFFER_SIZE, 0);

	if(read < 0)
	{
	  perror("read error");
	  return;
	}

	if(read == 0)
	{
	  // Stop and free watchet if client socket is closing
	  ev_io_stop(loop,watcher);
	  free(watcher);
	  perror("peer might closing");
	  total_clients --; // Decrement total_clients count
	  printf("%d client(s) connected.\n", total_clients);
	  return;
	}
	else
	{
	  printf("message:%s\n",buffer);
	}

	// Send message bach to the client
	send(watcher->fd, buffer, read, 0);
	bzero(buffer, read);
}
*/



// void ConcurrencyModel::io_accept(ev::io &watcher, int revents) {
//         if (EV_ERROR & revents) {
//                 perror("got invalid event");
//                 return;
//         }

//         struct sockaddr_in client_addr;
//         socklen_t client_len = sizeof(client_addr);

//         int client_sd = accept(watcher.fd, (struct sockaddr *)&client_addr, &client_len);

//         if (client_sd < 0) {
//                 perror("accept error");
//                 return;
//         }

//         EchoInstance *client = new EchoInstance(client_sd);
// }


}


/*
class EchoServer {
private:
        ev::io           io;
        ev::sig         sio;
        int                 s;
 
public:
 
        void io_accept(ev::io &watcher, int revents) {
                if (EV_ERROR & revents) {
                        perror("got invalid event");
                        return;
                }
 
                struct sockaddr_in client_addr;
                socklen_t client_len = sizeof(client_addr);
 
                int client_sd = accept(watcher.fd, (struct sockaddr *)&client_addr, &client_len);
 
                if (client_sd < 0) {
                        perror("accept error");
                        return;
                }
 
                EchoInstance *client = new EchoInstance(client_sd);
        }
 
        static void signal_cb(ev::sig &signal, int revents) {
                signal.loop.break_loop();
        }
 
        EchoServer(int port) {
                printf("Listening on port %d\n", port);
 
                struct sockaddr_in addr;
 
                s = socket(PF_INET, SOCK_STREAM, 0);
 
                addr.sin_family = AF_INET;
                addr.sin_port     = htons(port);
                addr.sin_addr.s_addr = INADDR_ANY;
 
                if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) != 0) {
                        perror("bind");
                }
 
                fcntl(s, F_SETFL, fcntl(s, F_GETFL, 0) | O_NONBLOCK); 
 
                listen(s, 5);
 
                io.set<EchoServer, &EchoServer::io_accept>(this);
                io.start(s, ev::READ);
 
                sio.set<&EchoServer::signal_cb>();
                sio.start(SIGINT);
        }
 
        virtual ~EchoServer() {
                shutdown(s, SHUT_RDWR);
                close(s);
        }
};
*/
