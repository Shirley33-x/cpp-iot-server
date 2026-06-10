#include <iostream>
#include <thread>
#include <vector>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <memory>
#include <string>

using namespace std;

void handleClient(int clientSocket) {
    char buffer[1024] = {0};
    while (true) {
        ssize_t bytesRead = read(clientSocket, buffer, 1024);
        if (bytesRead <= 0) break;

        cout << "[接收数据] " << buffer << endl;
        string res = "服务器已接收数据\n";
        send(clientSocket, res.c_str(), res.size(), 0);
        memset(buffer, 0, 1024);
    }
    close(clientSocket);
}

int main() {
    int serverFd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8888);
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(serverFd, (sockaddr*)&addr, sizeof(addr));
    listen(serverFd, 50);
    cout << "物联网数据采集服务已启动，端口：8888" << endl;

    vector<thread> threads;
    while (true) {
        int client = accept(serverFd, nullptr, nullptr);
        threads.emplace_back(handleClient, client);
    }

    for (auto& t : threads) t.join();
    close(serverFd);
    return 0;
}