#include <main.h>

#include <flex_lcd.c>
#include <ds1307.c>

#bit RB0=0x06.0
#bit RB3=0x06.3

int h = 0;
int min = 0;
int seg = 0;
int day = 0;
int mth = 0;
int year = 0;
int dow = 0;

int sync = 0;
int mode = 0;

int pass = 0;
int B0 = 0;

#INT_EXT
void interrupcio(void){
   while(RB0){
      delay_ms(10);
   }
 
   B0++;
   pass = 1;
   
}

void main(){

   SET_TRIS_A(0b00000000);
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
    
   
   
   while(true){
      if(B0 != 0){
         lcd_putc("\f*  Time Mode\n   Upgrade");
         while(B0 == 1){
            if(RB3 == 1){
               while(RB3){
                  delay_ms(10);
               }
               mode = !mode;
               if(mode == 1){
                  lcd_putc("\f   Time Mode\n*  Upgrade");
               }else{
                  lcd_putc("\f*  Time Mode\n   Upgrade");
               }
            }
            
         }
         if(mode == 0){
            while(B0 > 1){ 
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
               if(B0 == 2){
                  if(RB3 == 1){
                     while(RB3){
                        delay_ms(10);
                     }
                     seg = 0;
                     pass = 1;
                  } 
               }else if(B0 == 3){
                  if(RB3 == 1){
                     while(RB3){
                        delay_ms(10);
                     }
                     min = (min+1)%60;
                     pass = 1;
                  } 
               }else if(B0 == 4){
                  if(RB3 == 1){
                     while(RB3){
                        delay_ms(10);
                     }
                     h = (h+1)%24;
                     pass = 1;
                  } 
               }else if(B0 == 5){
                  if(RB3 == 1){
                     while(RB3){
                        delay_ms(10);
                     }
                     day = (day+1)%31;
                     pass = 1;
                  } 
               }else if(B0 == 6){
                  if(RB3 == 1){
                     while(RB3){
                        delay_ms(10);
                     }
                     mth = (mth+1)%12;
                     pass = 1;
                  } 
               }else if(B0 == 7){
                  if(RB3 == 1){
                     while(RB3){
                        delay_ms(10);
                     }
                     year = (year+1)%20;
                     pass = 1;
                  } 
               }else{
                  B0 = 0;
                  ds1307_set_date_time(day,mth,year,dow,h,min,seg);
                  lcd_setcursor_vb(false, false);
               }
                  
               
            }
                              
         }else{
            while(B0 > 1){
               if(pass == 1){
                  pass = 0;
                  if(sync){
                     lcd_putc("\f       ON");
                     OUTPUT_HIGH(PIN_A3);
                  }else{
                     lcd_putc("\f       OFF");
                     OUTPUT_LOW(PIN_A3);
                  }
                  if(B0 == 2){
                     lcd_gotoxy(13,1);
                     lcd_setcursor_vb(true, true);
                  }
               }
               
               if(B0 == 2){
                  if(RB3 == 1){
                     while(RB3){
                        delay_ms(10);
                     }
                     sync = !sync;
                     pass = 1;
                  }   
               }else{
                  B0 = 0;
                  lcd_setcursor_vb(false, false);
               }
            }
         }    
         mode = 0;
      }else{
         
         ds1307_get_time(h,min,seg); 
         ds1307_get_date(day,mth,year,dow);
         printf(lcd_putc,"\f Time: %02D:%02D:%02D\n Date: %02D/%02D/%02D ",h,min,seg,day,mth,year);
         delay_ms(1000); 
         if(h == 12 && min == 30 && seg == 0){
            OUTPUT_HIGH(PIN_A3);
            delay_ms(10);
            ds1307_get_time(h,min,seg); 
            ds1307_get_date(day,mth,year,dow);
            OUTPUT_LOW(PIN_A3);
            delay_ms(10);
            ds1307_set_date_time(day,mth,year,dow,h,min,seg);
         }
         delay_ms(1000);
      }
   }
}
