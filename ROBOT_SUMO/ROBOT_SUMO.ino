/******************************************************************************
; Universidad Del Valle De Guatemala
; IE3028: Simulación y Fabricación de PCB
; Autor: Samuel Tortola - 22094
; Proyecto: Robot Sumo (Super sumo)
; Hardware: ESP32
; Creado: 26/03/2024
; Última modificación: 20/05/2024 
;******************************************************************************/


#include "BluetoothSerial.h"   //Incluir libreria de bluetooth

const char *pin = "2020"; //secure PIN.
String device_name = "ESP32-SUPER_SUMOT";   //Nombre del dispositivo 

const int mot1_1 = 2;   //Control de motor 1 1
const int mot1_2 = 0;   //Control de motor 1 2
const int mot2_1 = 4;   //Control de motor 2 1
const int mot2_2 = 15;  //Control de motor 2 2
const int LED = 16; //LED de iluminación
const int Voltaje = 35; //Lectura ADC de voltaje de bateria
const int SensorV = 21;   //Conocer el estado de carga la bateria
float lectura;  //Almacena valor de lectura ADC
float volt = 0; //Valor real de voltaje
int lecSensorV = 0;
int estadobat = 0;  //Estado de la bateria

char datos;   //Variable para almacenar los datos entrantes de bluetooth

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)  //Configuración bluetooth
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Bluetooth not available or not enabled. It is only available for the ESP32 chip.
#endif

BluetoothSerial SerialBT;   //Dar nombre al objeto bluetooth

void setup() {
  Serial.begin(115200);  //Iniciar serial a 115200 DB
  SerialBT.begin(device_name); //Bluetooth device name
  Serial.printf("The device with name \"%s\" is started.\nNow you can pair it with Bluetooth!\n", device_name.c_str());
  
  #ifdef USE_PIN
    SerialBT.setPin(pin);
    Serial.println("Using PIN");
  #endif

  pinMode(mot1_1,OUTPUT);   //pines a L293D como salidas
  pinMode(mot1_2,OUTPUT);
  pinMode(mot2_1,OUTPUT);
  pinMode(mot2_2,OUTPUT);
  pinMode(LED,OUTPUT);
  pinMode(SensorV,INPUT);
}

void loop() {


  if (Serial.available()) {   //Si se desea escribir 
    SerialBT.write(Serial.read()); //Enviar lo que se escribe
  }
  
  if (SerialBT.available()) {   //Si se recibe un mensaje nuevo de bluetooth
    datos = (char)SerialBT.read();  //Almacenar en datos, el caracter recibido
    
    if (datos == 'A'){   //Si se desea mover el sumo hacia adelante
      digitalWrite(mot1_1,1);  //Encender el pin 
      digitalWrite(mot2_2,1);  //Encender el pin 
      digitalWrite(mot1_2,0);  //Apagar pin 
      digitalWrite(mot2_1,0);  //Apagar pin 
      
    }

     else if (datos == 'B'){ //Si se desea mover el sumo a la atras
      digitalWrite(mot1_1,0);  //Apagar pin
      digitalWrite(mot2_2,0);  //Apagar pin 
      digitalWrite(mot1_2,1);  //Encender el pin 
      digitalWrite(mot2_1,1); //Encender el pin  
    }

    else if (datos == 'C'){   //Si se desea mover el sumo hacia derecha
      digitalWrite(mot1_1,1);  //Encender el pin 
      digitalWrite(mot2_2,0);  //Apagar pin 
      digitalWrite(mot1_2,0);  //Apagar pin 
      digitalWrite(mot2_1,1); //Encender el pin  
     
    }

    else if (datos == 'D'){ //Si se desea mover el sumo hacia izquierda
      digitalWrite(mot1_1,0);  //Apagar pin 
      digitalWrite(mot2_2,1);   //Encender el pin
      digitalWrite(mot1_2,1);   //Encender el pin 
      digitalWrite(mot2_1,0); //Apagar pin  
    }
    
   else if (datos == 'E'){ //Si se desea Apagar todo 
      digitalWrite(mot1_1,0);  //Apagar pin 
      digitalWrite(mot2_2,0);   //Apagar pin 
      digitalWrite(mot1_2,0);   //Apagar pin 
      digitalWrite(mot2_1,0); //Apagar pin  
    }

     else if (datos == 'F'){ //Si se desea iluminación
      digitalWrite(LED,1);  //Encender pin 
    }

     else if (datos == 'G'){ //Si se desea apagar la iluminación
      digitalWrite(LED,0);  //Apagar pin 
    }

     else if(datos == 'H'){
       lectura = analogRead(Voltaje);
       volt = (map(lectura, 0, 4095, 0, 36367.59939)/1000)+ 1.90;  //Conversion de voltaje
       SerialBT.print(volt); //Enviar lo que se escribe
       SerialBT.print(";"); //Separación
       SerialBT.print(estadobat);
       SerialBT.println(";"); //Separación
      
     }
  }

  
  lecSensorV = digitalRead(SensorV);  //Lectura del sensor de carga de bateria
  
  if(lecSensorV == 1){   //Se la bateria se esta cargando 
    estadobat  = 1;
  }
  else{    //Si la bateria se esta descargando 
    estadobat = 0; 
  }
 
  delay(20);  //Pequeño retardo
}
