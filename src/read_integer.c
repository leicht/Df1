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

//*********************************************************
int read_integer(int plctype, char *straddress, word *value)
{
	int error=ERROR_READ_INTEGER;
	TThree_Address_Fields address;
	
	*value=0;//init to 0
	if ((error = calc_address(straddress,&address))!=SUCCES)
		return error;
	if (plctype==SLC)
		error=read_A2(address,value,sizeof(*value));
	return error;
}
