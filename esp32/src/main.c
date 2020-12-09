#include "defines.h"
#include "includes.h"

/* Aplicação Principal (Inicia após bootloader) */
void app_main(void)
{
    inicia_wifi();
    inicia_sensor();
    inicia_oled();
    envia_dados();
}
