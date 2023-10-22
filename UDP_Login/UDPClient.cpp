#include <bits/stdc++.h>
#include <winsock2.h>
using namespace std;
#pragma comment(lib, "ws2_32.lib")

#define SERVER "127.0.0.1"  // Server IP address
#define BUFLEN 1024
#define PORT 8888

int main(int argc, char *argv[]) {
    if(argc != 4  && argc != 2){
        cerr << "Please input following structure: .\\UDPClient LOGIN <id> <password> OR .\\UDPClient LOGOUT" << endl;
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
    }    // Initialize Winsock
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        cout << "Failed to initialize Winsock" << endl;
        return 1;
    }

    // Create socket
    SOCKET clientSocket;
    if ((clientSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR) {
        cout << "Failed to create socket" << endl;
        return 1;
    }

    // Setup server address structure
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER);

    cout << "UDP client is running" << endl;

    // Send ID to the server
    if (sendto(clientSocket, user, strlen(user), 0, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        cout << "sendto() failed with error code: " << WSAGetLastError() << endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }
    
    char buff1[BUFLEN];
    sockaddr_in serverResponse;
    int serverResponseLen = sizeof(serverResponse);
    int recvLen;
    recvLen = recvfrom(clientSocket, buff1, BUFLEN, 0, (sockaddr*)&serverResponse, &serverResponseLen);
    if (recvLen == SOCKET_ERROR) {
        cout << "recvfrom() failed with error code: " << WSAGetLastError() << endl;
        closesocket(clientSocket);
        WSACleanup();
    }
    buff1[recvLen] = '\0';
    cout << "Respone from server " << inet_ntoa(serverResponse.sin_addr) << ":" << ntohs(serverResponse.sin_port) << " : " << buff1 << endl;
    while(1){
        char buff2[BUFLEN];
        cout<<"\n[XIN VUI LONG NHAN 7 NEU BAN MUON NHAP DU LIEU DA LUU VAO CHUONG TRINH]"
             "\n||==============|CHUONG TRINH LOGIN|============================||"
             "\n||             --------CAC CHUC NANG--------                    ||"
             "\n||               0.Disconnect                                   ||"
             "\n||               1.LOGOUT                                       ||"
             "\n||     ** **     2.LOGIN <id> <password>          ** **         ||"
             "\n||     ** **     3.REGISTER <id> <password>          ** **      ||"
             "\n||==============================================================||\n";
        cout << "Input: " << endl;
        cin.getline(buff2,BUFLEN);

        if (sendto(clientSocket, buff2, strlen(buff2), 0, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
            cerr << "sendto() failed with error code: " << WSAGetLastError() << endl;
            break;
        }
        // Exit the loop to terminate the client
        if (strcmp(buff2, "disconnect") == 0) {
            cout << "Disconnected from the server." << endl;
            break; 
        }

        sockaddr_in serverResponse;
        int serverResponseLen = sizeof(serverResponse);

        int recvLen2 = recvfrom(clientSocket, buff2, BUFLEN, 0, (sockaddr*)&serverResponse, &serverResponseLen);
        if (recvLen2 == SOCKET_ERROR) {
            cout << "recvfrom() failed with error code: " << WSAGetLastError() << endl;
            closesocket(clientSocket);
            WSACleanup();
        }
        else{
            buff2[recvLen2] = '\0';
            cout << "Respone from server " << inet_ntoa(serverResponse.sin_addr) << ":" << ntohs(serverResponse.sin_port) << " : " << buff2 << endl;
        }
    }

    closesocket(clientSocket);
    WSACleanup();
    return 0;
}
