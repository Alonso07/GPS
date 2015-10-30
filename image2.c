#include "recursos.h"
#include "gps_variables.h"



void main() {

int n=0;
/*int x=37;
int y=170;*/

     initial_setup();

  while(1) {

           leer_gps();
            if (n!=0) {
            borrar_letras();
                            }
          /*if (x< 283 && comp_str()){
            x ++;
            }*/
            //escribir_serial();
            // escribir_datos_en();
            posicion_xy();
            runner();
            escribir_letras();
           n=1;
}
               //delay_ms(1000);
}