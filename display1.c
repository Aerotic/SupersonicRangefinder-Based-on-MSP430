/*
 * display1.h
 *This capetible for digitaltubes with TM1637 inside
 *Or those with I2C ports
 *  Created on: Nov.20th 2016
 *      Author: Aeroman
 */
#include<msp430.h>

unsigned char fig[] = {0x3f,//0
			      0x06,//1
				  0x5b,//2
  				  0x4f,//3
				  0x66,//4
				  0x6d,//5
				  0x7d,//6
				   0x07,//7
        		   0x7f,//8
        		   0x6f,//9
        		   0x77,//A
        		   0x7c,//b
        		   0x39,//C
		   	   	   0x5e,//d
		   	   	   0x79,//E
		   	   	   0x71,//F
		   	   	   0x31,//R
		   	   	   0x00//ledoff
		   	   	   };
unsigned char b_fig[][4]={{0x01,0x01,0x01,0x01},
						  {0x23,0x21,0x21,0x23},
						  {0x77,0x79,0x31,0x3f},};


#define CPU_F ((double)16000000)
#define delay(x) _delay_cycles((long)(CPU_F*(double)x/1000.0))

void start();
void stop();
void writeByte1(unsigned char a);
void display(unsigned char data0,unsigned char data1,unsigned char data2,unsigned char data3);
void c0(float a);
void dis_r(char a,char b,char c,char d);
//*****************basic fuctions*****************
void start()//send start signal
{
	P2DIR|=BIT4|BIT5;
	P2OUT|=BIT5|BIT4;
	P2OUT&=~BIT4;
}
void stop()//send stop signal
{
	P2DIR|=BIT5|BIT4;
	P2OUT|=BIT5;
	P2OUT&=~BIT4;
	P2OUT|=BIT4;
}
void writeByte1(unsigned char a)//send 8bit data
{
	P2DIR|=BIT5|BIT4;
	P2OUT&=~BIT5;
	P2OUT&=~BIT4;
	char i;
	for(i=0;i<8;i++)
	{
		P2OUT&=~BIT5;
		if(a&0x01)
			P2OUT|=BIT4;
		else
			P2OUT&=~BIT4;
		P2OUT|=BIT5;
		a>>=1;
	}
    P2OUT&=~BIT5;
    P2DIR&=~BIT4;
    while(P2IN&BIT4);//wait ACK signal
    P2DIR|=BIT4;
    P2OUT|=BIT5;
    P2OUT&=~BIT5;
}
//*****************end of basic fuctions*****************

//*****************derived fuctions*****************
void display(unsigned char data0,unsigned char data1,unsigned char data2,unsigned char data3)
{
	        //set fixed address mode
			start();
	    	writeByte1(0x44);
	    	stop();
	    	//end


	    	//set first address
	    	start();
	    	writeByte1(0xC0);
	    	stop();
	    	//end

	    	//send data0
	    	start();
	    	writeByte1(data0);
	    	stop();


	    	//set second address
	    	start();
	    	writeByte1(0xC1);
	    	stop();

	    	//send data1
	    	start();
	    	writeByte1(data1);
	    	stop();


	    	//set third address
	    	start();
	    	writeByte1(0xC2);
	    	stop();

	    	//send data2
	    	start();
	    	writeByte1(data2);
	    	stop();


	    	//set fourth address
	    	start();
	    	writeByte1(0xC3);
	    	stop();

	    	//send data3
	    	start();
	    	writeByte1(data3);
	    	stop();


	    	//set the lightness to the brightest
	    	start();
	    	writeByte1(0x8F);
	    	stop();
}
//*****************end of derived fuctions*****************

//*****************advanced fuctions*****************
void bootfig()
{
	char i;
	for(i=0;i<8;i++)
	{
		display(b_fig[0][0],b_fig[0][1],b_fig[0][2],b_fig[0][3]);
		delay(800);
		display(b_fig[1][0],b_fig[1][1],b_fig[1][2],b_fig[1][3]);
		delay(800);
		display(b_fig[2][0],b_fig[2][1],b_fig[2][2],b_fig[2][3]);
		delay(800);
//		display(b_fig[3][0],b_fig[3][1],b_fig[3][2],b_fig[3][3]);
//		delay(800);
	}
}

void dis_r(char a,char b,char c,char d)
{
	display(fig[a],fig[b],fig[c],fig[d]);
}

void c0(float a)//1<a<200
{
	long int d100,d10,d1,mode,d01,d1000;
	d1000=(long int )a/1000;
   d100=((long int)a%1000)/100;
   d10=((long int)a%100)/10;
   d1=(long int)a%10;
   d01=(10*(long int)a)%10;
   if(d01>4)
	   d1++;
   mode=0x00;
   display(fig[d1000],fig[d100],fig[d10],fig[d1]);
}

