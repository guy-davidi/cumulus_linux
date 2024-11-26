#include <stdio.h>
#include <stdlib.h>
#include <pcap.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define UDP_PORT 5000
#define SERVER_IP "127.0.0.1"

// Define the callback function for packet processing
void packet_handler(const struct pcap_pkthdr *pkthdr, const u_char *packet) {
    struct ip *ip_header = (struct ip*) (packet + 14); // Skip Ethernet header
    char src_ip[INET_ADDRSTRLEN];
    char dst_ip[INET_ADDRSTRLEN];

    // Convert IP addresses to readable format
    inet_ntop(AF_INET, &ip_header->ip_src, src_ip, INET_ADDRSTRLEN);
    inet_ntop(AF_INET, &ip_header->ip_dst, dst_ip, INET_ADDRSTRLEN);

    // Prepare the packet data to send via UDP
    char udp_message[1024];
    snprintf(udp_message, sizeof(udp_message), "SRC: %s, DST: %s", src_ip, dst_ip);

    // Create a UDP socket
    int sockfd;
    struct sockaddr_in servaddr;

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(UDP_PORT);
    servaddr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // Send data to the Flask server
    sendto(sockfd, udp_message, strlen(udp_message), MSG_CONFIRM, (const struct sockaddr*)&servaddr, sizeof(servaddr));
    close(sockfd);
}

// Function to start packet capture
void capture_packets(const char *dev) {
    pcap_t *handle;
    char errbuf[PCAP_ERRBUF_SIZE];

    // Open the capture interface
    handle = pcap_open_live(dev, BUFSIZ, 1, 1000, errbuf);
    if (handle == NULL) {
        fprintf(stderr, "Could not open device %s: %s\n", dev, errbuf);
        exit(EXIT_FAILURE);
    }

    // Start packet capture loop
    if (pcap_loop(handle, 0, packet_handler, NULL) < 0) {
        fprintf(stderr, "Error during packet capture: %s\n", pcap_geterr(handle));
        exit(EXIT_FAILURE);
    }

    // Close the capture handle
    pcap_close(handle);
}

int main() {
    // Start capturing packets from a specific network interface
    char *dev = "eth0"; // Adjust this to your network interface
    capture_packets(dev);

    return 0;
}
