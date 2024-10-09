// Pines del sensor ultrasónico
const int pinEcho = 16;       // Pin ECHO del sensor ultrasónico para recibir la señal
const int pinTrigger = 17;    // Pin TRIG del sensor ultrasónico para enviar el pulso

// Pines de las llantas (IN1, IN2, IN3, IN4, enA, enB)
const int controlPines[] = {33, 32, 25, 26, 27, 14};  // Definición de los pines para controlar el motor driver (dos motores con dos pines de dirección y uno de velocidad cada uno)

// Variables de control de velocidad y distancias
int velocidadMotor = 120;               // Velocidad inicial para los motores (PWM de 0 a 255)
const int distanciaAlerta = 50;         // Distancia (en cm) a la que el carrito comenzará a reducir la velocidad
const int distanciaParada = 40;         // Distancia mínima (en cm) a la que el carrito se detendrá por completo

// Matriz que define los movimientos de las llantas y su velocidad
const int secuenciaMovimientos[][6] = {
  {1, 0, 1, 0, 120, 120},  // 0- Adelante: las llantas izquierda y derecha giran hacia adelante, ambas a velocidad 120
  {0, 1, 0, 1, 120, 120},  // 1- Atrás: las llantas giran hacia atrás, ambas a velocidad 120
  {0, 1, 1, 0, 90, 200},   // 2- Giro Izquierda: la llanta izquierda gira hacia atrás y la derecha hacia adelante (diferentes velocidades)
  {1, 0, 0, 1, 200, 90},   // 3- Giro Derecha: la llanta izquierda hacia adelante y la derecha hacia atrás (diferentes velocidades)
  {1, 0, 1, 0, 100, 100},  // 4- Adelante despacio: ambas llantas hacia adelante, pero a velocidad reducida (100)
  {0, 0, 0, 0, 0, 0}       // 5- Detener: ambas llantas se detienen (velocidad 0)
};

void setup() {
  // Configuración de los pines como salidas para los motores
  for (int i = 0; i < 6; i++) {
    pinMode(controlPines[i], OUTPUT);  // Todos los pines en el arreglo 'controlPines' son configurados como salidas
  }
  
  // Configuración de los pines del sensor ultrasónico
  pinMode(pinTrigger, OUTPUT);  // El pin Trigger se configura como salida
  pinMode(pinEcho, INPUT);      // El pin Echo se configura como entrada

  // Inicia la comunicación serial para monitoreo y depuración
  Serial.begin(9600);  // Se configura la velocidad de comunicación serial en 9600 baudios
}

// Función para medir la distancia con el sensor ultrasónico
long calcularDistancia() {
  // Envía un pulso corto de 10 microsegundos al pin Trigger
  digitalWrite(pinTrigger, LOW);          // Se asegura que el Trigger esté en estado bajo
  delayMicroseconds(2);                   // Pausa de 2 microsegundos
  digitalWrite(pinTrigger, HIGH);         // Se pone el Trigger en estado alto
  delayMicroseconds(5);                   // Se mantiene el pulso de 5 microsegundos
  digitalWrite(pinTrigger, LOW);          // Se vuelve a poner en estado bajo

  // Lee la duración del pulso recibido en el pin Echo (tiempo en microsegundos)
  long duracionPulso = pulseIn(pinEcho, HIGH); // Espera a recibir el pulso de regreso en el Echo
  
  // Convierte la duración del pulso en distancia (en cm) usando la fórmula de velocidad del sonido
  long distanciaCm = duracionPulso / 58.2;   // El valor de 58.2 convierte microsegundos a centímetros
  return distanciaCm;                        // Retorna la distancia medida
}

// Función para mover el carrito con base en la matriz de movimientos
void accionarCarrito(const int movimiento[]) {
  // Control de los pines de las llantas para definir la dirección
  for (int i = 0; i < 4; i++) {
    digitalWrite(controlPines[i], movimiento[i]);  // Escribe el valor de movimiento (1 o 0) en los pines de las llantas
  }
  
  // Control de la velocidad de las llantas usando PWM
  analogWrite(controlPines[4], movimiento[4]);  // Controla la velocidad del motor izquierdo con el valor PWM
  analogWrite(controlPines[5], movimiento[5]);  // Controla la velocidad del motor derecho con el valor PWM
}

// Bucle principal donde se controla el movimiento del carrito
void loop() {
  // Medir la distancia usando el sensor ultrasónico
  long distanciaActual = calcularDistancia();  // Llama a la función para medir la distancia y la guarda en la variable 'distanciaActual'
  Serial.print("Distancia a: ");               // Imprime el mensaje "Distancia a:" en el monitor serial
  Serial.println(distanciaActual);             // Imprime el valor de la distancia medida

  // Control del movimiento basado en la distancia medida
  if (distanciaActual < distanciaParada) {               // Si la distancia es menor que la distancia mínima
    accionarCarrito(secuenciaMovimientos[5]);   // Detener el carrito (secuenciaMovimientos[5] corresponde a parar)
    delay(1000);                                // Espera 1 segundo
    accionarCarrito(secuenciaMovimientos[1]);   // Camina para atrás (secuenciaMovimientos[1] corresponde a caminar para atrás)
    delay(500);                                 // Durante medio segundo
    accionarCarrito(secuenciaMovimientos[2]);   // Girar a la izquierda (secuenciaMovimientos[2] corresponde a girar izquierda)
    delay(250);                                 // Espera 250 ms mientras gira
  } else if (distanciaActual < distanciaAlerta) {        // Si la distancia está entre la mínima y la segura
    accionarCarrito(secuenciaMovimientos[4]);   // Avanzar despacio (secuenciaMovimientos[4] corresponde a avanzar lento)
  } else {                                          
    accionarCarrito(secuenciaMovimientos[0]);   // Avanzar a velocidad normal (secuenciaMovimientos[0] corresponde a avanzar)
  }

  delay(100);  // Pausa de 100 ms antes de la siguiente lectura para evitar lecturas continuas demasiado rápidas
}
