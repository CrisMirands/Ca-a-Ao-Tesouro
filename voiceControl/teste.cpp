#include <iostream>
#include "UDP_client.hpp"

int main(int argc, char const *argv[])
{
	udp_client UDP("127.0.0.1", 8001);
	char comm;
	//size_t size = comm.size();
	int size = sizeof(char);
	while(true)
	{
		UDP.recv(&comm, size);
		std::cout <<  comm << std::endl;
	}

	return 0;
}