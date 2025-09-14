Proyecto de monitoreo inalambrico de constantes vitales - PRIMEROS PASOS

1- Fundamento
  El fundamento detrás de este proyecto es comenzar con los primeros pasos sobre el proyecto
  propuesto en la materia de ingeniería de software, el cual consistía en implementar una central
  de monitoreo de constantes vitales en una sala de internación general. 
  Inicialmente, en el módulo de "Seguridad de la información", se enfocó el trabajo de la misma 
  hacia los aspectos de ciberseguridad del pertinente proyecto. En el presente módulo, se avanzó 
  sobre el mismo proyecto, en términos de obtención y procesamiento de datos, además de su correspondiente envío hacia el ordenador. 
  Además, debido a una posible situación crítica de un paciente, es necesario que el desempeño de dicho
  monitoreo, sea llevado a cabo en tiempo real, garantizando que los datos visualizados no se encuentran 
  desfasados en el tiempo con las constantes vitales reales. Este requerimiento, asegura que el personal de 
  salud encargado de dichos cuidados, tenga la capacidad de realizar su labor respondiendo al instante a las necesidades
  de los pacientes.

2- Objetivo
  El objetivo general en este trabajo está compuesto por 2 índices:
    • Lograr realizar una comunicación con el módulo comercial MAX30100 a través del cual es 
      posible realizar mediciones de concentración de oxígeno y frecuencia cardíaca.
    • Visualizar, a través de un gráfico, las señales recibidas a lo largo del tiempo y que sus variaciones
      coincidan con los latidos en tiempo real. 

3- Implementación
  A fin de reducir los costos implementados en el proyecto, se utilizó la arquitectura de RP2350, la cual
  cuenta con capacidad para llevar a cabo un futura conexión inalambrica y, además, posee ejemplos de implementación
  de algoritmos para el cálculo y para la generación del algoritmo de encriptación SHA256. 
  Partiendo con el primer objetivo, es necesario aclarar ciertos apartados acerca del módulo MAX30100
  
  3.1 Módulo MAX30100
    El módulo MAX30100 consiste en un sistema embebido que permite ser empleado como rápida solución para el 
    monitoreo de frecuencia cardíaca y saturación de oxígeno. Este contiene dos leds, uno infrarrojo y otro rojo, 
    y un fotodetector. 
    El funcionamiento detrás de este módulo consiste en el fenómeno de la reflexión de la luz emitida por ambos leds.
    Al colocar un dedo sobre dichos leds, parte de ese haz se refleja sobre el fotodectector en función de la saturación
    de oxígeno presente en la persona. El led infrarrojo, brinda mayor información acerca de la porción oxigenada de la 
    hemoglobina; por su parte, el led rojo, brinda una mayor información acerca de la porción desoxigenada de la hemoglobina.
    Ambas señales son pulsátiles y coinciden con los latidos cardíacos. 
    La saturación de oxigeno en una persona, está en estrecha relación con el cociente entre los valores de ambas señales. Por
    su parte, la frecuencia cardíaca, se obtiene a partir de la inversa del tiempo entre pulsos de la señal. 
    Este módulo es configurado a través de la escritura de sus registros en su memoria por medio del protocolo I2C. La manera 
    de llevar esto a cabo es:
  Escribir:
      
        Condición de Start en I2C             + Enviar Address (write)  + Registro a escribir + Valor.
  Leer
  
        Condición de Start en I2C             + Enviar Address (write)  + Registro a leer
        Condicion de Repeated Start en I2C    + Enviar Address (read)   
  
  Dentro de estas configuraciones es necesario establecer, entre otras variables, interrupción por medición realizada, si
  interrumpe por Frecuencia cardíaca o por spo2 (la diferencia es si envía solamente valores del led IR o valores tanto del
  led IR como del led R); Frecuencia de muestreo e intensidad de led; Resolución del ADC (en estrecha relación con la fre-
  cuencia de muestreo); Puntero de lectura; Puntero de escritura; etc. 
  Los datos son almacenados en una FIFO de 8 bits capaz de almacenar hasta 16 muestras. Como la resolución máxima es de 16 bits
  la fifo almacena los datos como: 

                          | 15:8 LED IR | 7:0 LED R | 15:8 LED R | 7:0 LED R |
  
  Por lo cual, para la obtención de los mismos, es necesario leer 2 lugares en la memoria. El módulo posee registros que 
  permiten saber donde está el puntero de lectura y el de escritura. Para garantizar una "armonía" en la coordinación del 
  proyecto en cuanto a lectura y escritura del módulo, se estableció que siempre se va a leer la muestra anterior a donde se 
  encuentre el puntero de escritura. Dicho procedimiento se lleva a cabo en la tarea: 
  
                                  void gpio_check (void*pvParameters)
  
  3.2 Módulo CP2102
  Para la visualización de datos, se implementó el envío de estos por medio de protocolo UART hacia un módulo USB TO TTL que 
  permitía comunicar a la arquitectura RP2350 con el ordenador por medio de la conexión del módulo CP2102 conectado a uno
  de los puertos USB.
  
4- Resultados:
  Los resultados demostraron la correcta obtención de datos, ya que, evaluando con los latidos propios, la curva de dichos datos
  coincidia con el momento de los latidos. 
  A fin de evaluar el desempeño del módulo, se implementó el mismo algoritmo para 16 y para 12 bits del ADC. Como resultado de dicho
  experimento, se apreció que la conversión de 16 bits posee una menor componente de ruido.

5- Trabajo futuro:
  El siguiente paso en dicho proyecto esta compuesto por las aristas restantes del mismo, las cuales son, en terminos generales:
    • Establecer una conexión inalambrica entre dos puntos, ya sea por medio de una red local u otro tipo de red, donde uno de ellos
      sea el encargado de transmitir los datos obtenidos mientras que el restante sea el encargado de recibirlos y transmitirlos nuevamente
      hacia un posible software de monitoreo. 
    • Llevar a cabo el procesamiento de señales necesarios para traducir los datos de intensidad reflejada en datos de frecuencia
      cardíaca y de saturación de oxígeno. 
  
  
  
  

