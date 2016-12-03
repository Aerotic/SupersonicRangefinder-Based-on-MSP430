/*
 * display1.h
 *This capetible for digitaltubes with TM1637 inside
 *Or those with I2C ports
 *  Created on: Nov.20th 2016
 *      Author: Aeroman
 */

#ifndef DISPLAY1_H_
#define DISPLAY1_H_
extern display(unsigned char data0,unsigned char data1,unsigned char data2,unsigned char data3);
extern bootfig();
extern dis_r(char a,char b,char c,char d);
extern c0(float a);
extern delay(x);
#define dis_erro display(fig[14],fig[16],fig[16],fig[0])
//*********************************************
//characters
//unsigned char fig[] = {0x3f,//0
//			      0x06,//1
//				  0x5b,//2
//  				  0x4f,//3
//				  0x66,//4
//				  0x6d,//5
//				  0x7d,//6
//				   0x07,//7
//        		   0x7f,//8
//        		   0x6f,//9
//        		   0x77,//A
//        		   0x7c,//b
//        		   0x39,//C
//		   	   	   0x5e,//d
//		   	   	   0x79,//E
//		   	   	   0x71,//F
//		   	   	   0x31,//R
//		   	   	   0x00//ledoff
//		   	   	   };
//unsigned char b_fig[][4]={{0x01,0x01,0x01,0x01},
//						  {0x23,0x21,0x21,0x23},
//						  {0x77,0x79,0x31,0x3f},};
//***********************************************

#endif /* DISPLAY1_H_ */
