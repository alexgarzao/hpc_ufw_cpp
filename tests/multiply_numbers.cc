#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <strings.h>

#include "multiply_numbers.h"
#include "protocol1.h"

size_t
MultiplyNumbers::operator()(UFW::TaskInfo *task_info, int fd)
{
	if (task_info->data_len < packet_size()) {
		return 0;
	}

	message_request_t 	*message_request = (message_request_t*)task_info->buffer;
	message_reply_t 	message_reply;

	bzero(&message_reply, sizeof(message_reply_t));
	message_reply.result = message_request->value1 * message_request->value2;
	// Send message back to the client
	send(fd, &message_reply, sizeof(message_reply_t), 0);

	return packet_size();
}


unsigned int
MultiplyNumbers::packet_size()
{
	return sizeof(message_request_t);
}
