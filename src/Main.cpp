// TODO: Arg parse
// XMPP Ports: 5222
// XMPP Ports: 5269

#include <iostream>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>

class XMPPClient {
	private:
		// Socket
		int sockfd;
		int running;
		int connected;
	
		// Host address
		char* host;
		int	port;

		// Credentials
		char* user;
		char* pass;

	public:
		XMPPClient(char* host, int port) 
		{
			this->host = host;
			this->port = port;
		}

		void connectToHost()
		{
			// Socket
			this->sockfd = socket(AF_INET, SOCK_STREAM, 0);

			if (this->sockfd == -1)
			{
				perror("[Error] Could not create a socket");
				return;
			}

			// Resolve host
			struct hostent *host_info = gethostbyname(this->host);
			struct in_addr **addr_list;
			
			if (host_info == nullptr)
			{
				perror("[Error] Could not resolve host");
				return;
			}

			// Hosts
			addr_list = (struct in_addr**)host_info->h_addr_list;

			// Structures
			struct sockaddr_in sock_address;
			sock_address.sin_family = AF_INET;
			sock_address.sin_port = htons(this->port); 

			// Foreach host
			for (int i = 0; addr_list[i] != NULL; i++)
			{
				// Connect
				const char* address = inet_ntoa(*addr_list[i]);
				sock_address.sin_addr.s_addr = inet_addr(address);
				
				printf("[Info] Trying address: %s\n", address);

				if (connect(this->sockfd, (const struct sockaddr*)&sock_address, sizeof(sockaddr)) == -1)
				{
					printf("[Error] Could not connect to host: %s", address);
					continue;
				}

				printf("[Info] Connected to %s\n", this->host);
				this->connected = 1;

				startLoop();
			}
		}

		void startLoop() 
		{
			if (!this->connected)
				return;

			this->running = 1;	

			// Receive
			char* buffer = new char[1024];

			while (this->running)
			{
				while (recv(this->sockfd, buffer, 1024, 0) != -1)
				{
					printf("[Info] Buffer: %s\n", buffer);
				}
			}

			delete[] buffer;	
		}
};

int main(int argc, const char* argv[])
{
	// Config
	char* host = "example.com";
	int	  port = 5222; 

	XMPPClient* xmppclient = new XMPPClient(host, port);
	xmppclient->connectToHost();
}

