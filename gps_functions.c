#include "gps_variables.h"
#include "recursos.h"
#include "built_in.h"

unsigned int TFT_DataPort at GPIOE_ODR;
sbit TFT_RST at GPIOE_ODR.B8;
sbit TFT_RS at GPIOE_ODR.B12;
sbit TFT_CS at GPIOE_ODR.B15;
sbit TFT_RD at GPIOE_ODR.B10;
sbit TFT_WR at GPIOE_ODR.B11;
sbit TFT_BLED at GPIOE_ODR.B9;

char datos[80];
char comp[12];
char mx_char[12], my_char[12];
char comp1[]= "$GPRMC";
int flag1=0;
char save_bled;

float y_min=0.64268, y_max=0.71279, x_min=0.73288, x_max=0.84120;
float tiempof=0.0,latitud_f=0.0, longitud_f=0.0;
int temp2=0;
char *res;

char tiempo[12],tiempotx2[12], latitud[12], longitud[12], velocidad[12];
char ant_tiempo[12], ant_latitud[12], ant_longitud[12], ant_velocidad[12];
int separador[15];
int movilx=0, movily=0, ant_movilx, ant_movily;

void initial_setup(){
     save_bled = TFT_BLED;
     TFT_BLED           = 0;
     TFT_INIT_ILI9341_8BIT(320,240);    //inicia la TFT
     TFT_FILL_SCREEN(CL_WHITE);//llenar la pantalla de color blanco
     TFT_BLED           = save_bled;
     //Uart1_init(9600);  // puerto serial a la computadora
     delay_ms(100);
     UART2_Init_Advanced(9600, _UART_8_BIT_DATA, _UART_NOPARITY, _UART_ONE_STOPBIT, &_GPIO_MODULE_USART2_PD56); // puerto serial al sensor
     delay_ms(100);

     TFT_Partial_Image(0, 0, 320, 240,fondo2_bmp, 1);
     
      TFT_Set_Pen(CL_GREEN, 1);
      TFT_Set_Brush(1, CL_GREEN, 0, LEFT_TO_RIGHT, CL_BLACK, CL_WHITE);
     TFT_Rectangle(0, 215, 320, 240);
     tft_write_text("Tiempo= ",1,215);
     tft_write_text("Latitud= ",1,225);
     tft_write_text("Longitud= ",145,215);
     tft_write_text("Velocidad= ",145,225);
     
}

      void borrar_letras(){ 
      if (flag1==1){
       tft_set_font(tft_defaultfont,cl_green,fo_horizontal);
        tft_write_text(ant_tiempo,60,215);
        tft_write_text(ant_latitud,60,225);
        tft_write_text(ant_longitud,220,215);
        tft_write_text(ant_velocidad,220,225);

        }
        }
        
        void escribir_letras(){ 
             if (flag1==1){
        tft_set_font(tft_defaultfont,cl_red,fo_horizontal);
        //tiempof=str2num_f(tiempo);
          //        FloatToStr(tiempof, tiempotx2);
                                /*latitud_f=str2num_f(latitud);
                                  longitud_f=str2num_f(longitud);
                                  longitud_f=longitud_f - floor(longitud_f);
                                  latitud_f=latitud_f - floor(latitud_f);*/
                    /*posicion_xy();
                    FloatToStr(movilx,mx_char);
                    FloatToStr(movily,my_char);*/

           tft_write_text(tiempo,60,215);
           tft_write_text(latitud,60,225);
           tft_write_text(longitud,220,215);
           tft_write_text(velocidad,220,225);
           
           strncpy(ant_tiempo,tiempo,12);
           strncpy(ant_latitud,latitud,12);
           strncpy(ant_longitud,longitud,12);
           strncpy(ant_velocidad,velocidad,12);

           }
}
        
 void  leer_gps(){
       int i=0;
       flag1=0;
       if (UART2_Data_Ready()) {     // si se recive dato, espera hasta el salto de linea
                                   UART2_Read_Text(datos, "\n", 255);    // lee hasta el siguiente salto de linea
                                   }
       strncpy(comp,datos,6);      // guarda los primeros seis valores en una cadena adicional
       if (strcmp(comp,comp1) == 0  )  // compara cadenas para ver si empieza con $GPRMC
                              {
                              for(i=0; i<9; i++)
                                {
                              tiempo[i]=datos[i+7];
                              }
                              for(i=0; i<10; i++)
                                {
                              latitud[i]=datos[i+19];
                              }
                              for(i=0; i<10; i++)
                                {
                              longitud[i]=datos[i+32];
                              }
                              for(i=0; i<5; i++)
                                {
                              velocidad[i]=datos[i+46];
                              }
                              flag1=1;
                              }
   }

   
   float str2num_f( char *string_in){
       int n_enteros;
       float resu=0.0;
       int cifra;
       double potencia;
       int i;
              res = memchr(string_in, '.', 10);  // encontrar el punto decimal
              n_enteros=strlen(string_in) - strlen(res); // numeros a la izquierda del punto

              for(i=1; i < n_enteros + 1; i++){     // conteo de unidades,a la izquierda del punto
                       cifra =  ( string_in[n_enteros-i] - 48);
                       potencia = (pow(10.,(double)(i-1)));
                       resu = resu + cifra*potencia;
                       }
              resu=ceil(resu);

              for(i= n_enteros + 1; i <= strlen(string_in); i++){ // conteo de unidades a la derecha
                     cifra =  ( string_in[i-1] - 48);
                     potencia = (pow(10.,(double)(i - n_enteros - 1)));
                     resu = resu + cifra/potencia;
                     }
      return resu;
   }
