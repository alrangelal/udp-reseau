#ifndef _UDPSERVEUR_H_
#define _UDPSERVEUR_H_

#define PORT    1977
#define MAXLINE 1024

typedef struct
{
	char action[MAXLINE];
	char montant[MAXLINE];
}t_operation;
  
typedef struct
{
	char login[MAXLINE];
	char password[MAXLINE];
	float total;
	char somme[MAXLINE];
}t_client;

typedef t_client listeClients[100];

#endif
