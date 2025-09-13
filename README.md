Proyecto de monitoreo inalambrico de constantes vitales - PRIMEROS PASOS

1- Fundamento
  El fundamento detrás de este proyecto es comenzar con los primeros pasos sobre el proyecto
  propuesto en la materia de ingeniería de software, el cual consistía en implementar una central
  de monitoreo de constantes vitales en una sala de internación general. 
  Inicialmente, en el módulo de "Seguridad de la información", se enfocó el trabajo de la misma 
  hacia los aspectos de ciberseguridad del pertinente proyecto. En el presente módulo, el objetivo
  planteado inicialmente fue, avanzar sobre el mismo proyecto, en términos de obtención y procesamiento 
  de datos, además de su correspondiente envío hacia el ordenador. 
  Además, debido a una posible situación crítica de un paciente, es necesario que el desempeño de dicho
  monitoreo, sea llevado a cabo en tiempo real, garantizando que los datos visualizados no se encuentran 
  desfasados en el tiempo con las constantes vitales reales. Este requerimiento, asegura que el personal de 
  salud encargado de dichos cuidados, tenga la capacidad de realizar su labor respondiendo al instante a las necesidades
  de los pacientes.

2- Objetivo
  El objetivo general en este trabajo está compuesto por 2 índices:
    • Lograr realizar una comunicación con el módulo comercial MAX30100 a través del cual es 
      posible realizar mediciones de concentración de oxígeno y frecuencia cardíaca.
    • Visualizar, a través de un gráfico, las señales recibidas a lo largo del tiempo. 

3- Implementación
  A fin de reducir los costos implementados en el proyecto, se utilizó la arquitectura de RP2350, la cual
  cuenta con capacidad para llevar a cabo un futura conexión inalambrica y, además, posee ejemplos de implementación
  de algoritmos para el cálculo y para la generación del algoritmo de encriptación SHA256. 
  La manera en la cual se implementaría el sistema, consistió en la compra e implementacion del módulo MAX30100 y su
  posterior configuración por medio de escritura hacia lugares de memoria de dicho móduloi a través de I2C con la 
  arquitectura pertinente. 
  En cuanto al algoritmo implementado, se realizó un sistema basado en tareas, eventos y colas de freertos para evitar un posible
  deadlock, entre las tareas encargadas de guardar los datos obtenidos por el módulo y la encargada de enviarlos al 
  ordenador, o algún posible busy waiting. 
  
  3.1 Módulo MAX30100
  3.2 Módulo CP2102

4- Resultados
  
  
  