void posicion_xy(){
                   ant_movilx=movilx;
                   ant_movily=movily;
       
       latitud_f=str2num_f(latitud);
       longitud_f=str2num_f(longitud);
       
       longitud_f=longitud_f - floor(longitud_f);
       latitud_f=latitud_f - floor(latitud_f);   // sacar decimales
       
        movilx=(-1)*(int)((longitud_f-x_min)*(300.0)/(x_max-x_min) +  75);
        
        if (movilx < 0){
        movilx=0;
        }
        if (movilx > 300){
        movilx=300;
        }
        
        movily=(-1)*(int)(200 - ((latitud_f-y_min)*(200.0)/(y_max-y_min)) + 50);
        
        if (movily < 0){
        movily=0;
        }
        if (movily>200){
        movily=200;
        }
       //codigo aqui//
}
   
   void runner(){
                  if (flag1 ==1) {
                  
                TFT_Partial_Image(0, 0, 320, 210,fondo2_bmp, 1);

                //TFT_Image(movily, movilx,dino5_bmp, 1);
                if (movilx == ant_movilx && movily == ant_movily)
                   TFT_Image(movily, movilx,dino5_bmp, 1);
                
               if ( movilx > ant_movilx )
                   TFT_Image(movily, movilx,dino4_bmp, 1);

                if ( movilx < ant_movilx )
                   TFT_Image(movily, movilx,dino5_bmp, 1);
                   
                if ( movily > ant_movily )
                   TFT_Image(movily, movilx,dino3_bmp, 1);

                if ( movily < ant_movily )
                   TFT_Image(movily, movilx,dino2_bmp, 1);
                // mas codigo aqui//*/
                
                }
   }
   
   int comp_str(){
                if(flag1==1)
                return 1;
                else
                return 0;
                }
                
 // funciones para puerto serial "just in case"
                /*void escribir_serial(){
                              if (flag1 ==1) {
                                UART1_Write_Text("Tiempo : ");
                                  UART1_Write_Text(tiempo);
                                  UART1_Write('\n');
                                  UART1_Write('\r');

                                  UART1_Write_Text("Latitud : ");
                                  UART1_Write_Text(latitud);
                                  UART1_Write('\n');
                                  UART1_Write('\r');

                                  UART1_Write_Text("Longitud : ");
                                  UART1_Write_Text(longitud);
                                  UART1_Write('\n');
                                  UART1_Write('\r');

                                  UART1_Write_Text("velocidad : ");
                                  UART1_Write_Text(velocidad);
                                  UART1_Write('\n');
                                  UART1_Write('\r');

                                  UART1_Write('\n');
                                  UART1_Write('\r');
                                  }

   }

     void escribir_datos_en(){
                            if (flag1 ==1) {
                                   UART1_Write_Text(datos);
                                  UART1_Write('\n');
                                  UART1_Write('\r');
                                  }
                                  }   */