/**
 * Nombre proyecto:	LAVADORA
 * Integrantes:         Angel Francisco Sánchez, Roberto Guzmán
 * URL proyecto:
 * Fecha:    		10/03/2015
 * Asignatura:		SEyTR
 */

// Librerias:
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

int red=5; //Led RGB rojo
int green=6; //led RGB verde
int blue=7; //led RGB azul

int buzzer=13; //buzzer

// Caracteres personalizados para el LCD
byte arrowDown[8]	= {B00000, B01110, B01110, B01110, B11111, B01110, B00100};	
byte cursorLCD[8]	= {B00000, B00000, B00110, B01111, B01111, B00110, B00000};
byte white[8]		= {B11111, B11111, B11111, B11111, B11111, B11111, B11111};

int pin2=9;   //Entrada 2 del L293D
int pin7=10;  //Entrada 7 del L293D

void setup()
{
        //Inicializamos los pins de salida
        pinMode(pin2,OUTPUT);
        pinMode(pin7, OUTPUT);
        pinMode(buzzer, OUTPUT);
        // Pantalla de 20 caracteres y 4 líneas:
	lcd.begin(20,4);

	// Se añaden los caracteres especiales al objeto LCD:
	lcd.createChar(0, cursorLCD);
	lcd.createChar(1, arrowDown);
	lcd.createChar(3, white);
        
        pinMode(red, OUTPUT);		// Se configuran los pines de los LEDs.
	pinMode(green, OUTPUT);
        pinMode(blue, OUTPUT);
        analogWrite(red,255);
        analogWrite(green,0);
	analogWrite(blue,255);
        
}



void loop()
{
	LCDInicio();
}

void LCDInicio()
{
	// Guarda el resultado leído de una pulsación por teclado:
        char resultButton[7] = "null";

	// Se imprime la pantalla de inicio:
	lcd.setCursor(7, 0); lcd.print("ARDUINO");
	lcd.setCursor(3, 1); lcd.print("WASHING MACHINE");
	lcd.setCursor(7, 3); lcd.print("Pulse "); lcd.write(byte(1));

	// Se comprueba si se ha pulsado un botón:
	getButtonPush( resultButton, 0, 200);

	// Si se pulsa la tecla abajo, se entra en el menú:
	if( !strcmp(resultButton,"bottom"))
		LCDmenu();
}



// MENU PRINCIPAL, DA OPCION PARA ABRIR EL MENU 1 Y MENU 2, TAMBIEN PARA SALIR A LA PANTALLA PRINCIPAL
void LCDmenu()
{
	char resultButton[7];
	int posCursor=0, opcMenu=-2;


	// Menu
	lcd.clear();
	lcd.setCursor(2, 0); lcd.print("Lavado Rapido");
	lcd.setCursor(2, 1); lcd.print("Lavado Lento");
        lcd.setCursor(2, 2); lcd.print("Prendas Delicadas");
	lcd.setCursor(2, 3); lcd.print("Salir");


	// Mientras no se pulse sobre salir, no se saldrá a la pantalla principal:
	do{
		strcpy(resultButton,"null");
		getButtonPush( resultButton, 0, 200);

		// Cursor que indica la opción seleccionada:
		lcd.setCursor(0, posCursor); lcd.write(byte(0));
                
		// Desplaza el cursor arriba o abajo:
		if(!strcmp(resultButton,"up"))
			LCDMueveCursor(&posCursor, 3, 'r');	// Se resta una posición al cursor

		else if(!strcmp(resultButton,"bottom"))	// Se suma una posición al cursor
			LCDMueveCursor(&posCursor, 3, 's');

		else if(!strcmp(resultButton,"right"))
			opcMenu = posCursor;				// Al pulsar a la derecha (para acceder a esa opcion) se actualiza la opción de menú elegida según donde esta el cursor ahora.
                else if(!strcmp(resultButton,"left")){
			opcMenu = -1;
                        lcd.clear();
                        LCDInicio();
                }

		// Según la opción elegida del menú, se llama a otro menú o se cierra el menú actual:
		switch( opcMenu )
		{
			case 0:				
				LCDmenu0();
				opcMenu = -2;
				break;
			case 1:				
				LCDmenu1();
				opcMenu = -2;
				break;
                        case 2:				
				LCDmenu2();
				opcMenu = -2;
				break;
			case 3:				
                                opcMenu = -1;
				lcd.clear();
                                LCDInicio();
				break;
		};
	}
	while(opcMenu != -1);
        //lcd.clear();
}

