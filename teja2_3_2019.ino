////////////    librerias para display    ////////////////////
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

#define I2C_ADDR    0x27
LiquidCrystal_I2C             lcd(I2C_ADDR,2, 1, 0, 4, 5, 6, 7);

/////////////   libreria para eeprom    /////////////////////////
#include <EEPROM.h>

////////////    funcion del buzzer    /////////////////////
void beep(unsigned char pausa)
        {
            analogWrite(9, 20);           // suena
            delay(pausa);                 // Espera
            analogWrite(9, 0);            // Apaga
            delay(pausa);                 // Espera
        }
//se declara el tipo de variables a utilizar
int mas;
int menos;
int CT=0;
int mxc;
int segundos =0;
int set=0;
        
////////////funcion para sumar tiempo de juego por interrupcion///////
void credito(){
 
  CT=(mxc*60)+CT;
  set=1;
  int i=0;
 while(i<=30000){
  analogWrite(9, 20);           // suena
  i=i+1;
  }
  analogWrite(9, 0);           // apaga
}
//////////funcion para convertir en segundos//////////////  
void calseg(int count)
{

  int minuto;
  int segundo;
  int unidad;
  int decena;

  minuto = count/60;
  segundo = count-(minuto *60);
  unidad = segundo%10;
  decena = ((segundo/10)%10);
  
  lcd.setCursor ( 12, 1 );
  lcd.print(unidad);
  lcd.setCursor ( 11, 1 );
  lcd.print(decena);
}
//////////funcion para convertir en minutos//////////////  
void calmin(int count)
{
  int unidad;
  int decena;
  unidad = ((count/60)/1) % 10;
  decena = ((count/60)/10) % 10;
  
  lcd.setCursor ( 9, 1 );
  lcd.print(unidad);
  lcd.setCursor ( 8, 1 );
  lcd.print(decena);
}
// the setup function runs once when you press reset or power the board
void setup() 
{
  //////    asignamos a mxc el valor guardado en eeprom celda 2    ////////
  mxc=EEPROM.read(2);
  

  ////////declaramos interrupcion 0 del pin 2///////////
  attachInterrupt(0, credito, FALLING);
  
  ////////////    declaramos entradas y salidas     /////////////////
  pinMode(9, OUTPUT);      //pin 9 como salida buzzer
  pinMode(8, OUTPUT);     //pin 8 como salida rele
  
  pinMode(6, INPUT);     //pin 5 entrada boton -
  pinMode(5, INPUT);     //pin 4 como entrada boton +
 

////////    iniciamos con sonido de buzzer al energizar   //////
  
            beep(2000);

 /////////// Inicializar DISPLAY ///////////
 lcd.begin (16,2);    
  lcd.setBacklightPin(3,POSITIVE);
  lcd.setBacklight(HIGH);

  lcd.home ();                   // va a inicio
  lcd.print("Iniciando Timmer");
  lcd.setCursor ( 0, 1 );        // va a 2da linea
  lcd.print("    By Palio");
delay(1000);
delay(1000);
delay(1000);
  lcd.home ();                   // va a inicio
  lcd.print("Timmer XBOX PS4 ");
  lcd.setCursor ( 0, 1 );        // va a 2da linea
  lcd.print("    By Palio");
delay(1000);
delay(1000);
delay(1000);
  lcd.home ();                   // go home
  lcd.print("Min x Credito:  ");
  lcd.setCursor ( 14, 0 );        // 
  lcd.print(mxc);
  lcd.setCursor ( 0, 1 );        // va a 2da linea
  lcd.print(" Inserte Moneda");
delay(1000);   

}

