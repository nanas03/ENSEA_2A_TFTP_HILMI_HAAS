#include <stdio.h>
#include <stdlib.h>

int main(int argc, char * argv[]) {
	// We need at least 3 args
	if (argc < 3) {
		// There are less than 3 arguments
		printf("Error: not enough args\n");
		exit(EXIT_FAILURE);
	}
	printf("arg1: %s arg2: %s\n", argv[1], argv[2]);
	/*
	struct addrinfo *res;
	
	// Configure the servers
	hints.ai_family = AF_INET; // AF_INET = famille IPv4
	hints.ai_protocol = IPPROTO_UDP; // Protocol UDP
	
	int e = getaddrinfo(argv[1], argv[3], &hints, &res);
	if (e != 0) { // If getaddrinfo send an error
		fprintf(stderr, "getaddrinfo error %d\n", gai_strerror(e));
		exit(EXIT_FAILURE);
	}*/
}



