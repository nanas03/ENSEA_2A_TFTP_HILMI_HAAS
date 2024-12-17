#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 516
// Opcodes
#define WRQ 2
#define DATA 3
#define ACK 4


int main(int argc, char *argv[]) {
    // Q1) We need 4 arguments: server address, file name, port
    if (argc < 4) {
        // There are less than 4 arguments
        printf("Error: not enough arguments\n");
        exit(EXIT_FAILURE);
    }

	// Q2)
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;        // IPv4
    hints.ai_socktype = SOCK_DGRAM;   // UDP socket
    hints.ai_protocol = IPPROTO_UDP;  // UDP protocol
   
    // address resolution
    int e = getaddrinfo(argv[1], argv[3], &hints, &res);
    if (e != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(e));
        exit(EXIT_FAILURE);
    }

    // Q3) Create a socket
    int sock;
    if ((sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) < 0) {
        perror("socket error");
        exit(EXIT_FAILURE);
    }
    // Q5.a) Send Request
    printf("Send request\n");
    char wrq[BUFFER_SIZE];
    memset(wrq, 0, BUFFER_SIZE);
    wrq[0] = 0; wrq[1] = WRQ; // Opcode Write
    strcpy(&wrq[2], argv[2]); // File name
    strcpy(&wrq[2 + strlen(argv[2]) + 1], "octet");

    // Send the request WRQ
    if (sendto(sock, wrq, 2 + strlen(argv[2]) + 1 + 5, 0, res->ai_addr, res->ai_addrlen) < 0) {
        perror("sendto error");
        exit(EXIT_FAILURE);
    }

    // Create the received_file
    FILE *fp = fopen(argv[2], "rb");
    if (!fp) {
        perror("open error");
        exit(EXIT_FAILURE);
    }
    struct sockaddr_in server_addr;
    socklen_t server_addr_len = sizeof(server_addr);
    char buf[BUFFER_SIZE];
    int num_packet = 1;

    // Q5.b & Q5.c) Send file and received ACK
    printf("Send file\n");
    while (1) {
        // Build buffer at send
        memset(buf, 0, BUFFER_SIZE);
        buf[0] = 0; buf[1] = DATA;
        buf[2] = (num_packet >> 8) & 0xFF;
        buf[3] = num_packet & 0xFF;

        // Read the file
        size_t bytes_read = fread(&buf[4], 1, 512, fp);
        if (bytes_read < 0) {
            perror("File read error");
            fclose(fp);
            exit(EXIT_FAILURE);
        }

        // Send the file
        if (sendto(sock, buf, bytes_read + 4, 0, (struct sockaddr*)&server_addr, server_addr_len) < 0) {
            perror("sendto error");
            fclose(fp);
            exit(EXIT_FAILURE);
        }
        printf("Sent file of %ld bytes in the packet n°%d\n", bytes_read, num_packet);

        // Wait the ACK
        memset(buf, 0, BUFFER_SIZE);
        ssize_t recv_size = recvfrom(sock, buf, BUFFER_SIZE, 0, (struct sockaddr*)&server_addr, &server_addr_len);
        if (recv_size < 0) {
            perror("recvfrom error");
            fclose(fp);
            exit(EXIT_FAILURE);
        }
        // Check if it is an ACK for our file
        if (buf[1] == ACK && buf[2] == ((num_packet >> 8) & 0xFF) && buf[3] == (num_packet & 0xFF)) {
            printf("Received ACK for paquet n°%d\n", num_packet);
            num_packet++;
        } else {
            printf("Unexpected packet received\n");
        }
        // End if we received less than BUFFER_SIZE
        if (bytes_read < BUFFER_SIZE) {
            printf("Transfer complete.\n");
            break;
        }
    }

    // Clean up
    fclose(fp);
    freeaddrinfo(res);
    close(sock);
    printf("success all Q5.\n");
    exit(EXIT_SUCCESS);
}
