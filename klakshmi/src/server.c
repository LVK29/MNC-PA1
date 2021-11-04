/**
* @server
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
* This file contains the server init and main while loop for tha application.
* Uses the select() API to multiplex between network I/O and STDIN.
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <netdb.h>

#include <arpa/inet.h>
#define BACKLOG 5
#define STDIN 0
#define TRUE 1
#define CMD_SIZE 100
#define BUFFER_SIZE 256
int CONNECTED_CLIENTS = 0;

// Start of List Structure
struct node {
   int order; //data
   char *hostname;
   char * ip;
   int port; 

   struct node *next;
};

struct node *head = NULL;
struct node *current = NULL;

int length() {
   int length = 0;
   struct node *current;
	
   for(current = head; current != NULL; current = current->next) {
      length++;
   }
	
   return length;
}

//display the list
void printList() {
   struct node *ptr = head;
   printf("\n[ ");

   int order_index = 0;
	
   //start from the beginning
   while(ptr != NULL) {
        ptr->order = order_index;
        order_index++;
      printf("(%d,%s,%s,%d) ",ptr->order,ptr->hostname,ptr->ip,ptr->port);
      ptr = ptr->next;
   }
	
   printf(" ]");
}
void insertFirst(int order, char *hostname, char *ip, int port) {
   //create a link
   struct node *link = (struct node*) malloc(sizeof(struct node));
	
   link->order = order;
   link->hostname = hostname;
	link->ip = ip;
    link->port = port;
   //point it to old first node
   link->next = head;
	
   //point first to new first node
   head = link;
}


void sort() {

   int i, j, k, tempKey, tempData;
   char *tempHOST;
   char *tempIP;
   struct node *current;
   struct node *next;
	
   int size = length();
   k = size ;
	
   for ( i = 0 ; i < size - 1 ; i++, k-- ) {
      current = head;
      next = head->next;
		
      for ( j = 1 ; j < k ; j++ ) {   

         if ( current->port > next->port ) {
            tempData = current->port;
            current->port = next->port;
            next->port = tempData;

            tempKey = current->order;
            current->order = next->order;
            next->order = tempKey;

            tempHOST = current->hostname;
            current->hostname = next->hostname;
            next->hostname = tempHOST;

            tempIP = current->ip;
            current->ip = next->ip;
            next->ip = tempIP;

         }
			
         current = current->next;
         next = next->next;
      }
   }   
}
/// End of List Structure


///Start of STAT Structure
struct stat_node {
   int order; //data
   char *hostname;
   char *ip;
   int port; 
   int message_sent;
   int message_received;
   char *status;


   struct stat_node *next;
};

struct stat_node *stat_head = NULL;
struct stat_node *stat_current = NULL;

struct stat_node* stat_delete(char *key) {

   //start from the first link
   struct stat_node* current = stat_head;
   struct stat_node* previous = NULL;
	
   //if list is empty
   if(stat_head == NULL) {
      return NULL;
   }

   //navigate through list
   while(current->ip != key) {

      //if it is last node
      if(current->next == NULL) {
         return NULL;
      } else {
         //store reference to current link
         previous = current;
         //move to next link
         current = current->next;
      }
   }

   //found a match, update the link
   if(current == stat_head) {
      //change first to point to next link
      stat_head = stat_head->next;
   } else {
      //bypass the current link
      previous->next = current->next;
   }    
	
   return current;
}

//display the list
void stat_printList() {
   struct stat_node *ptr = stat_head;
   printf("\n[ ");

   int order_index = 0;
	
   //start from the beginning
   while(ptr != NULL) {
        ptr->order = order_index;
        order_index++;
      printf("(%d,%s,%d,%d,%s)\n",ptr->order,ptr->hostname,ptr->message_sent,ptr->message_received,ptr->status);
      ptr = ptr->next;
   }
	
   printf(" ]");
}

int stat_length() {
   int length = 0;
   struct stat_node *current;
	
   for(current = stat_head; current != NULL; current = current->next) {
      length++;
   }
	
   return length;
}
//insert link at the first location
void stat_insertFirst(int order, char *hostname, char *ip, int port, int message_sent, int message_received, char *status) {
   //create a link
   struct stat_node *link = (struct stat_node*) malloc(sizeof(struct stat_node));
	
   link->order = order;
   link->hostname = hostname;
	link->ip = ip;
    link->port = port;
    link->message_sent = message_sent;
    link->message_received = message_received;
    link->status = status;
   //point it to old first node
    link->next = stat_head;

	
   //point first to new first node
   stat_head = link;
}




struct stat_node *stat_find(char *key) {        //Find if ip existed

   //start from the first link
   struct stat_node* current = stat_head;

   //if list is empty
   if(stat_head == NULL) {
      return NULL;
   }

   //navigate through list
   while(strcmp(current->ip, key)) {
	
      //if it is last node
      if(current->next == NULL) {
         return NULL;
      } else {
         //go to next link
         current = current->next;
      }
   }      
	
   //if data found, return the current Link
   return current;
}

struct stat_node *stat_add_send_message(char *key) {        //Find if ip existed

   //start from the first link
   struct stat_node* current = stat_head;

   //if list is empty
   if(stat_head == NULL) {
      return NULL;
   }

   //navigate through list
   while(!strcmp(current->ip, key)) {
	
      //if it is last node
      if(current->next == NULL) {
         return NULL;
      } else {
         //go to next link
         current = current->next;

      }
   }      

    current->message_sent = current->message_sent + 1;
    stat_delete(current->ip);
    stat_insertFirst(current->order, current->hostname, current->ip, current->port, current->message_sent, current->message_received, current->status);
   //if data found, return the current Link
   return current;
}

struct stat_node *stat_add_received_message(char *key) {        //Find if ip existed

   //start from the first link
   struct stat_node* current = stat_head;

   //if list is empty
   if(stat_head == NULL) {
      return NULL;
   }

   //navigate through list
   while(!strcmp(current->ip, key)) {
	
      //if it is last node
      if(current->next == NULL) {
         return NULL;
      } else {
         //go to next link
         current = current->next;

      }
   }      
	
    current->message_sent = current->message_sent + 1;
    stat_delete(current->ip);
    stat_insertFirst(current->order, current->hostname, current->ip, current->port, current->message_sent, current->message_received, current->status);
   //if data found, return the current Link
   return current;
}

struct stat_node *stat_on_status(char *key) {        //Find if ip existed

   //start from the first link
   struct stat_node* current = stat_head;

   //if list is empty
   if(stat_head == NULL) {
      return NULL;
   }

   //navigate through list
   while(!strcmp(current->ip, key)) {
	
      //if it is last node
      if(current->next == NULL) {
         return NULL;
      } else {
         //go to next link
         current = current->next;
        //  stat_current = stat_current->next;

      }
   }      
	
    current->status = "logged-in";
        stat_delete(current->ip);
    stat_insertFirst(current->order, current->hostname, current->ip, current->port, current->message_sent, current->message_received, current->status);
    // stat_current->status = "logged-in"
    ;
   //if data found, return the current Link
   return current;
}

struct stat_node *stat_off_status(char *key) {        //Find if ip existed

   //start from the first link
   struct stat_node* current = stat_head;

   //if list is empty
   if(stat_head == NULL) {
      return NULL;
   }

   //navigate through list
   while(!strcmp(current->ip, key)) {
	
      //if it is last node
      if(current->next == NULL) {
         return NULL;
      } else {
         //go to next link
         current = current->next;

      }
   }      
	
    current->status = "logged-out";
    stat_delete(current->ip);
    stat_insertFirst(current->order, current->hostname, current->ip, current->port, current->message_sent, current->message_received, current->status);
   //if data found, return the current Link
   return current;
}




void stat_sort() {

   int i, j, k, tempKey, tempData, tempmessage_sent, tempmessage_received;
   char *tempHOST;
   char *tempIP;
   char *tempstatus;
   struct stat_node *current;
   struct stat_node *next;
	
   int size = stat_length();
   k = size ;
	
   for ( i = 0 ; i < size - 1 ; i++, k-- ) {
      current = stat_head;
      next = stat_head->next;
		
      for ( j = 1 ; j < k ; j++ ) {   

         if ( current->port > next->port ) {
            tempData = current->port;
            current->port = next->port;
            next->port = tempData;

            tempKey = current->order;
            current->order = next->order;
            next->order = tempKey;

            tempHOST = current->hostname;
            current->hostname = next->hostname;
            next->hostname = tempHOST;

            tempIP = current->ip;
            current->ip = next->ip;
            next->ip = tempIP;

            tempstatus = current->status;
            current->status = next->status;
            next->status = tempstatus;

            tempmessage_received = current->message_received;
            current->message_received = next->message_received;
            next->message_received = tempmessage_received;

            tempmessage_sent = current->message_sent;
            current->message_sent = next->message_sent;
            next->message_sent = tempmessage_sent;


         }
			
         current = current->next;
         next = next->next;
      }
   }   
}


///END of the STAT Structure

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
/**
* main function
*
* @param  argc Number of arguments
* @param  argv The argument list
* @return 0 EXIT_SUCCESS
*/
int main(int argc, char **argv)
{
	if (argc != 2)
	{
		printf("Usage:%s [port]\n", argv[0]);
		exit(-1);
	}

	int server_socket, head_socket, selret, sock_index, fdaccept = 0, caddr_len;
	struct sockaddr_in client_addr;
	struct addrinfo hints, *res;
	fd_set master_list, watch_list;

	/* Set up hints structure */
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	/* Fill up address structures */
	if (getaddrinfo(NULL, argv[1], &hints, &res) != 0)
		perror("getaddrinfo failed");

	/* Socket */
	server_socket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (server_socket < 0)
		perror("Cannot create socket");

	/* Bind */
	if (bind(server_socket, res->ai_addr, res->ai_addrlen) < 0)
		perror("Bind failed");

	freeaddrinfo(res);

	/* Listen */
	if (listen(server_socket, BACKLOG) < 0)
		perror("Unable to listen on port");

	/* ---------------------------------------------------------------------------- */

	/* Zero select FD sets */
	FD_ZERO(&master_list);
	FD_ZERO(&watch_list);

	/* Register the listening socket */
	FD_SET(server_socket, &master_list);
	/* Register STDIN */
	FD_SET(STDIN, &master_list);

	head_socket = server_socket;

	while (TRUE)
	{
		memcpy(&watch_list, &master_list, sizeof(master_list));

		//printf("\n[PA1-Server@CSE489/589]$ ");
		//fflush(stdout);

		/* select() system call. This will BLOCK */
		selret = select(head_socket + 1, &watch_list, NULL, NULL, NULL);
		if (selret < 0)
			perror("select failed.");

		/* Check if we have sockets/STDIN to process */
		if (selret > 0)
		{
			/* Loop through socket descriptors to check which ones are ready */
			for (sock_index = 0; sock_index <= head_socket; sock_index += 1)
			{

				if (FD_ISSET(sock_index, &watch_list))
				{

					/* Check if new command on STDIN */
					if (sock_index == STDIN)
					{
						char *cmd = (char *)malloc(sizeof(char) * CMD_SIZE);

						memset(cmd, '\0', CMD_SIZE);
						if (fgets(cmd, CMD_SIZE - 1, stdin) == NULL) //Mind the newline character that will be written to cmd
							exit(-1);
						strtok(cmd, "\n");

						if (strcmp(cmd, "AUTHOR") == 0)
						{
							char your_ubit_name[] = "dunjiong";
							printf("I, %s, have read and understood the course academic integrity policy.\n", your_ubit_name);
						}

						if (strcmp(cmd, "PORT") == 0)
						{
							int port = *argv[1];
							printf("[%s:SUCCESS]\n", cmd);
							printf("PORT:%d\n", port);
							printf("[%s:END]\n", cmd);
						}

						if (strcmp(cmd, "IP") == 0)
						{
							printf("[%s:SUCCESS]\n", cmd);
							getIP();
							printf("[%s:END]\n", cmd);
						}
						if (strcmp(cmd, "LIST") == 0)
						{
						printList();
						}

						if (strcmp(cmd, "STATISTICS")){
							stat_printList();

						}


						printf("\nI got: %s\n", cmd);

						//Process PA1 commands here ...

						free(cmd);
					}
					/* Check if new client is requesting connection */
					else if (sock_index == server_socket)
					{
						caddr_len = sizeof(client_addr);
						fdaccept = accept(server_socket, (struct sockaddr *)&client_addr, &caddr_len);
						if (fdaccept < 0)
						{
							perror("Accept failed.");
						}
						//lvk
						struct sockaddr_in addr;

						socklen_t addr_size = sizeof(struct sockaddr_in);

						int res = getpeername(fdaccept, (struct sockaddr *)&addr, &addr_size);

						char *clientip = malloc(20);

						strcpy(clientip, inet_ntoa(addr.sin_addr));

						printf("Client port : %d client id : %s", ntohs(addr.sin_port), clientip);

	
						struct hostent *he;
						struct in_addr ipv4addr;
						struct in6_addr ipv6addr;
						inet_pton(AF_INET, clientip, &ipv4addr);
						he = gethostbyaddr(&ipv4addr, sizeof ipv4addr, AF_INET);
						printf("Host name: %s\n", he->h_name);

						insertFirst(4, he->h_name , clientip , ntohs(addr.sin_port));

						if(stat_find(clientip) != NULL){
							stat_on_status(clientip);
						}else{
							stat_insertFirst(4, he->h_name , clientip , ntohs(addr.sin_port), 0, 0, "logged-in");
						}
					



						// lvk ends
						CONNECTED_CLIENTS++;
						printf("%d", CONNECTED_CLIENTS);

						printf("\nRemote Host connected!\n");

						/* Add to watched socket list */
						FD_SET(fdaccept, &master_list);
						if (fdaccept > head_socket)
							head_socket = fdaccept;
					}
					/* Read from existing clients */
					else
					{
						/* Initialize buffer to receieve response */
						char *buffer = (char *)malloc(sizeof(char) * BUFFER_SIZE);
						memset(buffer, '\0', BUFFER_SIZE);

						if (recv(sock_index, buffer, BUFFER_SIZE, 0) <= 0)
						{

						struct sockaddr_in addr;

						socklen_t addr_size = sizeof(struct sockaddr_in);

						int res = getpeername(fdaccept, (struct sockaddr *)&addr, &addr_size);

						char *clientip = malloc(20);

						strcpy(clientip, inet_ntoa(addr.sin_addr));

							close(sock_index);
							printf("Remote Host terminated connection!\n");
							stat_off_status(clientip); //turn of stat 
							CONNECTED_CLIENTS--;
							printf("%d", CONNECTED_CLIENTS);
							/* Remove from watched list */
							FD_CLR(sock_index, &master_list);
						}
						else
						{
							//Process incoming data from existing clients here ...

						struct sockaddr_in addr;

						socklen_t addr_size = sizeof(struct sockaddr_in);

						int res = getpeername(fdaccept, (struct sockaddr *)&addr, &addr_size);

						char *clientip = malloc(20);

						strcpy(clientip, inet_ntoa(addr.sin_addr));

							printf("this is client ip = %d\n", clientip);
							stat_add_send_message(clientip);
							printf("\nClient sent me: %s\n", buffer);
							printf("ECHOing it back to the remote host ... ");
							if (send(fdaccept, buffer, strlen(buffer), 0) == strlen(buffer))
								printf("Done!\n");
							fflush(stdout);
						}

						free(buffer);
					}
				}
			}
		}
	}

	return 0;
}
