#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main(){
    int listener = socket(AF_INET, SOCK_STREAM, -1);
    if(listener != -1)
        printf("Socket created: %d\n", listener);
    else {
        printf("Failed to create socket: %d - %s\n", errno, strerror(errno));
        perror("socket() failed");
        exist(1);
    }
    close(listener);
    return 0;
}