# Tarea03

## Decisiones de implementación

### Flujo Dumb Client
El flujo lógico del cliente depende en gran parte del Servidor, ya que, luego de que el cliente establece una conexión con el servidor. el cliente se queda esperando en un **ciclo** los requests del servidor y dependiendo de estos el cliente va **reaccionando**, es decir, dependiendo del id del mensaje, el cliente despliega la información y envía mensajes al servidor.

## Supuestos

### Envío de datos
Como era necesario enviar **un byte** de información como unidad básica.
decidimos que para enviar datos numéricos casteariamos **un unsigned char** en base
a al dato numérico correspondiente. Por ejemplo si queremos enviar un 10 (dec) necesitamos enviar 00001010. con el objetivo
de lograr esto casteamos el 10 a un char (independiente del valor del carácter en ascci que esto produzca) de tal forma de **enviar en un byte** la información del numero. a diferencia de transfórmalo a un array de char (e.g. char msg[8] = "00001010"), en donde estaríamos usando 8 bytes para mandar un int. Al momento de recibir el dato es necesario castear denuevo el numero (e.g. int num = (int)msg )

### Endians
En el caso en que un numero sea mayor a 255. este se divide en dos partes 8bits/8bits los primeros 8bits
corresponden a la parte entera de la división entre el numero y 255. La segunda parte corresponde al resto de esa misma división.
Una vez que tenemos las dos partes, estas son enviadas de la manera descrita anteriormente (Envío de datos), y luego son unidas
sumando la multiplicación de la primera parte por 255 y la segunda parte. Lo cual es equivalente a juntar los números 8bits/8bits.



## Integrantes:

- Sebasthian von Bergen (15635635)
- Francisco Paniagua (15638952)
