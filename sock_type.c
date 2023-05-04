#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>

int main(int argc, const char* argv[])
{
    int tcp_socket = 0;
    int udp_socket = 0;
    socklen_t option_len;
    int state = 0;
    int socket_type;
    tcp_socket = socket(PF_INET, SOCK_STREAM, 0);
    if(tcp_socket == -1) puts("tcp_socket 오류");
    udp_socket = socket(PF_INET, SOCK_DGRAM, 0);
    if(udp_socket == -1) puts("udp_socket 오류");
    printf("SOCK_STREAM : %d \r\n", SOCK_STREAM);
    printf("SOCK_DGRAM : %d \r\n", SOCK_DGRAM);
    
    state = getsockopt(tcp_socket, SOL_SOCKET, SO_TYPE, (void*)&socket_type, (socklen_t*)&option_len);
    if(state) printf("error\n");
    
    printf("socket_type : %d \t option_len : %d\r\n", socket_type, option_len);
    return 0;
}
