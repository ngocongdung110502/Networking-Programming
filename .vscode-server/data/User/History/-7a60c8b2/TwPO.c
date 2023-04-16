#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]){
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len;
    char buffer[BUFFER_SIZE];
    FILE *hello_file, *client_file;

    //Kiem tra so luong tham so dau vao
    if(argc != 4){
        printf("Sai cu phap!\n");
        fprintf(stderr, "Su dung: %s <port> <hello_file> <client_file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    //Mo tep tin chua cau chao
    hello_file = fopen(argv[2], "r");
    if (!hello_file){
        perror("Mo file hello_file that bai");
        exit(EXIT_FAILURE);
    }

    //Tao socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_fd < 0){
        perror("Tao socket that bai");
        exit(EXIT_FAILURE);
    }

    //Thiet lap dia chi va cong cua server_addr
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(atoi(argv[1]));

    //Gan dia chi va cong cho socket
    if(bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
        perror("Gan dia chi va cong cho socket that bai");
        exit(EXIT_FAILURE);
    }

    //Lang nghe ket noi 
    if(listen(server_fd, 1) < 0){
        perror("Cho ket noi that bai");
        exit(EXIT_FAILURE);
    }

    //Doi ket noi tu client
    printf("Dang cho ket noi den...\n");
    client_len = sizeof(client_addr);
    client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
    if(client_fd < 0){
        perror("Chap nhan ket noi that bai");
        exit(EXIT_FAILURE);
    }

    // Gui cau chao den client
    fgets(buffer, BUFFER_SIZE, hello_file);
    send(client_fd, buffer, strlen(buffer), 0);

    //Mo tep tin de luu noi dung
    client_file = fopen(argv[3], "w");
    if(!client_file){
        perror("Mo file client_file that bai");
        exit(EXIT_FAILURE);
    }

    //Nhan du lieu tu client va luu vao tep tin
    int num_bytes;
    while((num_bytes = recv(client_fd, buffer, BUFFER_SIZE, 0)) > 0){
        fwrite(buffer, 1, num_bytes, client_file);
    }

    fclose(hello_file);
    fclose(client_file);
    close(client_fd);
    close(server_fd);

    printf("Du lieu da duoc luu vao %s\n", argv[3]);

    return 0;
}