#include <bits/stdc++.h>
#include <winsock2.h>
using namespace std;

#define BUFLEN 2024
#define PORT 8888

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

    // Initialize Winsock
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        cout << "Failed to initialize Winsock" << endl;
        return 1;
    }

    // Create socket
    SOCKET serverSocket;
    if ((serverSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR) {
        cout << "Failed to create socket" << endl;
        return 1;
    }

    // Setup server address structure
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        cout << "Bind failed with error code: " << WSAGetLastError() << endl;
        return 1;
    }else{
        cout << "\nUDP server is running and listening on port " << PORT << endl;         
    }
    char buf[BUFLEN];
    sockaddr_in clientAddr;
    int clientAddrLen = sizeof(clientAddr);
    int recvLen;
    
        
    //request
    while (1) {
        
        recvLen = recvfrom(serverSocket, buf, BUFLEN, 0, (sockaddr*)&clientAddr, &clientAddrLen);
        if (recvLen == SOCKET_ERROR) {
            cout << "recvfrom() failed with error code: " << WSAGetLastError() << endl;
            break;
        }else{
            buf[recvLen] = '\0'; 
            //disconnect
            if(strcmp(buf, "disconnect") == 0){
                const char *rp1 = "disconnect";
                sendto(serverSocket, rp1, strlen(rp1), 0, (sockaddr *)&clientAddr, clientAddrLen);
                cerr << "disconnect" << endl;
                break;
            }
            else{
                cout << "Message from Client "<< inet_ntoa(clientAddr.sin_addr) << ":" <<ntohs(clientAddr.sin_port) << " : "<< buf << endl;
                string buffStr(buf);
                buffStr = Checking(buffStr);
                cout << "Response from Server: " << buffStr << endl;
                const char* message = buffStr.c_str();
                sendto(serverSocket, message, strlen(message), 0, (sockaddr *)&clientAddr, clientAddrLen);
            } 
        }

            
    }

    closesocket(serverSocket);
    WSACleanup();
    return 0;
}