//LAVADO RAPIDO
void LCDmenu0()
{
	char resultButton[7];
	int posCursor=2, opcMenu=-2;


	// Menu
	lcd.clear();
	lcd.setCursor(2, 0); lcd.print("El lavado dura ");
	lcd.setCursor(2, 1); lcd.print("59 minutos.");
        lcd.setCursor(2, 2); lcd.print("Iniciar");
	lcd.setCursor(2, 3); lcd.print("Salir");


	// Mientras no se pulse sobre salir, no se saldrá al Menú:
	do{
		strcpy(resultButton,"null");
		getButtonPush(resultButton, 0, 200);

		// Cursor que indica la opcion seleccioanda:
		lcd.setCursor(0, posCursor); lcd.write(byte(0));

		// Desplaza el cursor arriba o abajo,:
		if(!strcmp(resultButton,"up"))
			LCDMueveCursor(&posCursor, 3, 'r');	// Se resta una posición al cursor

		else if(!strcmp(resultButton,"bottom"))	// Se suma una posición al cursor
			LCDMueveCursor(&posCursor, 3, 's');

		else if(!strcmp(resultButton,"right"))
			opcMenu = posCursor;
                else if(!strcmp(resultButton,"left")){
			opcMenu = -1;
                        lcd.clear();
                        LCDmenu();
                }


		// Según la opción elegida del menú, se le cambia el estado a un LED, al otro o se cierra el menú actual:
		switch( opcMenu )
		{
                        case 2:
                                Lavado();
				opcMenu = -2;
			        break;
			case 3:
				opcMenu = -1;
                                lcd.clear();
                                LCDmenu();
				break;
		};
	}
	while(opcMenu != -1);

	// Al salir del Menu1 se imprime el menú anterior:
	//lcd.clear();
}

