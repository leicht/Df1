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

/* This file is inspired by source code of LibModbus 0.0.1
Author: Laurent LOPES
e-mail: pes@free.fr
homepage: pes.free.fr  */

#include "df1.h"

word tns;

/***********************************************************************************/
void Df1_close_device(int Df1_device)
{
  if (tcsetattr (Df1_device,TCSANOW,&saved_tty_parameters) < 0)
    MyLog("Can't restore terminal parameters ");
  close(Df1_device);
}

/************************************************************************************
		Df1_open_device : open the device
*************************************************************************************
input :
-------
Df1_port   : string with the device to open (/dev/ttyS0, /dev/ttyS1,...)
Df1_speed  : speed (baudrate)
Df1_parity : 0=don't use parity, 1=use parity EVEN, -1 use parity ODD
Df1_bit_l  : number of data bits : 7 or 8 	USE EVERY TIME 8 DATA BITS
Df1_bit_s  : number of stop bits : 1 or 2    ^^^^^^^^^^^^^^^^^^^^^^^^^^

answer  :
---------
device descriptor
************************************************************************************/
int Df1_open_device(char Df1_port[20], int Df1_speed, int Df1_parity, int Df1_bit_l, int Df1_bit_s)
{
  int fd;

  /* open port */
  fd = open(Df1_port,O_RDWR | O_NOCTTY  | O_NDELAY) ;//| O_NONBLOCK
  if(fd<0)
  {
    MyLog("Open device failure\n") ;
    exit(-1) ;
  }

  /* save olds settings port */
  if (tcgetattr (fd,&saved_tty_parameters) < 0)
  {
    MyLog("Can't get terminal parameters ");
    return -1 ;
  }

  /* settings port */
  bzero(&Df1_tio,sizeof(&Df1_tio));

  switch (Df1_speed)
  {
     case 0:
        Df1_tio.c_cflag = B0;
        break;
     case 50:
        Df1_tio.c_cflag = B50;
        break;
     case 75:
        Df1_tio.c_cflag = B75;
        break;
     case 110:
        Df1_tio.c_cflag = B110;
        break;
     case 134:
        Df1_tio.c_cflag = B134;
        break;
     case 150:
        Df1_tio.c_cflag = B150;
        break;
     case 200:
        Df1_tio.c_cflag = B200;
        break;
     case 300:
        Df1_tio.c_cflag = B300;
        break;
     case 600:
        Df1_tio.c_cflag = B600;
        break;
     case 1200:
        Df1_tio.c_cflag = B1200;
        break;
     case 1800:
        Df1_tio.c_cflag = B1800;
        break;
     case 2400:
        Df1_tio.c_cflag = B2400;
        break;
     case 4800:
        Df1_tio.c_cflag = B4800;
        break;
     case 9600:
        Df1_tio.c_cflag = B9600;
        break;
     case 19200:
        Df1_tio.c_cflag = B19200;
        break;
     case 38400:
        Df1_tio.c_cflag = B38400;
        break;
     case 57600:
        Df1_tio.c_cflag = B57600;
        break;
     case 115200:
        Df1_tio.c_cflag = B115200;
        break;
     case 230400:
        Df1_tio.c_cflag = B230400;
        break;
     default:
        Df1_tio.c_cflag = B9600;
  }
  switch (Df1_bit_l)
  {
     case 7:
        Df1_tio.c_cflag = Df1_tio.c_cflag | CS7;
        break;
     case 8:
     default:
        Df1_tio.c_cflag = Df1_tio.c_cflag | CS8;
        break;
  }
  switch (Df1_parity)
  {
     case 1:
        Df1_tio.c_cflag = Df1_tio.c_cflag | PARENB;
        break;
     case -1:
        Df1_tio.c_cflag = Df1_tio.c_cflag | PARENB | PARODD;
        break;
     case 0:
     default:
        Df1_tio.c_iflag = IGNPAR;
        break;
  }

  if (Df1_bit_s==2)
     Df1_tio.c_cflag = Df1_tio.c_cflag | CSTOPB;
     
  Df1_tio.c_cflag = Df1_tio.c_cflag | CLOCAL | CREAD ;
  Df1_tio.c_oflag = 0;
  Df1_tio.c_lflag = 0; /*1=ICANON;*/
  Df1_tio.c_cc[VMIN]=0;
  Df1_tio.c_cc[VTIME]=0;

  /* clean port */
  tcflush(fd, TCIFLUSH);

  fcntl(fd, F_SETFL, FASYNC);
  /* activate the settings port */
  if (tcsetattr(fd,TCSANOW,&Df1_tio) <0)
  {
    MyLog("Can't set terminal parameters ");
    return -1 ;
  }
  
  /* clean I & O device */
  tcflush(fd,TCIOFLUSH);
  /* init tns for DF1 */
  tns = (word) time((time_t *)0);
  
  return fd ;
}





