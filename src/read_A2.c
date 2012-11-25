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

/***********************************************************************/
/* Cmd:0F Fnc:A2 > Read 3 address fields in SLC500                     */
/* Read Protected Typed Logical                                        */
/***********************************************************************/
int read_A2(TThree_Address_Fields address, void *value, byte size) 
{
	TCmd cmd;
	TMsg sendMsg,rcvMsg;

	int error=ERROR_READ_A2;
	
	bzero(value,size);
	bzero(&sendMsg,sizeof(sendMsg));
	bzero(&rcvMsg,sizeof(rcvMsg));
	bzero(&cmd,sizeof(cmd));
	sendMsg.dst = DEST;
	sendMsg.src = SOURCE;
	sendMsg.cmd = 0x0F;
	sendMsg.sts = 0x00;
	sendMsg.tns = ++tns;
	cmd.fnc = 0xA2;
	cmd.size = address.size;
	cmd.fileNumber = address.fileNumber;
	cmd.fileType = address.fileType;
	cmd.eleNumber = address.eleNumber;
	cmd.s_eleNumber = address.s_eleNumber;
	memcpy(&sendMsg.data,&cmd,sizeof(cmd));
	sendMsg.size = sizeof(cmd);
	if ((error=send_DF1(sendMsg))!=0)
		return error;
	if ((error=rcv_DF1(&rcvMsg))!=0)
		return error;
	if (rcvMsg.tns!=sendMsg.tns)
		return ERROR_TNS_MISMATCH;
	if (rcvMsg.sts!=0)
		return rcvMsg.sts;
	memcpy(value,rcvMsg.data,address.size);
	return SUCCES;
}
