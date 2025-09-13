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
  El objetivo general en este trabajo está compuesto por 4 índices:
    • Lograr realizar una comunicación con el módulo comercial MAX30100 a través del cual es 
      posible realizar mediciones de concentración de oxígeno y frecuencia cardíaca.
    • Lograr filtrar los datos obtenidos de impurezas (ruido) y enviar las señales al ordenador
      para su visualización. 
    • Visualizar, a través de un gráfico, las señales recibidas a lo largo del tiempo. 
    • Implementar una subrutina que represente un caso de necesidad donde el paciente (o su acompañante)
      realiza un llamado al personal médico.
    • Implementar umbrales de "NO SITUADO", "NORMAL" y "CRÍTICO" que establezcan el estado de paciente
      en función de las constantes monitoreadas (SPo2 y HR-heart rate-).

3- Implementación
  A fin de reducir los costos implementados en el proyecto, se utilizó la arquitectura de RP2350, la cual
  cuenta con capacidad para llevar a cabo una conexión inalambrica y, además, posee ejemplos de implementación
  de algoritmos para el cálculo y para la generación del algoritmo de encriptación SHA256. 
  La manera en la cual se implementaría el sistema, sería a través de task de freertos asociadas a la obtención de datos,
  filtrado de datos, chequeo de estado de obtención de datos por parte del módulo, filtrado de los datos y 
  envío hacia el ordenador. 

4- Resultados
  
  
  

