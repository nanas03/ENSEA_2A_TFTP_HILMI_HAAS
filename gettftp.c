#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 516
#define RRQ 1
#define DATA 3
#define ACK 4


int main(int argc, char *argv[]) {
	// We need at least 4 args: adress file port
	if (argc < 4) {
		// There are less than 4 arguments
		printf("Error: not enough args\n");
        exit(EXIT_FAILURE);
    }
    
    struct addrinfo *res, hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;        // IPv4
    hints.ai_socktype = SOCK_DGRAM;   // Socket UDP
    hints.ai_protocol = IPPROTO_UDP;  // Protocole UDP
    // address resolution
    int e = getaddrinfo(argv[1], argv[3], &hints, &res);
    if (e != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(e));
        exit(EXIT_FAILURE);
    }
    
    // Create a socket
    int sock;
    if ((sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) < 0) {
		perror("socket error");
        exit(EXIT_FAILURE);
    }

    printf("Send request\n");
    
    // Send request
    char buf[BUFFER_SIZE];
    memset(buf, 0, BUFFER_SIZE);
    buf[0] = 0; buf[1] = RRQ; // Opcode RRQ
    strcpy(&buf[2], argv[2]);    // Nom du fichier
    strcpy(&buf[2 + strlen(argv[2]) + 1], "octet"); // Mode "octet"
    // Envoi de la requête RRQ
    if (sendto(sock, buf, 2 + strlen(argv[2]) + 1 + 6, 0, res->ai_addr, res->ai_addrlen) < 0) {
        perror("sendto error");
        exit(EXIT_FAILURE);
    }
    
      printf("Receive the file %s\n", argv[2]);

    // Received
    struct sockaddr_in server_addr;
    socklen_t server_addr_len = sizeof(server_addr);
    int num_packet = 0;
    FILE *fp = fopen("output_file", "wb"); // Create the received_file
    if (!fp) {
        perror("File error");
        exit(EXIT_FAILURE);
    }
    
     while (1) {
        // Received
        memset(buf, 0, BUFFER_SIZE);
        ssize_t recv_size = recvfrom(sock, buf, BUFFER_SIZE, 0, (struct sockaddr*)&server_addr, &server_addr_len);
        if (recv_size < 0) {
            perror("recvfrom error");
            fclose(fp);
            exit(EXIT_FAILURE);
        }
        // Check if the packet is indeed a file
        if (buf[1] == DATA) {
            num_packet = (buf[2] << 8) | buf[3];
            printf("packet number: %d, %ld octets received\n", num_packet, recv_size - 4);

            // Write in the file
            fwrite(&buf[4], 1, recv_size - 4, fp);
            
            // Send ACK for the block n°block_num
            char ack[4] = {0, ACK, buf[2], buf[3]};
            if (sendto(sock, ack, sizeof(ack), 0, (struct sockaddr*)&server_addr, server_addr_len) < 0) {
                perror("sendto error");
                fclose(fp);
                exit(EXIT_FAILURE);
            }
            printf("Send ACK for the packet n°%d\n", num_packet);
            if (recv_size < BUFFER_SIZE) {
                printf("Transfer complete.\n");
                break;
            }
        }
    }
    fclose(fp);
    freeaddrinfo(res); // Free the memory
    close(sock);
    printf("success Q4\n");
	exit(EXIT_SUCCESS);
}
            
            
            
            
