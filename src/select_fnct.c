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
//return 1:read_integer 2:read_boolean 3:read_float
//return 11:write_integer 12:write_boolean 13:write_float
int select_fnct(char *strquery, char *address, char *value)
{
	int result=ERROR;
	int x;
	char filetype[]= "OISBTCRNF";
	int writequery=FALSE;
	char *varaddress;
	char *varvalue;
	const char delimiters[] = "=\n\r";
	
	//begin by alphaFile ?
	if (strchr(filetype,toupper(strquery[0]))==NULL) {
		return ERROR_BAD_QUERY;
	}
	//find ':' in query ?
	if (strchr(strquery,58)==NULL) {
		return ERROR_BAD_QUERY;
	}	

	//find '=' in query ?	if yes > write value in SLC
	if (strchr(strquery,61)!=NULL) {
		writequery=TRUE;
	}	
	
	varaddress = strsep (&strquery, delimiters);
	//upper ALL
	for (x=0;x<strlen(varaddress);x++) {
		if (isalpha(varaddress[x]))
			varaddress[x]=toupper(varaddress[x]);
	}
	//printf("substring=%i=%s\n",strlen(varaddress),varaddress);
	strcpy(address,varaddress);
	
	if (writequery) {
		varvalue = strsep (&strquery, delimiters);
		//printf("substring=%i=%s\n",strlen(varvalue),varvalue);
		strcpy(value,varvalue);
	}

	switch (toupper(varaddress[0]))
		{
		case 'O':
		case 'I':
		case 'S':
		case 'B':
		case 'T':
		case 'C':
		case 'R':
		case 'N':
			result=1;			
			break;
		case 'F':
			result=3;
			break;
	}
		
	//find '.' in query ?
	if (strchr(varaddress,'.')!=NULL) {
		result=2;
	}
	//find '/' in query ?
	if (strchr(varaddress,'/')!=NULL) {
		result=2;
	}
	//find ACC in query ?
	if (strstr(varaddress,"ACC")!=NULL) {
		result=1;
	}
	//find PRE in query ?
	if (strstr(varaddress,"PRE")!=NULL) {
		result=1;
	}	
	//find LEN in query ?
	if (strstr(varaddress,"LEN")!=NULL) {
		result=1;
	}
	//find POS in query ?
	if (strstr(varaddress,"POS")!=NULL) {
		result=1;
	}	
	//find EN in query ?
	if (strstr(varaddress,"EN")!=NULL) {
		result=2;
	}
	//find TT in query ?
	if (strstr(varaddress,"TT")!=NULL) {
		result=2;
	}
	//find DN in query ?
	if (strstr(varaddress,"DN")!=NULL) {
		result=2;
	}
	if (writequery)
		result +=10;
	return result;
}