void Lavado()
{

        analogWrite(red,0);
        analogWrite(green,255);
	analogWrite(blue,255);
        int opcMenu=-2;      
      
	// Menu
	lcd.clear();
        lcd.setCursor(6, 1); lcd.print("PRELAVADO");
        analogWrite(pin2,155);
        for(int i=59; i>=55; i--){
           lcd.setCursor(8, 3); lcd.print("00:");lcd.print(i);
           delay(1000);
        }
        analogWrite(pin2, 0);
        lcd.clear();
        lcd.setCursor(6, 1); lcd.print("PRELAVADO");
        analogWrite(pin7,155);
        for(int i=54; i>=50; i--){
            lcd.setCursor(8, 3); lcd.print("00:");lcd.print(i);
           delay(1000);
        }
        analogWrite(pin7, 0);
        lcd.clear();
        lcd.setCursor(8, 1); lcd.print("LAVADO");
        analogWrite(pin2,120);
        for(int i=49; i>=40; i--){
           lcd.setCursor(8, 3); lcd.print("00:");lcd.print(i);
           delay(1000);
        }
        analogWrite(pin2, 0);
        lcd.clear();
        lcd.setCursor(8, 1); lcd.print("LAVADO");
        analogWrite(pin7,120);
        for(int i=39; i>=30; i--){
            lcd.setCursor(8, 3); lcd.print("00:");lcd.print(i);
           delay(1000);
        }
        analogWrite(pin7, 0);
        lcd.clear();
        
        lcd.setCursor(6, 1); lcd.print("ACLARADO");
        analogWrite(pin2,185);
        for(int i=29; i>=25; i--){
           lcd.setCursor(8, 3); lcd.print("00:");lcd.print(i);
           delay(1000);
        }
        analogWrite(pin2, 0);
        lcd.clear();
        lcd.setCursor(6, 1); lcd.print("ACLARADO");
        analogWrite(pin7,185);
        for(int i=24; i>=20; i--){
            lcd.setCursor(8, 3); lcd.print("00:");lcd.print(i);
           delay(1000);
        }
        analogWrite(pin7, 0);
        
        lcd.setCursor(4, 1); lcd.print("CENTRIFUGADO");
        analogWrite(pin2,255);
        for(int i=19; i>=10; i--){
           lcd.setCursor(8, 3); lcd.print("00:");lcd.print(i);
           delay(1000);
        }
        analogWrite(pin2, 0);
        lcd.clear();
        lcd.setCursor(4, 1); lcd.print("CENTRIFUGADO");
        analogWrite(pin7,255);
        for(int i=9; i>=0; i--){
            lcd.setCursor(8, 3); lcd.print("00:0");lcd.print(i);
           delay(1000);
        }
        analogWrite(pin7, 0);
        lcd.clear();
        lcd.setCursor(2, 2); lcd.print("LAVADO FINALIZADO");
        analogWrite(buzzer, 255);
        delay(500);
        lcd.setCursor(2, 2); lcd.print("LAVADO FINALIZADO");
        analogWrite(buzzer, LOW);
        delay(500);
        lcd.setCursor(2, 2); lcd.print("LAVADO FINALIZADO");
        analogWrite(buzzer, 255);
        delay(500);
        lcd.setCursor(2, 2); lcd.print("LAVADO FINALIZADO");
        analogWrite(buzzer, LOW);
        delay(500);
        lcd.setCursor(2, 2); lcd.print("LAVADO FINALIZADO");
        analogWrite(buzzer, 255);
        delay(500);
        lcd.setCursor(2, 2); lcd.print("LAVADO FINALIZADO");
        analogWrite(buzzer, LOW);
        delay(500);
        lcd.setCursor(2, 2); lcd.print("LAVADO FINALIZADO");
        analogWrite(buzzer, 255);
        delay(500);
        lcd.setCursor(2, 2); lcd.print("LAVADO FINALIZADO");
        analogWrite(buzzer, LOW);
        delay(500);
        analogWrite(red,255);
        analogWrite(green,0);
	analogWrite(blue,255);
        LCDmenu(); 
}

//LAVADO Lento
void LCDmenu1()
{
	char resultButton[7];
	int posCursor=2, opcMenu=-2;


	// Menu
	lcd.clear();
	lcd.setCursor(2, 0); lcd.print("El lavado dura ");
	lcd.setCursor(2, 1); lcd.print("89 minutos.");
        lcd.setCursor(2, 2); lcd.print("Iniciar");
	lcd.setCursor(2, 3); lcd.print("Salir");


	// Mientras no se pulse sobre salir, no se saldrá al Menú:
	do{
		strcpy(resultButton,"null");
		getButtonPush(resultButton, 0, 200);

		// Cursor que indica la opcion seleccioanda:
		lcd.setCursor(0, posCursor); lcd.write(byte(0));

		// Desplaza el cursor arriba o abajo,:
		if(!strcmp(resultButton,"up"))
			LCDMueveCursor(&posCursor, 3, 'r');	// Se resta una posición al cursor

		else if(!strcmp(resultButton,"bottom"))	// Se suma una posición al cursor
			LCDMueveCursor(&posCursor, 3, 's');

		else if(!strcmp(resultButton,"right"))
			opcMenu = posCursor;
                else if(!strcmp(resultButton,"left")){
			opcMenu = -1;
                        lcd.clear();
                        LCDmenu();
                }


		// Según la opción elegida del menú, se le cambia el estado a un LED, al otro o se cierra el menú actual:
		switch( opcMenu )
		{
                        case 2:
                                Lavado1();
				opcMenu = -2;
			        break;
			case 3:
				opcMenu = -1;
                                lcd.clear();
                                LCDmenu();
				break;
		};
	}
	while(opcMenu != -1);

	// Al salir del Menu1 se imprime el menú anterior:
	//lcd.clear();
}

