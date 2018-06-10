# Tarea03

## Desiciones de implementación

### Envio de datos
Como era necesario enviar un byte de información como unidad basica.
decidimos que para enviar datos numericos casteariamos un unsigned char en base
a al dato numerico correspondiente. Por ejemplo si queremos enviar un 10 (dec) necesitamos enviar 00001010. con el objetibo
de lograr esto casteamos el 10 a un char (independiente del valor del caracte en ascci que esto produsca) de tal forma de enviar en un byte la información del numero. a diferencia de transformalo a un array de char (e.g. char msg[8] = "00001010"), en donde estariamos usando 8 bytes para mandar un int. Al momento de recibir el dato es necesario castear denuevo el numero (e.g. int num = (int)msg )


## Integrantes:

- Sebasthian von Bergen
- Francisco Paniagua (15638952)
