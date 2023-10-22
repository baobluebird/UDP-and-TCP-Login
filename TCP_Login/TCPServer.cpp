#include <winsock2.h>
#include <WS2tcpip.h>
#include <bits/stdc++.h>
using namespace std;
#define SERVER_ADDR "127.0.0.1"
#define SERVER_PORT 5500
#define BUFF_MAXSIZE 1024
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

void writefileLogin(int count_line, string num){

    ifstream file_read("database.csv", ios::in);
    string line_out;
    vector<string> data;
    while(getline(file_read,line_out)){
        data.push_back(line_out);
    }

    size_t LastComma = data[count_line].find_last_of(",");
    if(LastComma != string::npos){
        data[count_line].replace(LastComma + 1, 1, num);
    }
    
    ofstream file_write("database.csv", ios::out) ;

    for(string x : data){
       file_write << x << "\n";
    }

    file_write.close();
    
}

string Checking(string in){
    userData = write_to_class();
    istringstream iss(in);
    string work, id, pass;
    iss >> work >> id >> pass;
    
    if(work == "LOGIN"){
        for(int i = 0; i < userData.size(); i++){
            if(userData[i].getLogin() == 1){
                User::Login(userData[i].getTk());
                string tmp1 = "You must logout this user *";
                string tmp2 = "* before login another";
                string tmp = tmp1 + User::saveID + tmp2;
                return tmp;
            }
        }
        for(int i = 0; i < userData.size(); i++){
            if(id == userData[i].getTk()){
                if(pass == userData[i].getMk()){
                    if(userData[i].getStatus() == 0){
                        return "haven't registered an account yet";
                    }else{
                        writefileLogin(i,"1"); 
                        User::Login(userData[i].getTk());
                        string tmp = "success loggin!...id: " + userData[i].getTk();
                        return tmp;
                    }
                        
                }
                else{
                    return "Wrong password!";
                }
            }  
        }
        for(int i = 0; i < userData.size(); i++){
            if(pass == userData[i].getMk()){
                return "Wrong id!";
            }
        }
        return "this user haven't registered an account yet";
    }
    else if(work == "LOGOUT"){
        for(int i = 0; i < userData.size();i++){
            if(userData[i].getLogin() == 1){
                writefileLogin(i,"0");
                User::Logout(userData[i].getTk());
                string tmp = "success logout!...id: " + userData[i].getTk();
                return tmp;
            }
        }
        return "haven't logged in yet";
    }
    else if(work == "REGISTER"){
        for(int i = 0; i < userData.size(); i++){
            if(id == userData[i].getTk()){
                return "This id has been registered";
            }
        }
        User user;
        user.setTk(id);
        user.setMk(pass);
        user.setStatus(1);
        user.setLog(0);
        userData.push_back(user);
        ofstream file("database.csv", ios::app);
        file << id << "," << pass << "," << "1" << "," << "0" << "\n";
        file.close();
        return "success register!...id: " + id;
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
    //tcpSeverAddr.sin_addr.S_un.S_addr = inet_addr(SERVER_ADDR);
    //or inet_pton
    if (inet_pton(AF_INET, SERVER_ADDR, &tcpSeverAddr.sin_addr.S_un.S_addr) == 1) {
        std::cout << "Server address set using inet_pton." << std::endl;
    } else {
        std::cout << "inet_pton failed with error." << std::endl;
        closesocket(listensock);
        WSACleanup();
        return 1;
    }
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
            else{
                string buffStr(buff);//char to string
                buffStr = Checking(buffStr);
                cout << "Response from Server: " << buffStr << endl;
                const char* message = buffStr.c_str(); //string to char
                ret = send(NewConnection, message, strlen(message), 0);
            }
        }
    }
    closesocket(NewConnection);
    closesocket(listensock);
    
    // Call WSACleanup when application finishes
    if (WSACleanup() != 0)cout << "Clean failed with error." << GetLastError() << endl;

    return 0;
}
