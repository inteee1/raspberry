#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> //linux
#include <arpa/inet.h> // linux
#include <sys/socket.h> // linux

void error_handling(const char* );

int main(int argc, const char* argv[])
{
    int sock = 0;
    struct sockaddr_in serv_addr;
    char message[BUFSIZ] = {'\0', };
    int str_length = 0;

    if(argc != 3) {
        fprintf(stdout, "Usage : %s IP PORT \r\n", argv[0]);
        exit(1);
    }
    // 1 socket()
    sock = socket(PF_INET/*IPv4*/, SOCK_STREAM/*TCP*/, 0);
    if(sock == -1) {
            error_handling("socket() error");
    }
    memset(&serv_addr, 0, sizeof serv_addr); // 0 initailizing
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]); // IP
    serv_addr.sin_port = htons(atoi(argv[2])); // PORT
    //2 connect()

    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof serv_addr) == -1) {
            error_handling("connect() error");
    }
    str_length = read(sock, message, sizeof message - 1);
    if (str_length == -1) {
            error_handling("No reading or No string error");
    }
    fprintf(stdout, "Message from server : %s\r\n", message);
    close(sock);

    return 0;
}

void error_handling(const char* _message)
{
        fputs(_message, stdout);
        fputs("\r\n",stdout);
        exit(1);
}
