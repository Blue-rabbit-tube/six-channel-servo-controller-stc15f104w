#include "stc15.h" 
#include "softuart.h"
#include "translate.h"
// 9600bps @ 22.1184MHz

typedef bit BOOL;
typedef unsigned char BYTE;
typedef unsigned int WORD;

unsigned int ds=0;
unsigned int i=0;
unsigned int lowms=0;
unsigned int set=0;               
unsigned int sevr[3]={90,90,90};
unsigned int origindata[14];

//BYTE bufrx[5];   
//BYTE buftx[5];   
BYTE bufrxl[30];                         //core

unsigned int mode=2;                  //select the core chip 1 or 2
unsigned int bufnm;


void init()                   //init the timer1/2 and I/O 
{
softuart_init();   	
T2L = 0xea;                   //定时器2  about 1us a period 
T2H = 0xff;	
AUXR |= 0x10;  
IE2 |= 0x04; 
EA=1;	
	               
P35=1;                //io_init set
P32=0;
P33=0;
P34=0;

}


void main()
{
init();
while(1)
{
softuart();
softuart_tx();
if(mode==1)
{
for(bufnm=0;bufnm<=25;bufnm++)
{
if(bufrxl[bufnm]=='s')
{
	
	
if(bufrxl[bufnm+1]=='1')
{ 
	sevr[0]=translate(bufrxl[bufnm+2])*100+translate(bufrxl[bufnm+3])*10+translate(bufrxl[bufnm+4]);
}
if(bufrxl[bufnm+1]=='2')
{
	sevr[1]=translate(bufrxl[bufnm+2])*100+translate(bufrxl[bufnm+3])*10+translate(bufrxl[bufnm+4]);
}
if(bufrxl[bufnm+1]=='3')
{
	sevr[2]=translate(bufrxl[bufnm+2])*100+translate(bufrxl[bufnm+3])*10+translate(bufrxl[bufnm+4]);
}


}
}
}
	


if(mode==2)
{
for(bufnm=0;bufnm<=4;bufnm++)
{
if(bufrxl[bufnm]=='s')
{
if(bufrxl[bufnm+1]=='4')
{
	sevr[0]=translate(bufrxl[bufnm+2])*100+translate(bufrxl[bufnm+3])*10+translate(bufrxl[bufnm+4]);
}
if(bufrxl[bufnm+1]=='5')
{
	sevr[1]=translate(bufrxl[bufnm+2])*100+translate(bufrxl[bufnm+3])*10+translate(bufrxl[bufnm+4]);
}
if(bufrxl[bufnm+1]=='6')
{
	sevr[2]=translate(bufrxl[bufnm+2])*100+translate(bufrxl[bufnm+3])*10+translate(bufrxl[bufnm+4]);
}
}
}
}






	
	
}
}


void high() interrupt 12  //定时器2
{
if(lowms>=1670)                 //20ms计时
{
	i=0;
	ds=0;
	lowms=0;
	P32=0;
	P33=0;
	P34=0;
}
lowms++;


if(ds<=40)
{
P34=1;
P33=1;
P32=1;
}
if(ds>40)
{
if(i<=sevr[0])                 //舵机1角度数据
{
	P34=1;
}
else
{
P34=0;
}


if(i<=sevr[1])                 //舵机2角度数据
{
	P33=1;
}
else
{
P33=0;
}



if(i<=sevr[2])                 //舵机3角度数据
{
	P32=1;
}
else
{
P32=0;
}
i++;}
ds++;
}