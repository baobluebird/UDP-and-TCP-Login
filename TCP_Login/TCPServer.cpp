// #define NTDDI_VERSION NTDDI_VISTA
// #define WINVER _WIN32_WINNT_VISTA
// #define _WIN32_WINNT _WIN32_WINNT_VISTA
#include <winsock2.h>
#include <WS2tcpip.h>
#include <bits/stdc++.h>
#include "setting.h"

// need link with Ws2_32.lib VS
//#pragma comment(lib, "Ws2_32.lib")

using namespace std;

class User{
    private :
        string tk;
        string mk;
        bool status; 
        bool login;
    public:
    static bool isLoggedIn;
    static string saveID;
    //get
    string getTk() const {
        return tk;
    }
    string getMk() const {
        return mk;
    }
    bool getStatus(){
        return status;
    }
    bool getLogin(){
        return login;
    }
    //set
    void setTk(string tk){
        this->tk = tk;
    }
    void setMk(string mk){
        this->mk = mk;
    }
    void setStatus(bool status){
        this->status = status;
    }
    
    void setLog(bool login){
        this->login = login;
    }

    static void Login(string save){
        isLoggedIn = true;
        saveID = save;
    }

    static void Logout(string save){
        isLoggedIn = false;
        saveID = save;
    }

};

bool User::isLoggedIn = false;
string User::saveID ="";
vector<User> userData;
//convert string to bool
int string_to_int(string &str){
    if(str == "0"){
        return 0;
    }
    if(str == "1"){
        return 1;
    }
    return 0;
}
vector<User> write_to_class(){
    
    vector<User> userDataRead;
    ifstream file("database.csv");

    if (!file.is_open()) cerr << "Error: Unable to open file "<< endl;

    string line;

    while (getline(file, line)){

        string cell;
        stringstream lineStream(line);
        User user;

        string username, pass;
        bool bool1,bool2;
        
        // Read id, password, active, and login
        getline(lineStream, username, ',') ;
        if(username == "")break;
        user.setTk(username);

        getline(lineStream, pass, ',') ;
        user.setMk(pass);

        getline(lineStream, cell, ',') ;
        stringstream(cell) >> bool1 ;
        user.setStatus(bool1);

        getline(lineStream, cell, ','); 
        stringstream(cell) >> bool2;
        user.setLog(bool2);

        userDataRead.push_back(user);
    }
    file.close();
    return userDataRead;
}

void writefileLogin(int count_line){
    ifstream file_read("database.csv", ios::in);

    string line_out;
    vector<string> data;
    while(getline(file_read,line_out)){
        data.push_back(line_out);
    }

    size_t LastComma = data[count_line].find_last_of(",");
    if(LastComma != string::npos){
        data[count_line].replace(LastComma + 1, 1, "1");
    }
    
    ofstream file_write("database.csv", ios::out) ;

    for(string x : data){
       file_write << x << "\n";
    }

    file_write.close();
    
}

void writefileLogout(int count_line){
    ifstream file_read("database.csv", ios::in);

    string line_out;
    vector<string> data;
    while(getline(file_read,line_out)){
        data.push_back(line_out);
    }

    size_t LastComma = data[count_line].find_last_of(",");
    if(LastComma != string::npos){
        data[count_line].replace(LastComma + 1, 1, "0");
    }

    ofstream file_write("database.csv", ios::out) ;

    for(string x : data){
        file_write << x << "\n";
    }

    file_write.close();
    
}

string Checking(const string &in){
    
    userData = write_to_class();
    istringstream iss(in);
    string work, id, pass;
    iss >> work >> id >> pass;
    cout << "--------------------------\n";
    for(User &user : userData){
        cout << user.getTk() << " " << user.getMk() << " " << user.getStatus() << " " << user.getLogin() << endl;
    }
    cout << "--------------------------\n";
    if(work == "LOGIN"){
        for(int i = 0; i < userData.size(); i++){
            if(id == userData[i].getTk()){
                if(pass == userData[i].getMk()){
                    if(userData[i].getLogin() == 1 && User::isLoggedIn == true){
                        string tmp1 = "You must logout this user *";
                        string tmp2 = "* before login another";
                        string tmp = tmp1 + User::saveID + tmp2;
                        return tmp;
                    }
                    else if(userData[i].getLogin() == 0 && User::isLoggedIn == true){
                        string tmp1 = "You must logout this user *";
                        string tmp2 = "* before login another";
                        string tmp = tmp1 + User::saveID + tmp2;
                        return tmp;
                    }
                    else if(userData[i].getLogin() == 1){
                        User::Login(userData[i].getTk());
                        string tmp = "already logged in this user " + userData[i].getTk();
                        return tmp;
                    }
                    else{
                        cout << i << endl;
                        writefileLogin(i); 
                        User::Login(userData[i].getTk());
                        string tmp = "success loggin!...id: " + userData[i].getTk();
                        return tmp;
                    }
                }
                else if(User::isLoggedIn == true){
                    string tmp1 = "You must logout this user ";
                    string tmp2 = " before login another";
                    string tmp = tmp1 + User::saveID + tmp2;
                    return tmp;
                }
                else{
                    return "Invalid password";
                }
            }  
        }
        return "Invalid id";
    }
    else if(work == "LOGOUT"){
        for(int i = 0; i < userData.size();i++){
            if(User::saveID == userData[i].getTk()){
                writefileLogout(i);
                User::Logout(userData[i].getTk());
                string tmp = "success logout!...id: " + userData[i].getTk();
                return tmp;
            }
        }
    }
    return "Invalid format";
}