////////////la funcion loop se ejecuta continuamente//////////////
void loop() 
{
  delay(300);
  /////////   en esta seccion leemos los minutos que se va ha trabajar    /////////////////
  mas = digitalRead(5);     //leemos estado pin 3 y almacenamos en variable
  if(mas == HIGH)      //si boton + esta en alto agregamos 1 minuto al ciclo de trabajo
  {
    mxc=mxc+1;
  mxc = max(mxc, 1); //limitamos valor inferior
  mxc = min(mxc, 60); //limitamos valor superior

  //MUESTRA MIN X CRED
  lcd.home ();                   // va a inicio de display
  lcd.print("Min x Credito:  ");
  lcd.setCursor ( 14, 0 );        // 
  lcd.print(mxc);
  delay(300);
  }
  
  menos = digitalRead(6);     //leemos estado pin 4 y almacenamos en variable
  if(menos == HIGH)           //si el boton b- esta en alto restamos un minuto al ciclo de trabajo
  {
    mxc=mxc-1; 
  mxc = max(mxc, 1); //limitamos valor inferior
  mxc = min(mxc, 60); //limitamos valor superior
  lcd.home ();                   // va a inicio de display
  lcd.print("Min x Credito:  ");
  lcd.setCursor ( 14, 0 );        // 
  lcd.print(mxc);//MUESTRA MIN X CRED
  delay(500);
  }
 //////////   se imprime valor agregado a MXC    //////////////
  lcd.home ();                   // va a inicio de display
  lcd.print("Min x Credito:  ");
  lcd.setCursor ( 14, 0 );        // 
  lcd.print(mxc);//MUESTRA MIN X CRED
  lcd.setCursor ( 0, 1 );        // va a 2da linea
  lcd.print(" Inserte Moneda");
  delay(200);
  
  ////////    leemos el boton set para empezar ciclo de trabajo     ////////////////
   
  CT=mxc*60;
    
    if (set==1)
    {
      /////almacenamos en eprom///
      EEPROM.update(2, mxc);
      segundos=59;

      //MENSAJE DE MONEDA
      lcd.home ();                   // va a inicio
      lcd.print("MONEDA INSERTADA");
      lcd.setCursor ( 0, 1 );        // va a 2da linea
      lcd.print("    GRACIAS     ");
      delay(1000);
      
      ////////conteo en segundos////////////////
      while(CT>=1)
      {
        for(segundos=59;segundos>=0;segundos--){
           CT=CT-1;
        digitalWrite(8,HIGH);//ACTIVA RELE
        //IMPRIME TIEMPO
        lcd.home ();                   // va a inicio de display
        lcd.print("Min x Credito:  ");
        lcd.setCursor ( 14, 0 );        // 
        lcd.print(mxc);//MUESTRA MIN X CRED
        lcd.setCursor ( 0, 1 );        // va a 2da linea
        lcd.print(" TIEMPO   :");
        calseg(CT); //muestra tiempo en segundos
        calmin(CT); //muestra tiempo en minutos
        delay(970);
        
        if(CT==30)/////primera advertencia de poco tiempo de juego
            {
          beep(500);  //prender buzzer
          beep(100);
          beep(500);  //prender buzzer
          beep(100);
          beep(500);  //prender buzzer
          beep(100);
            } 

            if(CT<=10)/////ultimas advertencias de poco tiempo de juego
            {
          beep(500);  //prender buzzer
          beep(100);
          beep(500);  //prender buzzer
          
            } 
   
        }
        
       }
      if(CT==0){
        set=0;
   digitalWrite(8, LOW);
   //TIEMPO TERMINADO
      lcd.home ();                   // va a inicio
      lcd.print("TIEMPO TERMINADO");
      lcd.setCursor ( 0, 1 );        // va a 2da linea
      lcd.print("    GRACIAS     ");
      delay(1000);
      beep(500);  //prender buzzer
      beep(100);
      beep(500);  //prender buzzer
      beep(100);
      beep(500);  //prender buzzer
      beep(100);
      delay(1000);
     }  
        }
        ///////////////aqui termina advertencias de pocotiempo de juego////////
     
  lcd.setCursor ( 0, 1 );        // va a 2da linea
  lcd.print("                ");
  delay(2);
   
 
    }
