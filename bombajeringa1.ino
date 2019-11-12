//Declarar variables
//  Variables de tiempo

double duty; //porcentaje de tiempo de encendido del pin con respecto al periodo
double pinON; //tiempo de encendido del pin
double pinOFF; // tiempo de apagado del pin
//  Pines
const int dir = 2;
const int stp = 3;
//const int MS2=4;
const int MS1 = 5;
const int EN = 6;

//Variables

unsigned long t_ejec; // en milisec
double periodo_paso; // Periodo entre paso y paso, o periodo de la onda generada

int Q; // caudal
int vol; //en ml
const int R; //volumen de jeringa recorrido cuando el motor da 1 paso

int aux;

void setup() {
  pinMode(stp, OUTPUT);
  pinMode(dir, OUTPUT);
  pinMode(MS1, OUTPUT);
  //  pinMode(MS2, OUTPUT);
  pinMode(EN, OUTPUT);
  resetBEDPins(); //Colocar pines en su estado por defecto
  Serial.begin(9600); //Abrir comunicación serial (USB)
  //valores()
  t_ejec = 60000; // en milisegundos, 60000 es 1 minuto
  periodo_paso = 20; // en milisegundos(como la velocidad)
  //periodo_paso = 1 / (Q * R); //en segundos/pasos
  duty = 0.5; //porcentaje
  pinON = duty * periodo_paso;
  pinOFF = (1 - duty) * periodo_paso;
  aux = 0;
}

//Main loop
void loop() {
  if (aux == 0) {
    pulsado();// Hacer 1000 pasos
    //continuo(t_ejec);
    aux = 1;
  }
}

void continuo(unsigned long t_ejeco) {
  digitalWrite(EN, LOW); // En low, habilita los FETs, para poder controlar el motor
  unsigned long startMillis = millis(); //empezamos a medir tiempo
  while (millis() - startMillis < t_ejeco) {
    digitalWrite(stp, HIGH);
    wait(pinON);
    digitalWrite(stp, LOW);
    wait(pinOFF);
  }
  digitalWrite(EN, HIGH); //Apagar FETs
}

void pulsado() {
  unsigned long t_cont;
  unsigned long t_apag;
  unsigned long startMillis;
  // Darle valores a t_cont y t_apag en milisegundos, ambos son entrada
  t_cont = 15000;
  t_apag = 5000;
  digitalWrite(EN, LOW); // En low, habilita los FETs, para poder controlar el motor
  startMillis = millis(); //empezamos a medir tiempo
  while (millis() - startMillis < t_ejec) {
    continuo(t_cont);
    wait(t_apag);
  }
}

void valores() {
  Serial.println("Ingrese el volumen");
  while (Serial.available() <= 0) {   // is a character available?
    vol = Serial.read();
  }
  Serial.println("Ingrese el tiempo de ejecucion");
  while (Serial.available() <= 0) {   // is a character available?
    t_ejec = Serial.read();
  }
}

void wait(unsigned long t_wait) {
  unsigned long startMillis=millis();
  while (millis() - startMillis < t_wait) {
 
  }
}

void resetBEDPins() {
  digitalWrite(stp, LOW);
  digitalWrite(dir, LOW);
  digitalWrite(MS1, HIGH); //en HIGH habilita micropasos
  //  digitalWrite(MS2, LOW);
  digitalWrite(EN, HIGH); //en HIGH deshabilita movimiento de motor
}
