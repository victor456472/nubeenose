nubeenose

descripcion

el presente repositorio guarda el codigo fuente del programa cargado en el
olfato electronico desarrollado por los estudiantes Samir Parra y Victor Vargas en
asesoria del ingeniero Kelvin Beleño.

|--lib
|  |
|  |--Bar
|  |  |--docs
|  |  |--examples
|  |  |--src
|  |     |- Bar.c
|  |     |- Bar.h
|  |  |- library.json (optional, custom build options, etc) https://docs.platformio.org/page/librarymanager/config.html
|  |
|  |--Foo
|  |  |- Foo.c
|  |  |- Foo.h
|  |
|  |- README --> THIS FILE
|
|- platformio.ini
|--src
   |- main.c


el archivo #1 es el codigo principal que se carga al controlador.

este hace uso de las librerias DHT.h, DHT_U y MQlab para reducir la cantidad de
lineas dentro del programa y asi facilitar su comprensión.

los archivos #2 y #3 correspoden al codigo fuente de la libreria MQlab, la cual
desarrollaron los autores para el uso de matrices de 10 sensores MQ.
