/*  Copyright (C) 2002 Stephane LEICHT (stephane.leicht@gmail.com)
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

extern word tns;
int file;
int Terminated=FALSE;

void Termine (int sig);

//******************** MAIN ************
int main (void) 
{
	openlog("DF1",LOG_NDELAY,LOG_DAEMON);
	setlogmask(~LOG_MASK(LOG_DEBUG)); // no debug informations
	
	signal(SIGTERM,Termine);
	signal(SIGINT,Termine);
	signal(SIGQUIT,Termine);
	signal(SIGSEGV,Termine);
	
	if ((file=Df1_open_device ("/dev/ttyS0", 9600,0,8,1)) == -1)
		{
			MyLog("OpenCom Failed\n");
			return (-1);
		}

	#ifndef DEBUG	
	switch (fork())
		{
		case -1:
						syslog(LOG_NOTICE,"Erreur a la creation du Daemon");
						//closelog;
						exit(1);
		case 0:
						setsid();
						chdir("/");
						umask(0);
						close(0);
						close(1);
						close(2);
						syslog(LOG_NOTICE,"Daemon OK");
						if (file == -1)
							{
								syslog(LOG_NOTICE,"OpenCom Failed\n");
								//closelog;
								exit(2);
							}
							else 
							{	
	#endif
					  server();	
					  close (file);
					  exit(0);
#ifndef DEBUG
							}
	default : exit(0);
	}
#endif	
}
//*********************************************
void Termine (int sig)
{
	switch (sig)
	{
		case SIGTERM:	syslog(LOG_NOTICE,"receive SIGTERM\n");
							Terminated=TRUE;
							break;
		case SIGINT:	syslog(LOG_NOTICE,"receive SIGINT\n");
							Terminated=TRUE;
							break;
		case SIGQUIT:	syslog(LOG_NOTICE,"receive SIGQUIT\n");
							Terminated=TRUE;
							break;
		case SIGSEGV:	syslog(LOG_NOTICE,"receive SIGSEGV, program ERROR\n");
							Terminated=TRUE;
							break;
		default:	syslog(LOG_NOTICE,"receive signal: %d\n",sig);
					Terminated=TRUE;
					break;
	}
}




