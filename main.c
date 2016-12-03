#include<msp430.h>
#include"display1.h"//The display device library

unsigned int  result1_start,result1_end;
unsigned int index=0;
double  temp;
double   distance;


void CloseWDTCTL()
{
    WDTCTL=WDTPW+WDTHOLD;
}
void ClockInit()
{
    BCSCTL1=CALBC1_16MHZ;
    DCOCTL=CALDCO_16MHZ;
}
void PortInit()
{
        P1DIR &=~BIT2;          //Echo pin
        P1SEL |= BIT2;   //
        P2DIR|=BIT0;
}
void Timer1Init()
{
        TACCTL1 = CAP + CCIS_0 + CM_3 + CCIE+SCS;     // Capture CCIxA, both edge, interrupt enable.CCI1A->p1.2
        TACTL |=TASSEL_2 + ID_0 + MC_2+ TAIE+TACLR ;//£¬continuous mode
        TACCR1=0;
        //TACTL&=~CCIFG;
}
void send_15us()//
{
         P2OUT&=~BIT0;
         P2OUT|=BIT0;
         _delay_cycles(200);
         P2OUT&=~BIT0;
}
char cd;
double td;
int main(void)
{
          CloseWDTCTL();
        ClockInit();
        PortInit();
       Timer1Init();
       //bootfig();
       display(0x3f,0x3f,0x3f,0x3f);
        _EINT();
        cd=0;
    while(1)
    {
    	//Mark 1
            send_15us();
            if(cd<2)
            	_delay_cycles(0.8*16000000);//normal scan period
            else
            	_delay_cycles(0.225*16000000);//"skip" mode scan period
    }
    return 0;
}

#pragma vector=TIMER0_A1_VECTOR

__interrupt void TAIV_ISR(void)
{

         switch(TA0IV)
                {
                 case 2:
                         if (TACCTL1&CCI)    //intertupt at the positive edge
                                {
                                     result1_start=CCR1;//record the initial value
                                     index=0;
                                     break;
                                }
                         else
                           {
                        	              //intertupt at the negative edge
                                          result1_end=CCR1;
                                    if(result1_end>result1_start)
                                    	temp=result1_end-result1_start;
                                    else
                                        temp=result1_end+index*65535-result1_start;
                                    distance=temp*0.0173862/16;
                                    distance=10*distance;
                                    //There is a echo-wave interference at a distance of 700mm which resembles a phinomenon like "reset to zero"
                                    //So I seperate the distance into two parts

                                    //This is the part that rangs from 0 to 699mm.If sonic velocity adjusted properly,the result would be very accurate with a ignorable deviation of 2-3mm
                                    if(distance<700)
                                    	c0(distance);//specific display fuction
                                    else
                                    {
                                    //This is the part that ranges beyond 700mm.For the interference's sake,I "pick up" a useful datum among interfered ones.
                                    //Those interfered ones varies below 700mm,accurately speaking,closing to approximately 20mm and they are far more than the availible ones.
                                    //So I raise the scan frequency if there's a distance datum beyond 699mm,which could help me skip the interfered data more quikly.
                                    //My solution is presented at Mark I above.

                                    //But I can ensure the useful data is accurate.
                                    //Thus I set a counter cd and a variant td.
                                    //I use td to record the second latest datum.
                                    //If the absolute value of td minus the latest datum varies below 8mm,1 is added to cd.When cd beyond 3,we ensure that the datum we get is relatively relible.
                                    	if(cd==0)
                                    		td=distance;
                                    	if(td-distance<8||distance-td<8)
                                    	{
                                    		cd++;
                                    		td=distance;
                                    	}
                                    	else if(td-distance>30||distance-td>30)
                                       		cd=0;
                                    	if(cd>4)
                                    	{
                                    		distance+=5;
                                    		c0(distance);//specific display fuction
                                    		_delay_cycles(100000000);
                                    	}
                                    }

                                    index=0;         //reset the timer
                                    break;
                           }
                 case 4: break;
                 case 10:
                	 index++;  //timer with a fixed range
                	 break;
                 default :break;
                }
}
