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
int read_socket(char *rcvmsg, char *response)
{
	int error=ERROR;
	int fnct;
	int plctype=SLC;
	word value;
	int varbool;
	float varfloat;
	char Address[20];
	char writevalue[20];
	
	fnct=select_fnct(rcvmsg,Address,writevalue);
	switch (fnct)
	{
		case 1 :
			if ((error=read_integer(plctype,Address,&value))==SUCCES) { 
				sprintf(response,"%d",value);
				//strcat(response,"\n");
			}
			break;
		case 2 :
			if ((error=read_boolean(plctype,Address,&varbool))==SUCCES) { 
				sprintf(response,"%i",varbool);
				//strcat(response,"\n");
			}
			break;	
		case 3 :
			if ((error=read_float(plctype,Address,&varfloat))==SUCCES) { 
				sprintf(response,"%f",varfloat);
				//strcat(response,"\n");
			}
			break;			
		case 11 :
			value = atoi(writevalue);
			if ((error=write_integer(plctype,Address,&value))==SUCCES) { 
				sprintf(response,"%d",value);
				//strcat(response,"\n");
			}
			break;
		case 12 :
			varbool = atoi(writevalue);
			if ((error=write_boolean(plctype,Address,&varbool))==SUCCES) { 
				sprintf(response,"%d",varbool);
				//strcat(response,"\n");
			}
			break;			
		case 13 :
			varfloat = atof(writevalue);
			if ((error=write_float(plctype,Address,&varfloat))==SUCCES) { 
				sprintf(response,"%f",varfloat);
				//strcat(response,"\n");
			}
			break;			
		default:
			error=fnct;
			break;
	}
	return error;
}
