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

int calc_address(char *straddress,TThree_Address_Fields *address)
{
	int error=ERROR_CALC_ADDRESS;
	int x,l;
	bzero(address,sizeof(*address));
	address->size=0;
	address->fileNumber=0;
	address->fileType=0;
	address->eleNumber=0;
	address->s_eleNumber=0;
	for (x=0;x<strlen(straddress);x++)
		{
		switch (straddress[x])
			{
			case 'O':
				address->fileType = 0x8b;
				address->fileNumber = 0;
				address->size = 2;
				break;
			case 'I':
				address->fileType = 0x8c;
				address->fileNumber = 1;
				address->size = 2;
				break;
			case 'S':
				x++;
				address->fileType = 0x84;
				address->fileNumber = atoi(&straddress[x]);
				address->size = 2;
				break;
			case 'B':
				x++;
				address->fileType = 0x85;
				address->fileNumber = atoi(&straddress[x]);
				address->size = 2;
				break;
			case 'T':
				x++;
				address->fileType = 0x86;
				address->fileNumber = atoi(&straddress[x]);
				address->size = 2;
				break;
			case 'C':
				x++;
				address->fileType = 0x87;
				address->fileNumber = atoi(&straddress[x]);
				address->size = 2;
				break;
			case 'R':
				x++;
				address->fileType = 0x88;
				address->fileNumber = atoi(&straddress[x]);
				address->size = 2;
				break;
			case 'N':
				x++;
				address->fileType = 0x89;
				address->fileNumber = atoi(&straddress[x]);
				address->size=2;
				break;
			case 'F':
				x++;
				address->fileType = 0x8a;
				address->fileNumber = atoi(&straddress[x]);
				address->size = 4;
				break;
			case ':':
				address->eleNumber = atoi(&straddress[++x]);
				break;
			case '.':
			case '/':
				x++;
				if (isdigit(straddress[x])) {
					address->s_eleNumber = atoi(&straddress[x]);
				}
				l=strlen(straddress) - x;
				if (strncasecmp (&straddress[x],"acc",l) == 0 )
					address->s_eleNumber = 2;
				if (strncasecmp (&straddress[x],"pre",l) == 0 )
					address->s_eleNumber = 1;
				if (strncasecmp (&straddress[x],"len",l) == 0 )
					address->s_eleNumber = 1;
				if (strncasecmp (&straddress[x],"pos",l) == 0 )
					address->s_eleNumber = 2;
				if (strncasecmp (&straddress[x],"en",l) == 0 )
					address->s_eleNumber = 13;
				if (strncasecmp (&straddress[x],"tt",l) == 0 )
					address->s_eleNumber = 14;
				if (strncasecmp (&straddress[x],"dn",l) == 0 )
					address->s_eleNumber = 15;				
				x = strlen(straddress)-1;
			}
		}
	error=SUCCES;
	return error;
}
