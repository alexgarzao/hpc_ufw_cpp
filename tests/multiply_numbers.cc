#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <strings.h>

#include "multiply_numbers.h"
#include "protocol1.h"

void MultiplyNumbers::execute(void *protocol, std::size_t size, int fd)
{
	message_request_t 	*message_request = (message_request_t*)protocol;
	message_reply_t 	message_reply;
	bzero(&message_reply, sizeof(message_reply_t));
	message_reply.result = message_request->value1 * message_request->value2;
	// Send message back to the client
	send(fd, &message_reply, sizeof(message_reply_t), 0);
}