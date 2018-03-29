#include "C:\Users\melvi_000\Documents\Pic Compiler\reloj\main.h"

#bit RA1 = 0x05.1
#bit RA2 = 0x05.2
#bit RA3 = 0x05.3
#bit RA4 = 0x05.4
#bit RA5 = 0x05.5

#include <lcd.c>

int count = 0;
int h = 0;
int min = 0;
int seg = 0;
int ah = 0;
int amin = 0;
int al = 0;
int alarm = 0;
int p = 0;
int mode = 0;

#int_TIMER0
void  TIMER0_isr(void) 
{
 count++;
   if (count == 175){
      if(alarm == 2){
         printf(lcd_putc,"\f Time: %02D:%02D:%02D\n It's time",h,min,seg);
         OUTPUT_HIGH(PIN_A1);
      }else if(mode == 1){
         printf(lcd_putc,"\f Time: %02D:%02D:  \n Alarm at: %02D:%02D",h,min,ah,amin);  
      }else if (mode == 2){
         printf(lcd_putc,"\f Time: %02D:  :%02D\n Alarm at: %02D:%02D",h,seg,ah,amin);
      }else if(mode == 3){
         printf(lcd_putc,"\f Time:   :%02D:%02D\n Alarm at: %02D:%02D",min,seg,ah,amin);
      }else if (al == 1 && (alarm == 1 || alarm == 2)){
         printf(lcd_putc,"\f Alarm: %02D:  \n       ON",ah);  
      }else if (al == 2 && (alarm == 1 || alarm == 2)){
         printf(lcd_putc,"\f Alarm:   :%02D\n       ON",amin);
      }else if (al == 1 && alarm == 0){
         printf(lcd_putc,"\f Alarm: %02D:  \n       OFF",ah);  
      }else if (al == 2 && alarm == 0){
         printf(lcd_putc,"\f Alarm:   :%02D\n       OFF",amin);
      }else if (al == 3){
         printf(lcd_putc,"\f Alarm: %02D:%02D\n",ah, amin);
      }
   }
   if (count == 250){
      count = 0;
      if(seg == 60){
         seg = 0;
         min++;
         if(alarm == 2){
            alarm = 1;
         }
         if(min == 60){
            min = 0;
            h++;
            if(h == 24){
            h = 0;
            }
         }
      }
      if(alarm == 2){
         printf(lcd_putc,"\f Time: %02D:%02D:%02D\n",h,min,seg);
         OUTPUT_LOW(PIN_A1);
      }else if(al == 0){
         printf(lcd_putc,"\f Time: %02D:%02D:%02D\n Alarm at: %02D:%02D",h,min,seg,ah,amin);
      }else if (alarm == 0){
         printf(lcd_putc,"\f Alarm: %02D:%02D\n       OFF",ah,amin);
      }else{
         printf(lcd_putc,"\f Alarm: %02D:%02D\n       ON",ah,amin);
      }
      if(h == ah && min == amin && alarm == 1 && seg == 0){
         alarm = 2;
      }
      seg++;
      set_TIMER0(0x06); //inicializa el timer0
   }
}


void main()
{
   lcd_init();
   SET_TRIS_A(0b00111100);
   SET_TRIS_B(0b01000000);


   setup_timer_0(RTCC_INTERNAL|RTCC_DIV_16);
   setup_timer_1(T1_DISABLED);
   setup_timer_2(T2_DISABLED,0,1);
   setup_comparator(NC_NC_NC_NC);
   setup_vref(FALSE);
   enable_interrupts(INT_TIMER0);
   enable_interrupts(GLOBAL);
   set_TIMER0(0x06);

   while(true){
      if(RA5 == 1){
      if (alarm == 2){
            alarm = 1;
            OUTPUT_LOW(PIN_A1);
            p++;
         }
         while(RA5 == 1){
         } 
         if (p != 0){
            p = 0;
         }else{
            mode++;
            al = 0;
            if(mode == 4){
               mode = 0;
            }
         } 
      }
      
      if(RA4 == 1){
         if (alarm == 2){
            alarm = 1;
            OUTPUT_LOW(PIN_A1);
            p++;
         }   
         while(RA4 == 1){
         }  
         
         if (p != 0){
            p = 0;
         }else{
            al++;
            mode = 0;
            if(al == 4){
               al = 0;
            } 
         }  
       }
      
      if(RA3 == 1){
      if (alarm == 2){
            alarm = 1;
            OUTPUT_LOW(PIN_A1);
            p++;
         }
         while(RA3 == 1){
         }  
         if (p != 0){
            p = 0;
         }else{
            if(mode == 1){
               seg = 0;
            }else if (mode == 2){
             min++;
             if(min == 60){
                 min = 0;
             }  
            }else if (mode == 3){
               h++;
               if(h == 24){
                 h = 0;
               }
            }
            if(al == 1){
               amin++;
               if(amin == 60){
                  amin = 0;
               }
            }else if (al == 2){
               ah++;
               if(ah == 24){
                  ah = 0;
               }  
            }else if(al == 3){
               if(alarm == 0){
                  alarm = 1;
               }else{
                  alarm = 0;
               }  
            }
         }
      }
      
      if(RA2 == 1){
      if (alarm == 2){
            alarm = 1;
            OUTPUT_LOW(PIN_A1);
            p++;
         }
         while(RA2 == 1){
         }  
         if (p != 0){
            p = 0;
         }else{
            if(mode == 1){
               seg = 0;
            }else if (mode == 2){
               if(min == 0){
                  min = 59;
               }else{
                  min--;
               }
            }else if (mode == 3){
               if(h == 0){
                  h = 23;
               }else{
                  h--;
               }
            }
            if(al == 1){
               if(amin == 0){
                  amin = 59;
               }else{
                  amin--;
               }
            }else if (al == 2){
               if(ah == 0){
                  ah = 23;
               }else{
                  ah--;
               }
            }else if(al == 3){
               if(alarm == 0){
                  alarm = 1;
               }else{
                  alarm = 0;
               }  
            }
         }
      }
      
         
    }
         
       

}