void Lavado1()
{

        analogWrite(red,0);
        analogWrite(green,255);
	analogWrite(blue,255);
        int opcMenu=-2;      
      
	// Menu
	lcd.clear();
        lcd.setCursor(6, 1); lcd.print("PRELAVADO");
        analogWrite(pin2,155);
        for(int i=89; i>=80; i--){
           lcd.setCursor(8, 3); lcd.print("00:");lcd.print(i);
           delay(1000);
        }
        analogWrite(pin2, 0);
        lcd.clear();
        lcd.setCursor(6, 1); lcd.print("PRELAVADO");
        analogWrite(pin7,155);
        for(int i=79; i>=70; i--){
            lcd.setCursor(8, 3); lcd.print("00:");lcd.print(i);
           delay(1000);
        }
        analogWrite(pin7, 0);
        lcd.clear();
        lcd.setCursor(8, 1); lcd.print("LAVADO");
        analogWrite(pin2,120);
        for(int i=69; i>=45; i--){
           lcd.setCursor(8, 3); lcd.print("00:");lcd.print(i);
           delay(1000);
        }
        analogWrite(pin2, 0);
        lcd.clear();
        lcd.setCursor(8, 1); lcd.print("LAVADO");
        analogWrite(pin7,120);
        for(int i=44; i>=35; i--){
            lcd.setCursor(8, 3); lcd.print("00:");lcd.print(i);
           delay(1000);
        }
        analogWrite(pin7, 0);
        lcd.clear();
        
        lcd.setCursor(6, 1); lcd.print("ACLARADO");
        analogWrite(pin2,185);
        for(int i=34; i>=28; i--){
           lcd.setCursor(8, 3); lcd.print("00:");lcd.print(i);
           delay(1000);
        }
        analogWrite(pin2, 0);
        lcd.clear();
        lcd.setCursor(6, 1); lcd.print("ACLARADO");
        analogWrite(pin7,185);
        for(int i=27; i>=20; i--){
            lcd.setCursor(8, 3); lcd.print("00:");lcd.print(i);
           delay(1000);
        }
        analogWrite(pin7, 0);
        
        lcd.setCursor(4, 1); lcd.print("CENTRIFUGADO");
        analogWrite(pin2,255);
        for(int i=19; i>=10; i--){
           lcd.setCursor(8, 3); lcd.print("00:");lcd.print(i);
           delay(1000);
        }
        analogWrite(pin2, 0);
        lcd.clear();
        lcd.setCursor(4, 1); lcd.print("CENTRIFUGADO");
        analogWrite(pin7,255);
        for(int i=9; i>=0; i--){
            lcd.setCursor(8, 3); lcd.print("00:0");lcd.print(i);
           delay(1000);
        }
        analogWrite(pin7, 0);
        lcd.clear();
        lcd.setCursor(2, 2); lcd.print("LAVADO FINALIZADO");
        analogWrite(buzzer, 255);
        delay(500);
        lcd.setCursor(2, 2); lcd.print("LAVADO FINALIZADO");
        analogWrite(buzzer, LOW);
        delay(500);
        lcd.setCursor(2, 2); lcd.print("LAVADO FINALIZADO");
        analogWrite(buzzer, 255);
        delay(500);
        lcd.setCursor(2, 2); lcd.print("LAVADO FINALIZADO");
        analogWrite(buzzer, LOW);
        delay(500);
        lcd.setCursor(2, 2); lcd.print("LAVADO FINALIZADO");
        analogWrite(buzzer, 255);
        delay(500);
        lcd.setCursor(2, 2); lcd.print("LAVADO FINALIZADO");
        analogWrite(buzzer, LOW);
        delay(500);
        lcd.setCursor(2, 2); lcd.print("LAVADO FINALIZADO");
        analogWrite(buzzer, 255);
        delay(500);
        lcd.setCursor(2, 2); lcd.print("LAVADO FINALIZADO");
        analogWrite(buzzer, LOW);
        delay(500);
        analogWrite(red,255);
        analogWrite(green,0);
	analogWrite(blue,255);
        LCDmenu(); 
}

