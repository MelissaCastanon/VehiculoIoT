# Carrito controlado por ESP32 con motores y sensor ultrasónico.

Este proyecto implementa un carrito controlado por un ESP32, que utiliza un sensor ultrasónico para medir distancias y ajustar su movimiento en consecuencia. El carrito es capaz de moverse hacia adelante, hacia atrás, girar y detenerse basado en la distancia detectada por el sensor. Las llantas del carrito son controladas por un motor driver, y su velocidad se ajusta a través de PWM.

Funcionalidades principales:
Medición de distancia usando un sensor ultrasónico.
Control de motores mediante un driver de motor y pines de dirección.
Movimientos definidos por una matriz de secuencias que permite avanzar, retroceder, girar y detener el carrito.
Ajuste dinámico de la velocidad basado en la distancia a un obstáculo.
