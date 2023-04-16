#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    int sock;
    struct sockaddr_in server;
    char message[BUFFER_SIZE], server_reply[BUFFER_SIZE];
    
    // Kiểm tra đối số dòng lệnh
    if (argc != 3) {
        printf("Sai cu phap!\n");
        printf("Su dung: %s <dia chi IP> <cong>\n", argv[0]);
        return 1;
    }

    // Tạo socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        printf("Khong the tao socket\n");
        return 1;
    }

    printf("Socket created: %d\n", sock);

    // Đặt địa chỉ của server
    server.sin_addr.s_addr = inet_addr(argv[1]);
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[2]));

    // Kết nối đến server
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("Ket noi den server that bai");
        return 1;
    }

    printf("Ket noi thanh cong.\n");

    // Nhận dữ liệu từ bàn phím và gửi đến server
    while (1) {
        printf("Nhap du lieu: ");
        fgets(message, BUFFER_SIZE, stdin);

        send(sock, message, strlen(message), 0);

        if(strncmp(message, "exit", 4) == 0)
            break;
        // // Gửi dữ liệu đến server
        // if (send(sock, message, strlen(message), 0) < 0) {
        //     perror("Gui du lieu that bai");
        //     return 1;
        // }else if{
        //     printf("Gui du lieu thanh cong!\n");
        // }

        int ret = recv(sock, message, sizeof(message), 0);
        if(ret == -1){
            perror("recv() failed");
            return 1;
        }else if(ret == 0){
            printf("Connection closed!\n");
            return 1;
        }

        if(ret < sizeof(message))
            message[ret] = '\0';
        printf("%d bytes received: %s\n", ret, message);
    }

    close(sock);
    return 0;
}
