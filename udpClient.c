//Alejandro RANGEL
//Evrard NGUEMO
//Projet Réseau - UDP
//EI2I HF
//16.04.2020
//Code Client
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "udpServeur.h"

int main() 
{
	int sockfd;
	int n, len;
	int creationCompte, menuPrincipal;
	char buffer[MAXLINE], packet[MAXLINE];
	char *idCompte, *login;
	struct sockaddr_in     servaddr;
  
	// Creating socket file descriptor
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) 
	{
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}
  
	memset(&servaddr, 0, sizeof(servaddr));
      
	// Filling server information
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);
	servaddr.sin_addr.s_addr = INADDR_ANY;
	creationCompte=1;
	len = sizeof(servaddr);
	//Menu
	while(1)
	{
		while(creationCompte==1)//Menu initial
		{
			printf("====================Bienvenue=====================\n");
			printf(" \n");
			printf("-Saisir N pour créer un nouveau compte\n");
			printf(" \n");
			printf("-Saisir C pour se connecter\n");
			printf(" \n");
			printf("-Pour quitter, saisir X\n");
			printf(" \n");
			gets(packet);
			sendto(sockfd, (const char *)packet, strlen(packet), MSG_CONFIRM, (const struct sockaddr *) &servaddr,  sizeof(servaddr));
			
			if(*packet=='X' || *packet=='x')
			{
				bzero(packet, sizeof(packet));
				printf("\n-Merci et à bientôt!\n\n");
				close(sockfd);
				return 0;
			}
			
			if(*packet=='N' || *packet=='n')//Création d'un nouveau compte
			{
				bzero(packet, sizeof(packet));
				printf("====================Créer un compte=====================\n");
				printf(" \n");
				printf("-Nouveau login (maximum 10 caractères): ");
    		gets(packet);
				sendto(sockfd, (const char *)packet, strlen(packet), MSG_CONFIRM, (const struct sockaddr *) &servaddr,  sizeof(servaddr));
				bzero(packet, sizeof(packet));
				
				printf("\n-Nouveau mot de passe (4 chiffres): ");
    		gets(packet);
				sendto(sockfd, (const char *)packet, strlen(packet), MSG_CONFIRM, (const struct sockaddr *) &servaddr,  sizeof(servaddr));
				bzero(packet, sizeof(packet));
				printf(" \n");
			}
			
			if(*packet=='C' || *packet=='c')//Connexion
			{
				bzero(packet, sizeof(packet));
				printf("====================Se connecter=====================\n");
				printf(" \n");
				printf("-Login: ");
				gets(packet);
				sendto(sockfd, (const char *)packet, strlen(packet), MSG_CONFIRM, (const struct sockaddr *) &servaddr,  sizeof(servaddr));
				bzero(packet, sizeof(packet));
				
				printf("\n-Mot de passe: ");
				gets(packet);
				sendto(sockfd, (const char *)packet, strlen(packet), MSG_CONFIRM, (const struct sockaddr *) &servaddr,  sizeof(servaddr));
				bzero(packet, sizeof(packet));
				printf(" \n");
				
				//Reception du login pour affichage
				n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *) &servaddr, &len);
				buffer[n] = '\0';
				strcpy(login, buffer);
				bzero(buffer, sizeof(buffer));
				
				n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *) &servaddr, &len);
				buffer[n] = '\0';
				if(strcmp(buffer, "error")==0)
				{
					printf("Erreur\n");
					bzero(buffer, sizeof(buffer));
					close(sockfd);
					return 0;
				}
				else
				{
					//Reception de l'ID compte pour affichage
					strcpy(idCompte, buffer);
					bzero(buffer, sizeof(buffer));
					creationCompte=0;
				}
			}
		}
		menuPrincipal=1;
		while(menuPrincipal==1)
		{	
			memset (buffer, 0, sizeof (buffer));
			printf("====================Menu Principal=====================\n");
			printf(" \n");
			printf("Bienvenue, %s. ID Compte %s\n\n", login, idCompte);
			printf("-Pour faire un ajout, saisir A\n");
			printf(" \n");
			printf("-Pour faire un retrait, saisir R\n");
			printf(" \n");
			printf("-Pour consulter votre solde, saisir S\n");
			printf(" \n");
			printf("-Pour consulter vos dernières opérations, saisir O\n");
			printf(" \n");
			printf("-Pour quitter, saisir X\n");
			printf(" \n");
			gets(packet);
			sendto(sockfd, (const char *)packet, strlen(packet), MSG_CONFIRM, (const struct sockaddr *) &servaddr,  sizeof(servaddr));
			
			if(*packet=='X' || *packet=='x')
			{
				bzero(packet, sizeof(packet));
				printf("\n-Merci et à bientôt!\n\n");
				close(sockfd);
				return 0;
			}
			
			if(*packet=='A' || *packet=='a')//Fonction Ajout
			{
				bzero(packet, sizeof(packet));
				printf("====================Ajout=====================\n");
				printf(" \n");
				printf("-Saisir la somme (montant maximale: 9999.99): \n");
				gets(packet);
				printf("-Vous avez ajouté %s euros\n", packet);
				sendto(sockfd, (const char *)packet, strlen(packet), MSG_CONFIRM, (const struct sockaddr *) &servaddr,  sizeof(servaddr));//envoi de la somme à ajouter
				bzero(packet, sizeof(packet));
				
				//calcul réalisé coté serveur
				
				bzero(buffer, sizeof(buffer));
				n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *) &servaddr, &len);
				buffer[n] = '\0';
				printf("Nouveau solde: %s\n", buffer);
				printf(" \n");
				bzero(buffer, sizeof(buffer));
			}
			
			if(*packet=='R' || *packet=='r')
			{
				bzero(packet, sizeof(packet));
				printf("====================Retrait=====================\n");
				printf(" \n");
				printf("\nSaisir la somme (montant maximale: 9999.99): \n");
				gets(packet);
				printf("\nVous avez retiré %s euros\n", packet);
				sendto(sockfd, (const char *)packet, strlen(packet), MSG_CONFIRM, (const struct sockaddr *) &servaddr,  sizeof(servaddr));//envoi de la somme à ajouter
				bzero(packet, sizeof(packet));
				
				//calcul réalisé coté serveur
				
				bzero(buffer, sizeof(buffer));
				n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *) &servaddr, &len);
				buffer[n] = '\0';
				printf("\nNouveau solde: %s\n", buffer);
				bzero(buffer, sizeof(buffer));
			}
			
			if(*packet=='S' || *packet=='s')
			{
				bzero(packet, sizeof(packet));
				printf("\n====================Solde=====================\n\n");
				n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *) &servaddr, &len);
				buffer[n] = '\0';
				printf("Solde: %s\n", buffer);
				printf(" \n");
				bzero(buffer, sizeof(buffer));
				
			}
			
			if(*packet=='O' || *packet=='o')
			{
				bzero(packet, sizeof(packet));
				printf("\n===============Dernière Opération==============\n\n");
				printf("Nom: %s\n", login);
				printf("\nID: %s\n", idCompte);
				
				n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *) &servaddr, &len);
				buffer[n] = '\0';
				printf("\nAction: %s\n", buffer);
				bzero(buffer, sizeof(buffer));
				
				n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *) &servaddr, &len);
				buffer[n] = '\0';
				printf("\nMontant: %s\n", buffer);
				bzero(buffer, sizeof(buffer));
				
				n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *) &servaddr, &len);
				buffer[n] = '\0';
				printf("\nTime: %s\n", buffer);
				bzero(buffer, sizeof(buffer));
			}
			
			if(*packet=='X' || *packet=='x')
			{
				bzero(packet, sizeof(packet));
				printf("\n-Merci et à bientôt!\n");
				close(sockfd);
				return 0;
			}
		}
	}
}
