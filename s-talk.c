#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "List.h"


pthread_mutex_t mutex;
pthread_cond_t sendCond;
pthread_cond_t recvCond;

int myPortNumber;
char * remoteMachineName;
int remotePortNumber;

LIST * sendList;
LIST * recvList;

int s; // socket
struct sockaddr_in remoteAddr, myAddr; 

pthread_t thread1, thread2, thread3, thread4;
int iret1, iret2, iret3, iret4;

// 4 threads
// print message function -> screen
// keyboard input function -> keyboard
// send to socket -> send
// receive from socket -> recv

/* Get input from keyboard */
void * keyboardThreadFunction (void * ptr) {
	char buffer[1024];

	while (1) {
		if (fgets(buffer, 1024, stdin) != NULL) {
			
			pthread_mutex_lock(&mutex); // acquire a lock on mutex
			ListAdd(sendList, buffer);
			pthread_cond_signal(&sendCond); // wait on the send condition variable
			pthread_mutex_unlock(&mutex); // unlock the mutex
		}

	}
	

}

/* Print the chats to the screen */
void * screenThreadFunction (void * ptr) {
	char * buffer;
	
	while (1) {
		pthread_mutex_lock(&mutex); // acquire a lock on mutex
		pthread_cond_wait(&recvCond, &mutex); // wait on recv condition variable

		while (ListCount(recvList) > 0) { // Get all the messages
			buffer = (char *) ListLast(recvList);

			printf("Received: ");
			fputs(buffer, stdout); // print out to screen
			if ((strcmp(buffer, "!\n") == 0)) { // check if user has typed "!" to exit
				close(s); // close the socket when done
				exit(0);
			}
			pthread_mutex_unlock(&mutex); // unlock mutex
			ListRemove(recvList);
			memset(buffer, 0, 1024);
		}

	}

}

/* Send the message to remote */
void * sendThreadFunction (void * ptr) {
	int size = sizeof(struct sockaddr_in);  

	while (1) {
		pthread_mutex_lock(&mutex); // acquire a lock on mutex
		pthread_cond_wait(&sendCond, &mutex); // wait on send condition variable

		while (ListCount(sendList) > 0) { // Get all the messages
			char * message = (char *) ListLast(sendList);
			int len = strlen(message);
			sendto(s, message, len, 0, (struct sockaddr *) &remoteAddr, size); // send the message
			if ((strcmp(message, "!\n") == 0)) { // check if user has typed "!" to exit
				close(s); // close the socket when done
				exit(0); 
			}
			ListRemove(sendList);
		}
		pthread_mutex_unlock(&mutex); // unlock mutex
	}
	
}

/* Receive the message from remote */
void * recvThreadFunction (void * ptr) {
	char recvBuff[1024];
	int size = sizeof(struct sockaddr_in);

	while (1) {
		if (recvfrom(s, recvBuff, 1024, 0, (struct sockaddr *)&myAddr, &size) > 0) { // receive the message
			pthread_mutex_lock(&mutex); // acquire a lock on mutex
			ListAdd(recvList, recvBuff); 
			pthread_cond_signal(&recvCond); // wait on recv condition variable
			pthread_mutex_unlock(&mutex); // unlock mutex
		}
	}

}


void socketInitialize() {
	/* Create the socket */
	s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s < 0) {
		printf("Cannot create socket.\n");
		exit(0);
	}
	else {
		printf("Socket is created.\n");
	}

	/* fill in my server's address and data */
	memset((char *)&myAddr, 0, sizeof(myAddr));
	myAddr.sin_family = AF_INET;
	myAddr.sin_port = htons(myPortNumber);
	myAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	memset(&myAddr.sin_zero, '\0', 8);

	/* fill in remote server's address and data */
	memset((char *)&remoteAddr, 0, sizeof(remoteAddr));
	remoteAddr.sin_family = AF_INET;
	remoteAddr.sin_port = htons(remotePortNumber);

	struct hostent *hostName; // host information

	/* look up the address of remote server with the given remote name */
	hostName = gethostbyname(remoteMachineName);
	if (!hostName) {
		printf("Could not obtain address of host.\n");
		exit(0);
	}
	memcpy((void *)&remoteAddr.sin_addr, hostName->h_addr_list[0], hostName->h_length);

	if (bind(s, (struct sockaddr *)&myAddr, sizeof(myAddr)) < 0) { // bind to an arbitrary return address
		printf("Bind failed.\n");
		exit(0);
	}

	printf("You are ready to chat!\n");
	
}

int main(int argc, char *argv[]) {
	/* extract the arguments 
		where s-talk [my port number] [remote machine name] [remote port number] */
	myPortNumber = atoi(argv[1]);
	remoteMachineName = argv[2];
	remotePortNumber = atoi(argv[3]);

	/* Create the lists */
	sendList = ListCreate();
	recvList = ListCreate();

	/* mutex initialization */
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&sendCond, NULL);
	pthread_cond_init(&recvCond, NULL);

	/* socket initialization */
	socketInitialize();

	char *keyboardMessage = "keyboard thread";
	char *screenMessage = "screen thread";
	char *sendMessage = "send thread";
	char *receiveMessage = "receive thread";

	/* thread creation */
	iret1 = pthread_create(&thread1, NULL, keyboardThreadFunction, (void *) keyboardMessage);
	iret2 = pthread_create(&thread2, NULL, sendThreadFunction, (void *) sendMessage);
	iret3 = pthread_create(&thread3, NULL, recvThreadFunction, (void *) receiveMessage);
	iret4 = pthread_create(&thread4, NULL, screenThreadFunction, (void *) screenMessage);

	/* join the threads */
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
	pthread_join(thread3, NULL);
	pthread_join(thread4, NULL);


	/* Close when done */
	close(s);
	exit(0);

	return 0;

}

