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
        printf("Sử dụng: %s <địa chỉ IP> <cổng>\n", argv[0]);
        return 1;
    }

    // Tạo socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        printf("Không thể tạo socket\n");
        return 1;
    }

    // Đặt địa chỉ của server
    server.sin_addr.s_addr = inet_addr(argv[1]);
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[2]));

    // Kết nối đến server
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("Kết nối đến server thất bại");
        return 1;
    }

    printf("Kết nối thành công.\n");

    // Nhận dữ liệu từ bàn phím và gửi đến server
    while (1) {
        printf("Nhập tin nhắn: ");
        fgets(message, BUFFER_SIZE, stdin);

        // Gửi dữ liệu đến server
        if (send(sock, message, strlen(message), 0) < 0) {
            perror("Gửi tin nhắn thất bại");
            return 1;
        }

        // Nhận dữ liệu từ server
        if (recv(sock, server_reply, BUFFER_SIZE, 0) < 0) {
            perror("Nhận tin nhắn từ server thất bại");
            break;
        }

        printf("Server trả lời: %s", server_reply);
    }

    close(sock);
    return 0;
}
