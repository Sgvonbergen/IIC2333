# Tarea01

## El Programa:
Nuestro programa simula las acciones de un Scheduler Multi Level Feedback Queue (MLFQ). Para simular esto, existen varias versiones(ver Uso). Para la funcionalidad del programa se utilizaron tres principales estructuras:

### Process:
Cada process contiene toda la información relevante de cada proceso. Su cantidad de bursts y los ciclos que necesita cada uno, cuanto ciclos lleva ya dentro de la CPU, el nombre y PID. Ademas contienen datos estadisticos como Turnaround Time o Waiting time.

### Queue:
Cada queue representa una cola de procesos de distinta prioridad. Esta estructura contiene un puntero a cada proceso en que se encuentre en esa prioridad. Ademas existen varias funciones como Queue_get_process_by_start_time o Queue_pop que ayudan para la logica del scheduler.

### MLFQ
Esta estructura representa el scheduler en si. Aqui se guardan valores del alto nivel, como que version se esta ejecutando, el valor del quantum, las colas de prioridad y el proceso que esta actualmente usando la CPU. Tambien existen varias funciones como mlqf_tick en donde se ejecuta la mayoria de la logica de la simulación.

## Analisís Empírico
Existen variables estadisticas que nos permiten describir la diferencia de eficiencia entre versiones del Scheduler. Además, variando los valores de Q, quantum y S se pueden obtener distintos valores.

Cabe notar que para el caso de un q muy grande, el scheduler se comporta como Round Robin, circulando procesos hasta que cada uno termine solo. También cabe notar que si tenemos un S pequeño en comparación al quantum, un valor de Q muy alto es inutl, pues ningun proceso podra llegar y mantenerse por un tiempo en una prioridad mas baja.

En particular para el caso del test.txt:

PROCESS0 5 3 4 3 5

PROCESS1 11 3 1 4 7

PROCESS2 14 2 14 2


Se obtienen los siguentes valores en las 3 versiones del scheduler. con quantum = 3, Q = 3 y S = 15.

V1:

PROCESS0:

Turnos de CPU: 5
Bloqueos: 3
Turnaround time: 32
Response time: 0
Waiting time: 20

PROCESS1:

Turnos de CPU: 6
Bloqueos: 3
Turnaround time: 30
Response time: 1
Waiting time: 18

PROCESS2:

Turnos de CPU: 6
Bloqueos: 4
Turnaround time: 31
Response time: 2
Waiting time: 15

V2:

PROCESS0:

Turnos de CPU: 5
Bloqueos: 3
Turnaround time: 26
Response time: 0
Waiting time: 14

PROCESS1:

Turnos de CPU: 6
Bloqueos: 3
Turnaround time: 30
Response time: 1
Waiting time: 18

PROCESS2:

Turnos de CPU: 6
Bloqueos: 4
Turnaround time: 31
Response time: 2
Waiting time: 15


V3:

PROCESS0:

Turnos de CPU: 5
Bloqueos: 3
Turnaround time: 26
Response time: 0
Waiting time: 14

PROCESS1:

Turnos de CPU: 6
Bloqueos: 3
Turnaround time: 30
Response time: 1
Waiting time: 18

PROCESS2:

Turnos de CPU: 6
Bloqueos: 4
Turnaround time: 31
Response time: 2
Waiting time: 15


La unica diferencia entre las versiones se presenta en el PROCESS0. Esto en particular es porque el reset de prioridad causa que este consiga mas tiempo en la CPU que en el caso que no hubiera un S (V1). El waitTime y turnarountime del PROCESS0 es 6 segundos mas rapido en V2 y V3 que en V1. Notamos que no hay diferencia entre V2 y V3 dabido a que el valor del S es bastante bajo, el valor cambiante de Q dependiendo del nivel de prioridad no llega a afectar significativamente a los procesos.


## Uso:

Existen varias versiones del programa con distintas variables.

foo@bar:~$ ./mlfq [version] [file] [quantum] [Q] [S]

version: v1 o v2 o v3

file: Ruta al archivo que contiene los procesos a ser simulados

quantum: La cantidad de ciclos maximos que cada proceso puede estar corriendo en la CPU.

Q: la cantidad de colas de procesos.

S: El tiempo antes de que se reinicien todos los procesos de vuelta a la primera cola.
*S solo esta presente en v2 o v3.*


## Integrantes:

- Sebasthian von Bergen
- Francisco Paniagua