//LAVADO RAPIDO
void LCDmenu2()
{
	char resultButton[7];
	int posCursor=2, opcMenu=-2;


	// Menu
	lcd.clear();
	lcd.setCursor(2, 0); lcd.print("El lavado dura ");
	lcd.setCursor(2, 1); lcd.print("70 minutos.");
        lcd.setCursor(2, 2); lcd.print("Iniciar");
	lcd.setCursor(2, 3); lcd.print("Salir");


	// Mientras no se pulse sobre salir, no se saldrá al Menú:
	do{
		strcpy(resultButton,"null");
		getButtonPush(resultButton, 0, 200);

		// Cursor que indica la opcion seleccioanda:
		lcd.setCursor(0, posCursor); lcd.write(byte(0));

		// Desplaza el cursor arriba o abajo,:
		if(!strcmp(resultButton,"up"))
			LCDMueveCursor(&posCursor, 3, 'r');	// Se resta una posición al cursor

		else if(!strcmp(resultButton,"bottom"))	// Se suma una posición al cursor
			LCDMueveCursor(&posCursor, 3, 's');

		else if(!strcmp(resultButton,"right"))
			opcMenu = posCursor;
                else if(!strcmp(resultButton,"left")){
			opcMenu = -1;
                        lcd.clear();
                        LCDmenu();
                }


		// Según la opción elegida del menú, se le cambia el estado a un LED, al otro o se cierra el menú actual:
		switch( opcMenu )
		{
                        case 2:
                                Lavado2();
				opcMenu = -2;
			        break;
			case 3:
				opcMenu = -1;
                                lcd.clear();
                                LCDmenu();
				break;
		};
	}
	while(opcMenu != -1);

	// Al salir del Menu1 se imprime el menú anterior:
	//lcd.clear();
}

