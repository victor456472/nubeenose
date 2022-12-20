# nubeenose

##descripcion
el presente repositorio guarda el codigo fuente del programa cargado en el
olfato electronico desarrollado por los estudiantes Samir Parra y Victor Vargas en
asesoria del ingeniero Kelvin Beleño.

en este hay tres archivos importantes:<br>
|<br>
|<br>
|___src<br>
|\t\t|<br>
|  |<br>
|  |__main.cpp      <---archivo #1<br>
|<br>
|__lib<br>
|  |<br>
|  |__Adafruit_Sensor-master<br>
|  |<br>
|  |__DHT_sensor_library-1.4.4<br>
|  |<br>
|  |__MQlab<br>
|     |<br>
|     |__MQlab.cpp  <---archivo #2<br>
|     |<br>
|     |__MQlab.h    <---archivo #3<br>


el archivo #1 es el codigo principal que se carga al controlador.

este hace uso de las librerias DHT.h, DHT_U y MQlab para reducir la cantidad de
lineas dentro del programa y asi facilitar su comprensión.

los archivos #2 y #3 correspoden al codigo fuente de la libreria MQlab, la cual
desarrollaron los autores para el uso de matrices de 10 sensores MQ.
