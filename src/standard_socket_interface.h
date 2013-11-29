
namespace UFW {

// standard_socket_interface.h
// A partir da classe abaixo sera gerada .so que sera carregado no ufw.SetSocketInterface
// Classe abstrata com metodos como bind, listen, accept, recv, send, close, ...
// Com isso, para um plugin lidar com sockets, ele tem que implementar estes metodos...
class StandardSocketInterface : public UFW::SocketInterface
{
	bind() ...
	listen() ...
	UFW::Socket accept() ...
	int recv(UFW::Socket &socket, UFW::Buffer &buffer, size_t max_size) ...
	int send(UFW::Socket &socket, UFW::Buffer &buffer, size_t size) ...
	int close(UFW::Socket &socket) ...
};

}



Para alocar memoria, chama callbacks...
Para criar threads, chama callbacks...
As callbacks direcionam para metodos/funcoes que foram implementados nos plugins.

DUVIDAS:
* Ver padrao do google, llvm e gcc (4.8)