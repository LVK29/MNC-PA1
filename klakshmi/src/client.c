/**
* @client
* @author  Swetank Kumar Saha <swetankk@buffalo.edu>, Shivang Aggarwal <shivanga@buffalo.edu>
* @version 1.0
*
* @section LICENSE
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License as
* published by the Free Software Foundation; either version 2 of
* the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
* General Public License for more details at
* http://www.gnu.org/copyleft/gpl.html
*
* @section DESCRIPTION
*
* This file contains the client.
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <strings.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netdb.h>

#define TRUE 1
#define MSG_SIZE 256
#define BUFFER_SIZE 256
#define CMD_SIZE 100
int logged_in = 0;
int server;
int connect_to_host_old(char *server_ip, char *server_port);
int connect_to_host(char *server_ip, int server_port);
void clientListener(char **argv);

void getIP()
{
	char hostbuffer[256];
	char *IPbuffer;
	struct hostent *host_entry;
	int hostname;

	// To retrieve hostname
	hostname = gethostname(hostbuffer, sizeof(hostbuffer));

	// To retrieve host information
	host_entry = gethostbyname(hostbuffer);
	// To convert an Internet network
	// address into ASCII string
	IPbuffer = inet_ntoa(*((struct in_addr *)
							   host_entry->h_addr_list[0]));

	printf("Hostname: %s\n", hostbuffer);
	printf("Host IP: %s\n", IPbuffer);
}
void clientListener(char **argv)
{
	while (!logged_in)
	{
		char *firstWord, *context;
		char *cmd = (char *)malloc(sizeof(char) * CMD_SIZE);

		memset(cmd, '\0', CMD_SIZE);
		if (fgets(cmd, CMD_SIZE - 1, stdin) == NULL)
			exit(-1);
		char *key = strtok(cmd, " ");
		//printf("KEY is %s\n", key);
		//printf("strcmp is %d	",strcmp(key, "AUTHOR"));
		if (strstr(key, "LOGIN"))
		{
			//printf("Logged in first and second attribute are %s %s \n",argv[1], argv[2]);
			server = connect_to_host_old(argv[1], argv[2]);
			// check if login fails then dont swtich loggedin to 1
			logged_in = 1;
			//	printf("LOGGED IN : %d\n", logged_in);
		}
		else if (strstr(key, "AUTHOR"))
		{
			printf("[%s:SUCCESS]\n", cmd);
			char your_ubit_name[] = "dunjiong";
			printf("I, %s, have read and understood the course academic integrity policy.\n", your_ubit_name);
			printf("[%s:SUCCESS]\n", cmd);
		}

		else if (strstr(key, "PORT"))
		{
			int port = *argv[1];
			printf("[%s:SUCCESS]\nPORT:%d\n[%s:END]\n",key,port,cmd);
			printf("[%s:SUCCESS]\n",cmd);
			printf("PORT:%d\n", port);
			printf("[%s:END]\n", cmd);
		}

		else if (strstr(key, "IP"))
		{
			printf("[%s:SUCCESS]\n", cmd);
			getIP();
			printf("[%s:END]\n", cmd);
		}
		else if (strstr(key, "EXIT"))
		{
			printf("[%s:SUCCESS]\n", cmd);
			exit(1);
			printf("[%s:END]\n", cmd);
		}
	}
	// if(lOGGEDin){
	// 	int server;
	// erver = connect_to_host(argv[1], atoi(argv[2]));
	// logged_in=1
	// }
}
/**
* main function
*
* @param  argc Number of arguments
* @param  argv The argument list
* @return 0 EXIT_SUCCESS
*/
int main(int argc, char **argv)
{
	if (argc != 3)
	{
		printf("Usage:%s [ip] [port]\n", argv[0]);
		exit(-1);
	}

	//INFINATE LOOP UNTILL CLIENT LOGS IN while (!logged_in)
	clientListener(argv);

	//server = connect_to_host(argv[1], atoi(argv[2]));

	while (TRUE)
	{
		printf("\n[PA1-Client@CSE489/589]$ ");
		fflush(stdout);

		char *msg = (char *)malloc(sizeof(char) * MSG_SIZE);
		memset(msg, '\0', MSG_SIZE);
		if (fgets(msg, MSG_SIZE - 1, stdin) == NULL) //Mind the newline character that will be written to msg
			exit(-1);
		strtok(msg, "\n");
		printf("I got: %s(size:%d chars)", msg, strlen(msg));

		printf("\nSENDing it to the remote server ... ");
		if (send(server, msg, strlen(msg), 0) == strlen(msg))
			printf("Done!\n");
		fflush(stdout);

		/* Initialize buffer to receieve response */
		char *buffer = (char *)malloc(sizeof(char) * BUFFER_SIZE);
		memset(buffer, '\0', BUFFER_SIZE);

		if (recv(server, buffer, BUFFER_SIZE, 0) >= 0)
		{
			printf("Server responded: %s", buffer);
			fflush(stdout);
		}
		if (strcmp(msg, "LOGOUT") == 0)
		{
			printf("logged out?");
			server = connect_to_host_old(argv[1], argv[2]);
			logged_in = 0;
			printf(" LOGGED OUT : .\n", logged_in);
			clientListener(argv);
		}
	}
}

int connect_to_host_old(char *server_ip, char *server_port)
{
	int fdsocket;
	struct addrinfo hints, *res;

	/* Set up hints structure */
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	/* Fill up address structures */
	if (getaddrinfo(server_ip, server_port, &hints, &res) != 0)
		perror("getaddrinfo failed");

	/* Socket */
	fdsocket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (fdsocket < 0)
		perror("Failed to create socket");

	/* Connect */
	if (connect(fdsocket, res->ai_addr, res->ai_addrlen) < 0)
		perror("Connect failed");

	freeaddrinfo(res);

	return fdsocket;
}