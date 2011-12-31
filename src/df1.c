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

extern int file;
extern word tns;
struct timeval tv;

/***********************************************************************/
word calc_crc (word crc, word buffer) 
{
 word temp1, y;
 //MyLog ("buffer[x] = %04X\n",buffer);
 temp1 = crc ^ buffer;
 crc = (crc & 0xff00) | (temp1 & 0xff);
 for (y = 0; y < 8; y++)
  {
   if (crc & 1)
    {	  
		crc = crc >> 1;
		crc ^= 0xa001;
    }
   else
		crc = crc >> 1;
  }
 return crc;
}

/***********************************************************************/
word compute_crc (TBuffer * buffer) 
{
	byte x;
	word crc = 0;
	for (x=0;x<buffer->size;x++) { 
		crc = calc_crc (crc, buffer->data[x]);
	}
	crc = calc_crc (crc,ETX);
	//MyLog("CRC : %04X\n",crc);
	return (crc);
}


/***********************************************************************/
int send_DF1(TMsg Df1Data)
{
	int error=ERROR_SENDDF1;
	int flag;
	byte c;
	TBuffer crcBuffer,dataSend,dataRcv;
	time_t time_start;
	int nbr_NAK=0;
	int nbr_ENQ=0;
	//int x;
	
	//build message to send
	bzero(&crcBuffer,sizeof(crcBuffer));
	bzero(&dataSend,sizeof(dataSend));
	bzero(&dataRcv,sizeof(dataRcv));
	add_byte2buffer(&dataSend,DLE);	
	add_byte2buffer(&dataSend,STX);
	add_data2bufferWithDLE(&dataSend,Df1Data);
	add_byte2buffer(&dataSend,DLE);	
	add_byte2buffer(&dataSend,ETX);	
	add_data2buffer(&crcBuffer,&Df1Data,Df1Data.size+6);
	add_word2buffer(&dataSend, compute_crc(&crcBuffer));
	//for (x=0;x<dataSend.size;x++) {
	//	printf("datasend %i=%2X\n",x,dataSend.data[x]);
	//}
	//ready to send
	do 
	{	
		if (write (file, &dataSend, dataSend.size)!=dataSend.size)
			return error;
		time_start=time((time_t *) 0);
		tv.tv_sec = TIME_OUT;
      tv.tv_usec = 0;
		//Read for ACK or NAK
		do
		{
			if (is_timeout(time_start)==ERROR_TIMEOUT)
			{
				if (++nbr_ENQ>3) 
					return ERROR_TIMEOUT;
				else
				{
					sendResponse(ENQ);
					time_start=time((time_t *) 0);
					tv.tv_sec = TIME_OUT;
					tv.tv_usec = 0;
				}
			}
			flag=get_symbol(&c);
		} while ((flag!=CONTROL_FLAG) & ((c!=ACK) | (c!=NAK)));
		if (c==ACK)
			 return SUCCES;
		if (c==NAK) 
		{
			error=ERROR_NAK;
			++nbr_NAK;
		}
	} while (nbr_NAK<=3);
	return error;
}	

/***********************************************************************/
int rcv_DF1(TMsg * Df1Data) 
{
	byte c,crcb1,crcb2;
	byte last_response;
	word crc;
	int error = ERROR_RCVDF1;
	TBuffer dataRcv;
	int flag;
	int crcOK=FALSE;
	
	do
	{
		bzero(&dataRcv,sizeof(dataRcv));
		last_response = NAK;
		do
		{
			flag=get_symbol(&c);
			if (flag<0) 
				return flag;
		} while (flag!=CONTROL_FLAG);
		switch (c)
		{	
		case ENQ : 
			sendResponse(last_response); 
			break;
		case STX : 
			while ((flag=get_symbol(&c))!=CONTROL_FLAG)
			{
				add_byte2buffer(&dataRcv,c);
			}
			if (c==ETX)
			{	
            if (read_byte(&crcb1)!=SUCCES)
					return error;
            if (read_byte(&crcb2)!=SUCCES)
					return error;

				crc=bytes2word(crcb1,crcb2);
				if (crc==compute_crc(&dataRcv)) 
				{
					sendResponse(ACK);
					crcOK=TRUE;
					error=SUCCES;
				}	
				else
				{
					sendResponse(NAK);
					crcOK=FALSE;	
				}
			}
			else
			{
				sendResponse(NAK);
			} 
			break;
		default :	sendResponse(NAK);
		}
	} while (crcOK != TRUE);
	memcpy(Df1Data,dataRcv.data,dataRcv.size);
	return error;			
}

/***********************************************************************/
int get_symbol(byte * b)
{
	byte c1,c2;
	int error;
	if ((error=read_byte(&c1))!=SUCCES)
		return error;
	if (c1==DLE)
	{
		if ((error=read_byte(&c2))!=SUCCES) 
			return error;
		*b = c2;
		switch (c2)
		{
			case DLE: return DATA_FLAG;
			case ETX: return CONTROL_FLAG;
			case STX: return CONTROL_FLAG;
			case ENQ: return CONTROL_FLAG;
			case ACK: return CONTROL_FLAG;
			case NAK: return CONTROL_FLAG;	
		}	
	}
	*b = c1;
	return DATA_FLAG;	
}	

/***********************************************************************/
void sendResponse(byte response) // used to send NAK or ACK
{
	word w;
	byte dle=DLE;
	w=bytes2word(dle,response);
	write (file, &w, 2);
}	

/***********************************************************************/
int read_byte(byte * b)
{
	fd_set rfds;
   int retval;
   FD_ZERO(&rfds);
   FD_SET(file, &rfds);
	retval=select(file+1, &rfds, NULL, NULL, &tv);
   switch (retval)
	{
		case 0: 	return ERROR_TIMEOUT;
		case -1: return retval;
		default :
		   if (FD_ISSET(file,&rfds))
			{	
				if (read(file, b, 1)!=1)
					return ERROR;
				else
				{
					//MyLog("Read :%02X\n",*b);
					return SUCCES;
				}
			}
			else
				return ERROR;
	}

}



