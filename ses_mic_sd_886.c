#include <16f886.h>
#device adc= 8
#fuses  hs,NOWDT,NOPUT,MCLR,NOPROTECT,NOCPD,NOBROWNOUT,NOIESO,NOFCMEN,NOLVP,NODEBUG,BORV40, NOWRT
#use delay(clock=20000000) //because of tricky fuses a 20Mhz oscillator will make the PIC run at 48Mhz
//!#include <PCF8574A_LCD_SEND.c>
#include <stdlib.h>
#include <math.h>
//****************************************************************************************************
#define MP3_TX_PIN PIN_B0
//!#define MP3_RX_PIN PIN_B1
#use rs232 (baud=9600, xmit=MP3_TX_PIN, parity=N, stop=1,stream=DFPLAYER,errors)//
//***************************************************************************************************
#define TX_PIN PIN_C6  
#define RX_PIN PIN_C7  
#use rs232 (baud=9600, xmit=pin_C6, rcv=pin_C7, parity=N, stop=1, bits=8,errors,stream=RS232,timeout=5)//,timeout=35,,stream=RS232
//********************************************************************************************************
unsigned int8 a0=0 ;
unsigned int8  say=0 , say1=0, say2=0, say3=0, say4=0 , cevir=0;
unsigned int8 i=0,bilgi=0;
unsigned int32 topla=0;
signed int32 temperature;
unsigned int32 pressure ,pressure1;
char data[6] ;

 #int_rda 
void serihaberlesme_kesmesi ()
{
       disable_interrupts(int_rda);
       fgets(data,RS232);
       data[0]='0';
       cevir=atoi(data);
       fprintf(RS232,"cevir%3u\r\n",cevir);
       enable_interrupts(int_rda);
}

void aoku() {   a0=read_adc();  }

unsigned char comandoDFplayer[10] = { 0x7E,0xFF,0x06,0x03,0x00,0x00,0x02,0xFE,0xF6,0xEF };
 unsigned int8 a=0;
void write_Comand(unsigned char *);
void seccal(unsigned char);
void pause(void);
void play(void);
void sonraki(void);
void onceki(void);
void volume(unsigned char);
void sesa(void);
void sese(void);
void eq(unsigned char);


void seccal(int pist){
   comandoDFplayer[3] = (unsigned char)0x03 ;
   comandoDFplayer[5] = (unsigned char)0x00 ;
   comandoDFplayer[6] = (unsigned char)pist ;
   write_Comand(comandoDFplayer);
}

void pause(void){
   comandoDFplayer[3] = (unsigned char)0x0E ;
   comandoDFplayer[5] = (unsigned char)0x00 ;
   comandoDFplayer[6] = (unsigned char)0x00 ;
   write_Comand(comandoDFplayer);
}

void play(void){
   comandoDFplayer[3] = (unsigned char)0x0D ;
   comandoDFplayer[5] = (unsigned char)0x00 ;
   comandoDFplayer[6] = (unsigned char)0x00 ;
   write_Comand(comandoDFplayer);
}

void sonraki(void){
   comandoDFplayer[3] = (unsigned char)0x01 ;
   comandoDFplayer[5] = (unsigned char)0x00 ;
   comandoDFplayer[6] = (unsigned char)0x00 ;
   write_Comand(comandoDFplayer);
}

void onceki(void){
   comandoDFplayer[3] = (unsigned char)0x02 ;
   comandoDFplayer[5] = (unsigned char)0x00 ;
   comandoDFplayer[6] = (unsigned char)0x00 ;
   write_Comand(comandoDFplayer);
}
void volume(unsigned char volumen){
   comandoDFplayer[3] = (unsigned char)0x06 ;
   comandoDFplayer[5] = (unsigned char)0x00 ;
   comandoDFplayer[6] = (char) volumen ;
   write_Comand(comandoDFplayer);
}
void sesa(void){
   comandoDFplayer[3] = (unsigned char)0x04 ;
   comandoDFplayer[5] = (unsigned char)0x00 ;
   comandoDFplayer[6] = (unsigned char)0x00 ;
   write_Comand(comandoDFplayer);
}

void sese(void){
   comandoDFplayer[3] = (unsigned char)0x05 ;
   comandoDFplayer[5] = (unsigned char)0x00 ;
   comandoDFplayer[6] = (unsigned char)0x00 ;
   write_Comand(comandoDFplayer);
}

void eq(int eqo){
   comandoDFplayer[3] = (unsigned char)0x07 ;
   comandoDFplayer[5] = (unsigned char)0x00 ;
   comandoDFplayer[6] = (unsigned char)eqo ;
   write_Comand(comandoDFplayer);
}

void write_Comand(unsigned char *cmd){
   volatile static unsigned int16 checksum = 0;
   volatile static unsigned char i = 0;
   checksum = 0;
   for(i=1; i<7; i++){
      checksum += cmd[i];
   }
   checksum = 0xFFFF - checksum + 1;
   cmd[7] = (unsigned char)  (checksum >> 8);
   cmd[8] = (unsigned char)  checksum ;
   for(i = 0; i <10; i++) { //envia cmd      
         fputc(cmd[i], DFPLAYER);
   }
}
void main()
{
   delay_ms(1000);  // wait 1 second
   setup_timer_1(T1_DISABLED);
   setup_timer_2(T2_DISABLED,0,1);
   setup_adc_ports(sAN0);
   setup_adc(adc_clock_div_32);   // ADC clock frekansý fosc/32
   
   setup_CCP2(CCP_OFF);
   setup_wdt(WDT_OFF);
  // setup_comparator(NC_NC_NC_NC);
   enable_interrupts(int_rda);
   enable_interrupts(GLOBAL);
   set_adc_channel(0);delay_us(20);
   output_drive(pin_c6);
   output_float(pin_c7);
   set_tris_b (0b11111110);
//!   lcd_init (Wlcd7) ;lcd_backlight (1, Wlcd7) ;
//!   printf("hazir\r\n");
int i , j ;
//! while(j<25) {j++;sesa();}

   volume(29);
    while(TRUE)
    {
    aoku();
    if(a0>100){ output_high(pin_c0); play(); delay_ms(2000);} output_low(pin_c0);
//!    int i , j ;

//!        lcd_gotoxy (1, 1, Wlcd7); printf(lcd_putc,"%u %u ",i,j); 

//!    if( cevir==1) play(); 
//!    if( cevir==2) pause(); 
//!    if( cevir==3){  sonraki(); }
//!    if( cevir==4){  onceki(); }
//!    if( cevir==5){ a++;if(a>9) a=1; seccal(a); }
//!    if( cevir==6){ j++;  sesa();  }
//!    if( cevir==7){ j--;  sese();  }
//!    if( cevir==8){ i=1; eq(i); }
//!    if( cevir==9){ i=5; eq(i); }
//!    cevir=0;
    }}
 




