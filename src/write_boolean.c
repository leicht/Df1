/*  Copyright (C) 2002 Stephane LEICHT (stephane.leicht@gmail.com) h
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

//*********************************************************
int write_boolean(int plctype, char *straddress, int *value)
{
	int error=ERROR_READ_INTEGER;
	TThree_Address_Fields address;
	byte posit;
	word valuebool;
	word mask;
	
	if ((error = calc_address(straddress,&address))!=SUCCES)
		return error;
	posit=address.s_eleNumber;
	if (plctype==SLC){
		mask = 0x0001<<posit;
		if (*value)
			valuebool = 0x0001<<posit;
		else
			valuebool = 0x0000;
		error=write_AB(address,valuebool,mask);
	}
	return error;
}
