#include <main.h>

#include <flex_lcd.c>
#include <ds1307.c>

//Buttons
#bit RB0=0x06.0
#bit RB3=0x06.3

//Time and date
int h = 0;
int min = 0;
int seg = 0;
int day = 0;
int mth = 0;
int year = 0;
int dow = 0;

//Alarm time
int ah = 0;
int amin = 0;

//Alarm ON-OFF
int alarm = 0;
//Counter of alarm beeping
int al = 0;
//Mode menu
int mode = 0;

//For cursor blink
int pass = 0;
//What value of time or date is selected to change.
int B0 = 0;

//Button RB0 interrupt
#INT_EXT
void interrupcio(void){
	//Wait to the rebounds of button and rising edge.
	while(RB0){
      delay_ms(10);
	}
	//Stop alarm
	if(al < 10){
		al = 10;
	//Next number of time or data
	}else{
		B0++;
		pass = 1;
	}
}

void main(){

   setup_timer_0(RTCC_INTERNAL|RTCC_DIV_256);
   setup_timer_1(T1_DISABLED);
   setup_timer_2(T2_DISABLED,0,1);
   setup_comparator(NC_NC_NC_NC);
   setup_vref(FALSE);
   enable_interrupts(GLOBAL);
   ENABLE_INTERRUPTS(INT_EXT);
   
   lcd_init();
   ds1307_init();
   ds1307_set_date_time(5,05,14,3,07,27,05);
    
	//Program running.
   while(true){
   
	  //If button RB0 is pressed we enter in the menu
      if(B0 != 0){
         lcd_putc("\f*  Time Mode\n   Alarm Mode");
		 //Menu to select time or alarm (var. mode)
         while(B0 == 1){
            if(RB3 == 1){
               while(RB3){
                  delay_ms(10);
               }
               mode = !mode;
               if(mode == 1){
                  lcd_putc("\f   Time Mode\n*  Alarm Mode");
               }else{
                  lcd_putc("\f*  Time Mode\n   Alarm Mode");
               }
            }
         }
		 //time mode
         if(mode == 0){
            while(B0 > 1){ 
			   //If RB0 is pressed, shift cursor.	
               if(pass == 1){
                  pass = 0;
                  printf(lcd_putc,"\f Time: %02D:%02D:%02D\n Date: %02D/%02D/%02D ",h,min,seg,day,mth,year);
                  if(B0 == 2){
                     lcd_gotoxy(15,1);
                     lcd_setcursor_vb(true, true);
                  }else if(B0 == 3){
                     lcd_gotoxy(12,1);
                     lcd_setcursor_vb(true, true);
                  }else if(B0 == 4){
                     lcd_gotoxy(9,1);
                     lcd_setcursor_vb(true, true);
                  }else if(B0 == 5){
                     lcd_gotoxy(9,0);
                     lcd_setcursor_vb(true, true);
                  }else if(B0 == 6){
                     lcd_gotoxy(12,0);
                     lcd_setcursor_vb(true, true);
                  }else if(B0 == 7){
                     lcd_gotoxy(15,0);
                     lcd_setcursor_vb(true, true);
                  }
               }
			   //Cursor in seconds position
               if(B0 == 2){
                  if(RB3 == 1){
                     while(RB3){
                        delay_ms(10);
                     }
                     seg = 0;
                     pass = 1;
                  } 
			   //Cursor in minutes position
               }else if(B0 == 3){
                  if(RB3 == 1){
                     while(RB3){
                        delay_ms(10);
                     }
                     min = (min+1)%60;
                     pass = 1;
                  } 
			   //Cursor in hours position
               }else if(B0 == 4){
                  if(RB3 == 1){
                     while(RB3){
                        delay_ms(10);
                     }
                     h = (h+1)%24;
                     pass = 1;
                  } 
			   //Cursor in days position	
               }else if(B0 == 5){
                  if(RB3 == 1){
                     while(RB3){
                        delay_ms(10);
                     }
                     day = (day+1)%31;
                     pass = 1;
                  } 
			   //Cursor in months position
               }else if(B0 == 6){
                  if(RB3 == 1){
                     while(RB3){
                        delay_ms(10);
                     }
                     mth = (mth+1)%12;
                     pass = 1;
                  } 
			   //Cursor in years position
               }else if(B0 == 7){
                  if(RB3 == 1){
                     while(RB3){
                        delay_ms(10);
                     }
                     year = (year+1)%20;
                     pass = 1;
                  }
			   //End of time mode. Set new time to RTC.
               }else{
                  B0 = 0;
                  ds1307_set_date_time(day,mth,year,dow,h,min,seg);
                  lcd_setcursor_vb(false, false);
               }                
            }
         //Alarm mode                    
         }else{
            while(B0 > 1){
			   //If RB0 is pressed, shift cursor.	
               if(pass == 1){
                  pass = 0;
                  if(alarm){
                     printf(lcd_putc,"\f Alarm: %02D:%02D\n       ON",ah,amin);
                  }else{
                     printf(lcd_putc,"\f Alarm: %02D:%02D\n       OFF",ah,amin);
                  }
                  if(B0 == 2){
                     lcd_gotoxy(13,1);
                     lcd_setcursor_vb(true, true);
                  }else if(B0 == 3){
                     lcd_gotoxy(10,1);
                     lcd_setcursor_vb(true, true);
                  }else if(B0 == 4){
                     lcd_gotoxy(8,0);
                     lcd_setcursor_vb(true, false);
                  }
               }
               //Cursor in minutes position
               if(B0 == 2){
                  if(RB3 == 1){
                     while(RB3){
                        delay_ms(10);
                     }
                     amin = (amin+1)%60;
                     pass = 1;
                  }
			   //Cursor in hours position
               }else if(B0 == 3){
                  if(RB3 == 1){
                     while(RB3){
                        delay_ms(10);
                     }
                     ah = (ah+1)%24;
                     pass = 1;
                  } 
			   //Cursor in ON-OFF position
               }else if(B0 == 4){
                  if(RB3 == 1){
                     while(RB3){
                        delay_ms(10);
                     }
                     alarm = !alarm;
                     pass = 1;
                  } 
			   //End of alarm mode. 
               }else{
                  B0 = 0;
                  lcd_setcursor_vb(false, false);
               }
            }
         }    
         mode = 0;
		 
	  //If button RB0 was not pressed we enter in the normal mode of clock.
      }else{
	  
         //Refresh time and data
         ds1307_get_time(h,min,seg); 
         ds1307_get_date(day,mth,year,dow);
         printf(lcd_putc,"\f Time: %02D:%02D:%02D\n Date: %02D/%02D/%02D ",h,min,seg,day,mth,year);
         delay_ms(1000); 
		 
		 //Alarm
         if(h == ah && min == amin && seg == 0 && alarm){
            al = 0;
            while(al < 10){
               OUTPUT_HIGH(PIN_A3);
               delay_ms(500);
               OUTPUT_LOW(PIN_A3);
               delay_ms(500);
               al++;
            }
         }

      }
   }
}
