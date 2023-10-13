#include <winsock2.h>
#include <ws2tcpip.h>
#include <bits/stdc++.h>
#include "setting.h"
using namespace std;

int main(int argc, char *argv[]) {
    if(argc != 4  && argc != 2){
        cerr << "Please input following structure: .\\TCPClient LOGIN <id> <password> OR .\\TCPClient LOGOUT" << endl;
        return 1;
    }
    for (int i = 0; i < argc; i++){
        cout << argv[i] << " ";
    }
    cout << endl;
    char* user;
    if(argc == 4){
        const char *work = argv[1];
        const char *ID = argv[2];
        const char *PASSWORD = argv[3];
        int userLength = snprintf(NULL, 0, "%s %s %s", work, ID, PASSWORD);
        user = new char[userLength + 1];
        snprintf(user, userLength + 1, "%s %s %s", work, ID, PASSWORD);
    }
    else{
        user = argv[1];
    }
    WORD wVersionRequested = MAKEWORD(2, 2);
    WSADATA wSadata;
    if (WSAStartup(wVersionRequested, &wSadata) != 0)
    {
        cout << "WSAStartup failed: \n";
        return 0;
    }

    // STEP 1: Create a listening socket
    sockaddr_in tcpserverAddr;
    tcpserverAddr.sin_family = AF_INET;//IPV4
    tcpserverAddr.sin_port = htons(SERVER_PORT);
    tcpserverAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY); // Listen on any available interface

    SOCKET listensocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // khởi tạo socket
    if (listensocket != INVALID_SOCKET)
    {
        cout << "socket function succeeded" << endl;
    }

    // STEP 2: Bind the listening socket
    if (bind(listensocket, (sockaddr *)&tcpserverAddr, sizeof(tcpserverAddr)) == SOCKET_ERROR){
        cout << "Bind failed with error: " << WSAGetLastError() << endl;
        return 0;
    }

    // STEP 3: Set up and connect to the server
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.S_un.S_addr = inet_addr(SERVER_ADDR);

    SOCKET client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (connect(client, (sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
    {
        cout << "Connection failed with code " << WSAGetLastError() << endl;
        return 0;
    }
    else
        cout << "Connection to server completed successfully." << endl;

    // STEP 3: Send data (example)
    int bytesSent = send(client, user, strlen(user), 0);
    if (bytesSent == SOCKET_ERROR){
        cout << "Send failed with error: " << WSAGetLastError() << endl;
    }

    // STEP 4: Receive data (example)
    char buff[BUFF_MAXSIZE];
    int bytesReceived = recv(client, buff, sizeof(buff), 0);
    if (bytesReceived == SOCKET_ERROR){
        cout << "Receive failed with error: " << WSAGetLastError() << endl;
    }
    else{
        buff[bytesReceived] = '\0';
        cout << "Received: " << buff << endl;
    }
     while(1){
        char buff2[BUFF_MAXSIZE];
        cout<<"\n[XIN VUI LONG NHAN 7 NEU BAN MUON NHAP DU LIEU DA LUU VAO CHUONG TRINH]"
             "\n||==============|CHUONG TRINH LOGIN|============================||"
             "\n||             --------CAC CHUC NANG--------                    ||"
             "\n||               0.Disconnect                                   ||"
             "\n||               1..\\TCPClient LOGOUT                           ||"
             "\n||               2..\\TCPClient LOGIN <id> <password>            ||"
             "\n||==============================================================||\n";
        cout << "Input: " << endl;
        cin.getline(buff2,BUFF_MAXSIZE);
        int bytesSent = send(client, buff2, strlen(buff2), 0);
        if (bytesSent == SOCKET_ERROR){
            cout << "Send failed with error: " << WSAGetLastError() << endl;
        }
        // Exit the loop to terminate the client
        if (strcmp(buff2, "disconnect") == 0) {
            cout << "Disconnected from the server." << endl;
            break; 
        }
        int bytesReceived = recv(client, buff2, sizeof(buff2), 0);
        if (bytesSent == SOCKET_ERROR){
            cout << "Send failed with error: " << WSAGetLastError() << endl;
            break;
        }       
        else{
            buff2[bytesReceived] = '\0';
            cout << "Respone from server: " << buff2 << endl;
        }
    }

    closesocket(listensocket);
    closesocket(client);
    WSACleanup();

    return 0;
}
