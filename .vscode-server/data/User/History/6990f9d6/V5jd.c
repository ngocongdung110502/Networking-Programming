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

        // Gửi dữ liệu đến server
        if (send(sock, message, strlen(message), 0) < 0) {
            perror("Gui du lieu that bai");
            return 1;
        }

        // // Nhận dữ liệu từ server
        // if (recv(sock, server_reply, BUFFER_SIZE, 0) < 0) {
        //     perror("Nhan tin nhan tu server that bai");
        //     break;
        // }

        // printf("Server tra loi: %s\n", server_reply);

        printf("Gui du lieu thanh cong!");
        return 0;
    }

    close(sock);
    return 0;
}
