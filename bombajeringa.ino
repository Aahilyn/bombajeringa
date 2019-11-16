//Declarar variables

/* Variables de forma de onda cuadrada */
// Porcentaje de tiempo de encendido del pin con respecto al periodo
// Puede darle más fuerza al paso (entre 0 y 1)
double duty;        

double pinON;       // Tiempo de encendido del pin 
double pinOFF;      // Tiempo de apagado del pin


/*  Pines */
const int dir = 2;  // Pin de direccion
const int stp = 3;  // Pin de paso (acción de dar un paso)
//const int MS2=4;  // Pin de segunda precision
const int MS1 = 5;  // Pin de primera precision
const int EN = 6;   // Pin de ENABLE

/*Variables de tiempo*/
// Tiempo de ejecucion total del arduino, en milisec, puede almacenar un máximo de 50 dias
unsigned long t_ejec;

// Tiempo de ejecucion continuo del arduino, en milisec (solo en la funcion pulsado)
unsigned long t_cont;
// Tiempo de ejecucion apagado del arduino, en milisec (solo en la funcion pulsado)
unsigned long t_apag;


// Periodo entre paso y paso, o periodo de la onda generada
// Es inversamente proporcional a la velocidad del motor
double periodo_paso; 

/* Variable auxiliar*/
// Variable auxiliar de encendido o apagado del dispositivo
// Si vale 0, se encenderá, caso contrario se apagará
int encendido; 
 
void setup()
{
  pinMode(stp, OUTPUT);
  pinMode(dir, OUTPUT);
  pinMode(MS1, OUTPUT);
  //  pinMode(MS2, OUTPUT);
  pinMode(EN, OUTPUT); // En LOW, habilita los FETs, para poder controlar el motor, caso contrario en HIGH
  resetPins(); //Colocar pines en su estado por defecto
    
  Serial.begin(9600); //Abrir comunicación serial (USB)
    
  // ENTRADA Y SALIDA
  t_ejec = 60000*10; // en milisegundos, 60000 es 1 minuto
  t_cont = 15000;
  t_apag = 5000;
  periodo_paso = 100; // en milisegundos, 20 default
  duty = 0.3; //porcentaje
  encendido = 0;
  
  pinON = duty * periodo_paso;
  pinOFF = (1 - duty) * periodo_paso;
}

//Main loop
void loop()
{
  if (encendido == 0) {  
    Serial.println("Inicio");
    
    Serial.print("t_ejec: ");
    Serial.println(t_ejec);
    
    Serial.print("t_cont: ");
    Serial.println(t_cont);
    
    Serial.print("t_apag: ");
    Serial.println(t_apag);

    Serial.print("periodo_paso: ");
    Serial.println(periodo_paso);

    Serial.print("duty: ");
    Serial.println(duty);
    
    //continuo(t_ejec);
    //pulsado(t_ejec,t_cont,t_apag)
    
    encendido = 1; //Para ejecutar sólo una ves, lo colocamos igual a 1
    Serial.println("Fin");
  }
}

// Funcion que realiza un movimiento continuo
void continuo(unsigned long t_ejeco) 
{
  digitalWrite(EN, LOW); // Habilitar motores
  unsigned long startMillis = millis(); // Empezamos a medir tiempo
  
  // Comparamos el tiempo de ejecucion con la diferencia de tiempo entre que se empieza a ejecutar Arduino
  // y el tiempo actual
  while ( millis() - startMillis < t_ejeco) { 
    // Dar un paso
    digitalWrite(stp, HIGH);
    wait(pinON);
    digitalWrite(stp, LOW);
    wait(pinOFF);
  }
  digitalWrite(EN, HIGH); // Deshabilitar motores
}

// Funcion que realiza un movimiento alternado, entre continuo y apagado
void pulsado(unsigned long t_ejeco, unsigned long t_cont, unsigned long t_apag)
{ 
  digitalWrite(EN, LOW); // Habilitar motores
  unsigned long startMillis = millis(); // Empezamos a medir tiempo

  // Comparamos el tiempo de ejecucion con la diferencia de tiempo entre que se empieza a ejecutar Arduino
  // y el tiempo actual
  while (millis() - startMillis < t_ejeco) {
    continuo(t_cont);
    wait(t_apag);
  }
  digitalWrite(EN, HIGH); // Deshabilitar motores
}

void valores() {
  t_ejec = 60000*10; // en milisegundos, 60000 es 1 minuto
  t_cont = 15000;
  t_apag = 5000;
  periodo_paso = 100; // en milisegundos, 20 default
  duty = 0.3; //porcentaje
  encendido = 0;
  
  Serial.println("Ingrese el volumen");
  while (Serial.available() <= 0) {   // is a character available?
    
  }
  Serial.println("Ingrese el tiempo de ejecucion");
  while (Serial.available() <= 0) {   // is a character available?
    t_ejec = Serial.read();
  }
}

// Funcion que espera un tiempo determinado
void wait(unsigned long t_wait) 
{ 
  unsigned long startMillis=millis();
  while (millis() - startMillis < t_wait) {
  }
}

// Funcion que resetea el estado de los pines
void resetPins()
{ 
  digitalWrite(stp, LOW);
  digitalWrite(dir, LOW);
  digitalWrite(MS1, HIGH); //en HIGH habilita micropasos
  //  digitalWrite(MS2, LOW);
  digitalWrite(EN, HIGH); //en HIGH deshabilita movimiento de motor
}