int main(){
    WORD wVersion = MAKEWORD(2, 2); 
                                   
    WSADATA wsaData;
    if (WSAStartup(wVersion, &wsaData) != 0) 
    {
        cout << "WSAStartup failed with error" << GetLastError() << endl;
        return 0;
    }
    cout << "WSAStartup completed!" << endl;
    
    //listen
    SOCKET listensock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(listensock != INVALID_SOCKET){
        cout << "Creating socket completed successfully." << endl;
    } else{
        cout << "Creating socket failed with code " << GetLastError() << endl;
    }

    sockaddr_in tcpSeverAddr;
    tcpSeverAddr.sin_family = AF_INET;
    tcpSeverAddr.sin_port = htons(SERVER_PORT);
    tcpSeverAddr.sin_addr.S_un.S_addr = inet_addr(SERVER_ADDR);

    //bind
    if(!bind(listensock, (sockaddr *) &tcpSeverAddr, sizeof(tcpSeverAddr))){
        cout << "Bind API completed successfully." << endl;

    } else{
        cout << "Bind API failed with code " << GetLastError() << endl;
        closesocket(listensock);
        WSACleanup();
        return 1;
    }


    if (listen(listensock, 5) != SOCKET_ERROR){ 
        cout << "Successfully !!! Server is listening the requests ..." << endl;
    } else{ 
        cout << "Listen failed with code " << GetLastError() << endl;
        closesocket(listensock);
        WSACleanup();
        return 1;
    }
    
    //Step 5: Data exchange
    sockaddr_in clientAddr;// địa chỉ của client và address
    char buff[BUFF_MAXSIZE],clientIP[INET_ADDRSTRLEN];
    int ret, clientAddrLen = sizeof(clientAddr),clientPort;

    SOCKET NewConnection = accept(listensock, (sockaddr*) &clientAddr, &clientAddrLen);
    if(NewConnection == SOCKET_ERROR){
        cout << "Connection failed with code" << WSAGetLastError() << endl;
        return 0;
    }
    else{
        inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, sizeof(clientIP));
        clientPort = ntohs(clientAddr.sin_port);
        cout << "Connection is established: IP " << clientIP << " at port " << clientPort << endl;
    }

    while(1){
        //recieve messages from client
        ret = recv(NewConnection, buff, BUFF_MAXSIZE, 0);
        if(ret == SOCKET_ERROR){
            cout << "Error with code" << WSAGetLastError() << endl;
            break;
        }
        else{
            buff[ret] = '\0'; 

            cout << "Recieve message from client! " << clientIP << ":" << clientPort << " message is: "<< buff << " !..." << endl;
            if(ret == SOCKET_ERROR){
                cout << "Error with code" << WSAGetLastError() << endl;
                break;
            }
            if(strcmp(buff, "disconnect") == 0){
                const char *rp_disconnect = "disconnect";
                ret = send(NewConnection, rp_disconnect, strlen(rp_disconnect), 0);
                cerr << "disconnect" << endl;
                break;
            }
            if((strcmp(buff, "LOGOUT") == 0) && (User::isLoggedIn == false)){
                const char* rp_logout_system = "not logging yet!...";
                ret = send(NewConnection, rp_logout_system, strlen(rp_logout_system), 0);
            }
            else{
                string buffStr(buff);
                buffStr = Checking(buffStr);
                cout << "Response from Server: " << buffStr << endl;
                const char* message = buffStr.c_str();
                ret = send(NewConnection, message, strlen(message), 0);
            }
        }
    }
    closesocket(NewConnection);
    closesocket(listensock);
    
    // Setup winsock communication code here
    // sockaddr_in a;
    
    
    // Call WSACleanup when application finishes
    if (WSACleanup() != 0)
        cout << "Clean failed with error." << GetLastError() << endl;

    return 0;
}
