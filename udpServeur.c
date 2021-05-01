//Alejandro RANGEL
//Evrard NGUEMO
//Projet Réseau - UDP
//EI2I HF
//16.04.2020
//Code Serveur
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "udpServeur.h"

int main() 
{
	int sockfd;
	int menuLogin;
	int len, n, connecte=0, i, nClient=1, nOperation=1000, val, flag;
	char buffer[MAXLINE], packet[MAXLINE];
	char *error="error";
	char *rien="-----------";
	float res;
	t_client listeClients[100];
	t_operation listeOperation[100];
	t_client* client=malloc(sizeof(t_client));
	struct sockaddr_in servaddr, cliaddr;
      
	// Creating socket file descriptor
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) 
	{
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}
      
	memset(&servaddr, 0, sizeof(servaddr));
      
	// Filling server information
	servaddr.sin_family    = AF_INET; // IPv4
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(PORT);
      
	// Bind the socket with the server address
	if ( bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 )
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	menuLogin=1;
	len = sizeof(cliaddr);  //len is value/result
  memset (buffer, 0, sizeof (buffer));
  memset (packet, 0, sizeof (packet));
  printf("\n=============Nouvelle connexion===============\n\n");
  
  //Gestion serveur
	while(1)
	{
		bzero(buffer, sizeof(buffer));
		bzero(packet, sizeof(packet));
		while(menuLogin==1)//Menu Login
		{
			bzero(buffer, sizeof(buffer));
			bzero(packet, sizeof(packet));
			n = recvfrom(sockfd, (char *)buffer, MAXLINE,  MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len);
			buffer[n] = '\0';
			printf("Option: %s\n", buffer);
			
			if(*buffer=='X' || *buffer=='x')
			{
				bzero(buffer, sizeof(buffer));
				printf("\n=============Nouvelle connexion===============\n\n");
				menuLogin=1;
			}
			
			if(*buffer=='N' || *buffer=='n')//Création compte
			{
				bzero(buffer, sizeof(buffer));
				printf("\n==============Nouveau Compte===============\n\n");
				/*Reception Nouveau Login*/
				n = recvfrom(sockfd, (char *)buffer, MAXLINE,  MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len);
				buffer[n] = '\0';
				strcpy(listeClients[nClient].login, buffer); //enregistre le login saisi dans un espace compte vide
				printf("Nouveau Login : %s\n", listeClients[nClient].login);
				bzero(buffer, sizeof(buffer));
				
				/*Reception Nouveau MDP*/
				n = recvfrom(sockfd, (char *)buffer, MAXLINE,  MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len);
				buffer[n] = '\0';
				strcpy(listeClients[nClient].password, buffer); //enregistre le mot de passe saisi dans un espace compte vide
				printf("Nouveau Mot de Passe : %s\n", listeClients[nClient].password);
				bzero(buffer, sizeof(buffer));
				
				/*Solde nul par défaut*/
				listeClients[nClient].total=0.0f;
				nClient++;
				
			}
			
			if(*buffer=='C' || *buffer=='c')//Connexion
			{
				printf("\n==================Login===================\n\n");
				
				/*Reception Login*/
				bzero(buffer, sizeof(buffer));
				n = recvfrom(sockfd, (char *)buffer, MAXLINE,  MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len);
				buffer[n] = '\0';
				strcpy(client->login, buffer); //enregistre le login saisi dans une memo pour pouvoir aller chercher dans le tableau
				printf("Login : %s\n", client->login);
				bzero(buffer, sizeof(buffer));
				
				/*Reception MDP*/
				n = recvfrom(sockfd, (char *)buffer, MAXLINE,  MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len);
				buffer[n] = '\0';
				strcpy(client->password, buffer);//enregistre le mdp saisi dans une memo pour pouvoir aller chercher dans le tableau
				printf("Mot de Passe : %s\n", client->password);
				bzero(buffer, sizeof(buffer));
				val=1;
				for(i=0;val!=0;i++)//parcours de la liste
				{
					//printf("%d",i); //DEBUG
					if(strcmp(client->login,listeClients[i].login)==0)
					{
						if(strcmp(client->password,listeClients[i].password)==0)
						{
							strcpy(packet, listeClients[i].login);
							sendto(sockfd, (const char *)packet, strlen(packet), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);
							bzero(packet, sizeof(packet));
							sprintf(packet, "%d", i);
							printf("\nID Compte : %s\n", packet);
							sendto(sockfd, (const char *)packet, strlen(packet), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);
							bzero(packet, sizeof(packet));
							//printf("\nSolde initial=%f\n", listeClients[i].total), //DEBUG
							menuLogin=0;
							connecte=1;
							val=0;
						}
					}
				}
			}
		}
		
		while(connecte==1)
		{
			//printf("\ni=%d\n", i); //DEBUG
			bzero(buffer, sizeof(buffer));
			bzero(packet, sizeof(packet));
			n = recvfrom(sockfd, (char *)buffer, MAXLINE,  MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len);
			buffer[n] = '\0';
			printf("Option: %s\n", buffer);
			
			/*if(*buffer=='X' || *buffer=='x')
			{
				bzero(buffer, sizeof(buffer));
				printf("\n=============Nouvelle connexion===============\n\n");
				menuLogin=1;
				connecte=0;
			}*/
			
			if(*buffer=='A' || *buffer=='a')
			{
				bzero(buffer, sizeof(buffer));
				bzero(packet, sizeof(packet));
				printf("\n==================Ajout===================\n\n");
				n = recvfrom(sockfd, (char *)buffer, MAXLINE,  MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len);
				buffer[n] = '\0';
				
				/*Dernière operation*/
				strcpy(listeOperation[i].action, "AJOUT");
				strcpy(listeOperation[i].montant, buffer);

				res=atof(buffer);
				printf("Ajout de %.2f euros\n", res);
				
				listeClients[i].total+=res;
				sprintf(listeClients[i].somme, "%f", listeClients[i].total);
				strcpy(packet, listeClients[i].somme);
				printf("Total= %s\n", packet);
				sendto(sockfd, (const char *)packet, strlen(packet), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);
				bzero(buffer, sizeof(buffer));
				bzero(packet, sizeof(packet));
				nOperation++;
			}
			
			if(*buffer=='R' || *buffer=='r')
			{
				bzero(buffer, sizeof(buffer));
				bzero(packet, sizeof(packet));
				printf("\n=================Retrait==================\n\n");
				n = recvfrom(sockfd, (char *)buffer, MAXLINE,  MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len);
				buffer[n] = '\0';
				
				/*Remplissage du tableau operations*/
				strcpy(listeOperation[i].action, "RETRAIT");
				strcpy(listeOperation[i].montant, buffer);

				
				res=atof(buffer);
				printf("Retrait de %.2f euros\n", res);
				
				listeClients[i].total-=res;
				sprintf(listeClients[i].somme, "%f", listeClients[i].total);
				strcpy(packet, listeClients[i].somme);
				printf("Total= %s\n", packet);
				sendto(sockfd, (const char *)packet, strlen(packet), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);
				bzero(buffer, sizeof(buffer));
				bzero(packet, sizeof(packet));
			}
			
			if(*buffer=='S' || *buffer=='s')
			{
				bzero(buffer, sizeof(buffer));
				bzero(packet, sizeof(packet));
				printf("\n===================Solde===================\n\n");
				sprintf(listeClients[i].somme, "%f", listeClients[i].total);
				strcpy(packet, listeClients[i].somme);
				printf("Total= %s\n", packet);
				sendto(sockfd, (const char *)packet, strlen(packet), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);
				bzero(packet, sizeof(packet));
			}
			
			if(*buffer=='O' || *buffer=='o')
			{
				bzero(buffer, sizeof(buffer));
				printf("\n================Opérations=================\n\n");
				/*Transmission Opérations*/
				strcpy(packet, listeOperation[i].action);
				printf("Action: %s\n", packet);
				sendto(sockfd, (const char *)packet, strlen(packet), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);
				bzero(packet, sizeof(packet));
				
				strcpy(packet, listeOperation[i].montant);
				printf("Montant: %s\n", packet);
				sendto(sockfd, (const char *)packet, strlen(packet), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);
				bzero(packet, sizeof(packet));
				
				time_t ticks;
				ticks = time(NULL);
				snprintf(packet,sizeof(packet),"%24s\r\n",ctime(&ticks));
				printf("Time: %s\n", packet);
				sendto(sockfd, (const char *)packet, strlen(packet), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);
				bzero(packet, sizeof(packet));
			}
			
			if(*buffer=='X' || *buffer=='x')//Déconnexion du client
			{
				bzero(buffer, sizeof(buffer));
				printf("\n=============Nouvelle connexion===============\n\n");
				menuLogin=1;
				connecte=0;
			}
		}
  }    
	return 0;
}
