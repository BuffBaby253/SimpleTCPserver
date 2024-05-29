//Author: BuffBaby253
//Description: TCP Server that listens on port 6667 and sends report back to client when connection is established

#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

int main() {
    int sockfd, portno, n;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int opt = 1;
    socklen_t cli_len = sizeof(cli_addr);

    // Initialize the socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
        perror("socket error");
        exit(0);
    }

    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(int));

    bzero((char *)&serv_addr, sizeof(serv_addr));
    portno = atoi("6667");
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
        perror("bind error");
        exit(0);
    }

    if (listen(sockfd, 5) == -1) {
        perror("listen error");
        exit(0);
    }

    while ((n = accept(sockfd, (struct sockaddr*)&cli_addr, &cli_len)) != -1) {
        bzero((char*) buffer , sizeof buffer );
        n = read(n, buffer, sizeof(buffer) - 1);
        if (n > 0) {
            buffer[n] = '\0';
            cout << "Received: " << buffer << endl;
            send(n, "\x89PNG\xAABB\xFFCDEF", 12, 0);
        }
        close(n);
    }

    close(sockfd);
    return 0;
}
