#include <string>
#include <Winsock2.h>

class ClientInfo
{
public:
	ClientInfo(const std::string &name, unsigned long address)
		: name_(name), addr_(address){}
	std::string getName()
	{
		return name_;
	}
	unsigned long getAddr()
	{
		return addr_;
	}

private:
	std::string   name_;
	unsigned long addr_;
};
