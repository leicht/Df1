/*  Copyright (C) 2002 Stephane LEICHT (stephane.leicht@foxinfo.fr) http://www.foxinfo.fr
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "df1.h"

extern int Terminated;

//*********************************************************
int server(void)
{
	struct sockaddr_in monadr, sonadr;
	int fd, opt=1, taille;
	int serv_sock, client_sock;
   int nb_lu;	
	int nfds;
	fd_set rfds, afds;
	int error;
	
	char message[20]="";
	char response[20]="";
	
	MyLog("Create server ...\n");
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR,(char *) &opt, sizeof(opt));
	
	/* init socket serveur */
	bzero( (char *)&monadr, sizeof monadr);
	monadr.sin_family = AF_INET;
	monadr.sin_port = htons(PORT);
	monadr.sin_addr.s_addr = INADDR_ANY;

	if( bind(serv_sock, (struct sockaddr *)&monadr, sizeof(monadr)) == -1 ) {
		return(1);
	}

	if( listen(serv_sock, MAXCONN) == -1 ) {
		return(1);
	}
	
	/* init sockets list*/
	nfds = getdtablesize();
	FD_ZERO(&afds);
	FD_SET(serv_sock, &afds);

	/* gestion des clients */
	while(!Terminated) 
	{
		memcpy(&rfds, &afds, sizeof(rfds));
		if( select(nfds, &rfds, 0, 0, 0) == -1 ) 
			{/* signaux non bloquants */
				if( errno == EINTR ) continue;
				return(1);
			}
		if( FD_ISSET(serv_sock, &rfds) ) /* New connection ?*/
			{
				taille = sizeof sonadr;
				if( (client_sock = accept(serv_sock, (struct sockaddr *)&sonadr,(socklen_t *)&taille)) == -1 ) 
					{
						return(1);
					}
				//syslog(LOG_NOTICE,"client connection from %s \n", inet_ntoa(sonadr.sin_addr));
				fflush(stdout);
				FD_SET(client_sock, &afds);
				fcntl(client_sock, F_SETFL, O_NONBLOCK | fcntl(client_sock, F_GETFL, 0));
			}
		/* Tester si les sockets clientes ont boug�s */
		for( fd=0; fd<nfds; fd++ ) {
			if( fd != serv_sock && FD_ISSET(fd, &rfds) ) {
				/* traiter le client */
				strcpy(message,"");
				strcpy(response,"");
				nb_lu = read(fd, message, BUFSIZE);
				//MyLog("nb_lu : %d/%d/%d\n",nb_lu,strlen(message),fd);
				if( nb_lu > 0 ) {
					message[nb_lu]='\0';
					if ((error=read_socket(message,response))!=SUCCES) {
						sprintf(response,"error :%d\n",error);
					}
					strcat(response,"\n");
					write(fd, response, strlen(response));
				} else {
					close(fd);
					FD_CLR(fd, &afds);
				}
			}
		}
	}
	return(0);
}