void Lavado2()
{

        analogWrite(red,0);
        analogWrite(green,255);
	analogWrite(blue,255);
        int opcMenu=-2;      
      
	// Menu
	lcd.clear();
        lcd.setCursor(6, 1); lcd.print("PRELAVADO");
        analogWrite(pin2,145);
        for(int i=70; i>=60; i--){
           lcd.setCursor(8, 3); lcd.print("00:");lcd.print(i);
           delay(1000);
        }
        analogWrite(pin2, 0);
        lcd.clear();
        lcd.setCursor(6, 1); lcd.print("PRELAVADO");
        analogWrite(pin7,145);
        for(int i=59; i>=50; i--){
            lcd.setCursor(8, 3); lcd.print("00:");lcd.print(i);
           delay(1000);
        }
        analogWrite(pin7, 0);
        lcd.clear();
        lcd.setCursor(8, 1); lcd.print("LAVADO");
        analogWrite(pin2,110);
        for(int i=49; i>=40; i--){
           lcd.setCursor(8, 3); lcd.print("00:");lcd.print(i);
           delay(1000);
        }
        analogWrite(pin2, 0);
        lcd.clear();
        lcd.setCursor(8, 1); lcd.print("LAVADO");
        analogWrite(pin7,110);
        for(int i=39; i>=30; i--){
            lcd.setCursor(8, 3); lcd.print("00:");lcd.print(i);
           delay(1000);
        }
        analogWrite(pin7, 0);
        lcd.clear();
        
        lcd.setCursor(6, 1); lcd.print("ACLARADO");
        analogWrite(pin2,165);
        for(int i=29; i>=25; i--){
           lcd.setCursor(8, 3); lcd.print("00:");lcd.print(i);
           delay(1000);
        }
        analogWrite(pin2, 0);
        lcd.clear();
        lcd.setCursor(6, 1); lcd.print("ACLARADO");
        analogWrite(pin7,165);
        for(int i=24; i>=20; i--){
            lcd.setCursor(8, 3); lcd.print("00:");lcd.print(i);
           delay(1000);
        }
        analogWrite(pin7, 0);
        
        lcd.setCursor(4, 1); lcd.print("CENTRIFUGADO");
        analogWrite(pin2,205);
        for(int i=19; i>=10; i--){
           lcd.setCursor(8, 3); lcd.print("00:");lcd.print(i);
           delay(1000);
        }
        analogWrite(pin2, 0);
        lcd.clear();
        lcd.setCursor(4, 1); lcd.print("CENTRIFUGADO");
        analogWrite(pin7,205);
        for(int i=9; i>=0; i--){
            lcd.setCursor(8, 3); lcd.print("00:0");lcd.print(i);
           delay(1000);
        }
        analogWrite(pin7, 0);
        lcd.clear();
        lcd.setCursor(2, 2); lcd.print("LAVADO FINALIZADO");
        analogWrite(buzzer, 255);
        delay(500);
        lcd.setCursor(2, 2); lcd.print("LAVADO FINALIZADO");
        analogWrite(buzzer, LOW);
        delay(500);
        lcd.setCursor(2, 2); lcd.print("LAVADO FINALIZADO");
        analogWrite(buzzer, 255);
        delay(500);
        lcd.setCursor(2, 2); lcd.print("LAVADO FINALIZADO");
        analogWrite(buzzer, LOW);
        delay(500);
        lcd.setCursor(2, 2); lcd.print("LAVADO FINALIZADO");
        analogWrite(buzzer, 255);
        delay(500);
        lcd.setCursor(2, 2); lcd.print("LAVADO FINALIZADO");
        analogWrite(buzzer, LOW);
        delay(500);
        lcd.setCursor(2, 2); lcd.print("LAVADO FINALIZADO");
        analogWrite(buzzer, 255);
        delay(500);
        lcd.setCursor(2, 2); lcd.print("LAVADO FINALIZADO");
        analogWrite(buzzer, LOW);
        delay(500);
        analogWrite(red,255);
        analogWrite(green,0);
	analogWrite(blue,255);
        LCDmenu(); 
}



// DESPLAZA EL CURSOR UNA POSICION EN LA DIRECCION INDICADA
void LCDMueveCursor(int *pos, int max, char ope)
{
	// Precondición:	El parámetro ope debería de ser el carácter 's' o 'r' para sumar o restar una posición.
	//					El parámetro max indica el numero de opciones a elegir.
	// Poscondición:	Se devuelve por referencia el nuevo valor del cursor.

	if(ope == 's')
		if(*pos < max)	*pos += 1;
		else			*pos = 0;
	else
		if(*pos > 0)	*pos -= 1;
		else			*pos = max;

	// Se borran los cursores:
	lcd.setCursor(0, 0); lcd.print(" ");
	lcd.setCursor(0, 1); lcd.print(" ");
	lcd.setCursor(0, 2); lcd.print(" ");
	lcd.setCursor(0, 3); lcd.print(" ");
}



// PROCEDIMIENTO QUE COMPRUEBA SI HA HABIDO NUEVAS PULSACIONES CON LOS BOTONES:
void getButtonPush(char *resultButton, int pin, int pause)
{
	// Precondición:	El parámetro pin ha de ser un pin Analógico del Arduino.
	// Poscondición:	El parámetro resultButton es una cadena por referencia que devolverá el botón pulsado.

	int data = analogRead(0);

	if (data > 10)
	{
		if (data > 500 && data < 540)
			strcpy(resultButton,"up");
		else if (data > 330 && data < 370)
			strcpy(resultButton,"left");
		else if (data > 240 && data < 280)
			strcpy(resultButton,"bottom");
		else if (data > 190 && data < 230)
			strcpy(resultButton,"right");

		delay(pause);
	}
}
