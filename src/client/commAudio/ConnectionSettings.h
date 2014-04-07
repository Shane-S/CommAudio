#ifndef CONNECTION_SETTINGS_H
#define CONNECTION_SETTINGS_H

class ConnectionSettings
{
public:
    ConnectionSettings() {}
    ConnectionSettings(std::string ipAddress, std::string username, int portUDP, int portTCP)
        : ipAddress(ipAddress), username(username), portUDP(portUDP), portTCP(portTCP) {}
    
    std::string getIpAddress() { return ipAddress; }
    void setIpAddress(std::string ipAddress_) { ipAddress = ipAddress_; }
    
    std::string getUsername() { return username; }
    void setUsername(std::string username_) { username = username_; }

    int getPortUDP() { return portUDP; }
    void setPortUDP(int port_) { portUDP = port_; }

    int getPortTCP() { return portTCP; }
    void setPortTCP(int port_) { portTCP = port_; }

private:
    std::string ipAddress;
    std::string username;
    int         portUDP;
    int         portTCP;
};

#endif