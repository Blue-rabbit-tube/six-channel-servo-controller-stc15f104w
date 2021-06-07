#include "stc15.h"
#include "softuart.h"


#define BAUD  0xFD00                  // 9600bps @ 22.1184MHz

int uart2datacount=0;
char bittest1;
int s=0;



#define RXB P30                        //define UART TX/RX port
#define TXB P31

typedef bit BOOL;
typedef unsigned char BYTE;
typedef unsigned int WORD;

BYTE TBUF,RBUF;
BYTE TDAT,RDAT;
BYTE TCNT,RCNT;
BYTE TBIT,RBIT;
BOOL TING,RING;
BOOL TEND,REND;

void UART_INIT();

BYTE t, r;


//core

//extern BYTE bufrx[5];                         //core
extern BYTE bufrxl[30];                         //core
//extern BYTE buftx[30];                         //core



void UART_INIT()
{
    TING = 0;
    RING = 0;
    TEND = 1;
    REND = 0;
    TCNT = 0;
    RCNT = 0;
}



void softuart_init()
{
    TMOD = 0x00;                        //timer0 in 16-bit auto reload mode
    AUXR = 0x80;                        //timer0 working at 1T mode
    TL0 = BAUD;
    TH0 = BAUD>>8;                      //initial timer0 and set reload value
    TR0 = 1;                            //tiemr0 start running
    ET0 = 1;                            //enable timer0 interrupt
    PT0 = 1;                            //improve timer0 interrupt priority
    EA = 1;                             //open global interrupt switch

    UART_INIT();
}	
void softuart()
    {          
                               			//user's function
        if (REND)
        {
            REND = 0;
					  bittest1=RBUF;
        //    bufrx[r++ & 0x05] = RBUF;     //where to RX the data;
										
					if(bittest1!='\n')
			{
			bufrxl[uart2datacount]=bittest1;
				uart2datacount++;
				r=1;
			}
				else
				{
				uart2datacount=0;
				}
					
			
        }
   //     if (TEND)
    //    {
          //  if (t != r)
          //  {
          //      TEND = 0;
         //     TBUF = bufrxl[t++ & 0x05];  //where to TX the data;
				//	    for(s=0;s<=4;s++)
				//	      TBUF = bufrxl[s];
         //       TING = 1;
          //  }
    //    }	
    }
		
		
void softuart_tx()
{
if (TEND)
        {
					if(r==1&&s<=4)
          {     
						    TEND = 0;
					      TBUF = bufrxl[s];
                TING = 1;
						    s++;
					}
					if(s>4)
					{
					r=0;
					s=0;	
					}
        }
}


void tm0() interrupt 1
{
    if (RING)
    {
        if (--RCNT == 0)
        {
            RCNT = 3;                   //reset send baudrate counter
            if (--RBIT == 0)
            {
                RBUF = RDAT;            //save the data to RBUF
                RING = 0;               //stop receive
                REND = 1;               //set receive completed flag
            }
            else
            {
                RDAT >>= 1;
                if (RXB) RDAT |= 0x80;  //shift RX data to RX buffer
            }
        }
    }
    else if (!RXB)
    {
        RING = 1;                       //set start receive flag
        RCNT = 4;                       //initial receive baudrate counter
        RBIT = 9;                       //initial receive bit number (8 data bits + 1 stop bit)
    }

    if (--TCNT == 0)
    {
        TCNT = 3;                       //reset send baudrate counter
        if (TING)                       //judge whether sending
        {
            if (TBIT == 0)
            {
                TXB = 0;                //send start bit
                TDAT = TBUF;            //load data from TBUF to TDAT
                TBIT = 9;               //initial send bit number (8 data bits + 1 stop bit)
            }
            else
            {
                TDAT >>= 1;             //shift data to CY
                if (--TBIT == 0)
                {
                    TXB = 1;
                    TING = 0;           //stop send
                    TEND = 1;           //set send completed flag
                }
                else
                {
                    TXB = CY;           //write CY to TX port
                }
            }
        }
    }
}

