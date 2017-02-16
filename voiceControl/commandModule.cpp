/* Julius Folder
julius -quiet -input mic -C julius.jconf 2>/dev/null | ./../voiceControl/commandModule */


#include "commandModule.hpp"
#include "UDP_server.hpp"


std::string commandModule::getCommand()
{
	std::string input, command;
	std::getline(std::cin, input);
  //std::cout << input << std::endl;
	if( input.length() > 20)
  	{
    	if(input[15] == 'q' && input[16] == 'u' && input[17] == 'e' && input[18] == 'n' && input[19] == 't' && input[20] == 'e')
    	{
    		std::cout << "quente" << std::endl;
    		command = "q";
    	}
    	else if(input[15] == 'f' && input[16] == 'r' && input[17] == 'i' && input[18] == 'o')
    	{
    		std::cout << "frio" << std::endl;
    		command = "f";
  		}
  		else if(input[15] == 'c' && input[16] == 'h' && input[17] == 'e' && input[18] == 'g' && input[19] == 'o' && input[20] == 'u')
    	{
    		std::cout << "chegou" << std::endl;
    		command = "a";
  		}
  		else
  			command.clear();
  	}
  	else
  		command.clear();
	return command;
}

void commandModule::sendCommand(std::string comm)
{
	
	size_t size = comm.size();
	udp_server UDP("127.0.0.1", 8001);
	std::cout << UDP.send(comm.c_str(), size) << std::endl;

}

int main(int argc, char const *argv[])
{
	commandModule Command;
	std::string input;

	while(true)
	{
		input = Command.getCommand();
		if(!input.empty())
		{
			//std::cout << input << std::endl;
			Command.sendCommand(input);
		}
	}	

	return 0;
}