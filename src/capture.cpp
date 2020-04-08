#include <stdio.h>
#include "hiredis.h"

int main ()
{
	redisReply *reply;
	int port = 6379;

	redisContext *c = redisConnect("localhost", port);
	if (c != NULL && c->err) {
		printf("Error: %s\n", c->errstr);
		// handle error
	} else {
		printf("Connected to Redis\n");
	}

	reply = redisCommand(c,"SET %s %s","foo","bar");
	freeReplyObject(reply);

	reply = redisCommand(c,"GET %s","foo");
	printf("%s\n",reply->str);
	freeReplyObject(reply);

	redisFree(c);
}
