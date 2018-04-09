##Tarea01

#El Programa:
=============
Nuestro programa simula las acciones de un Scheduler Multi Level Feedback Queue (MLFQ). Para simular esto, existen varias versiones(ver Uso). Para la funcionalidad del programa se utilizaron tres principales estructuras:

#Process:
Cada process contiene toda la información relevante de cada proceso. Su cantidad de bursts y los ciclos que necesita cada uno, cuanto ciclos lleva ya dentro de la CPU, el nombre y PID. Ademas contienen datos estadisticos como Turnaround Time o Waiting time.

#Queue:
Cada queue representa una cola de procesos de distinta prioridad. Esta estructura contiene un puntero a cada proceso en que se encuentre en esa prioridad. Ademas existen varias funciones como Queue_get_process_by_start_time o Queue_pop que ayudan para la logica del scheduler.

#MLFQ
Esta estructura representa el scheduler en si. Aqui se guardan valores del alto nivel, como que version se esta ejecutando, el valor del quantum, las colas de prioridad y el proceso que esta actualmente usando la CPU. Tambien existen varias funciones como mlqf_tick en donde se ejecuta la mayoria de la logica de la simulación.

#Uso:
=====
Existen varias versiones del programa con distintas variables.
'''bash
foo@bar:~$ ./scheduler [version] [file] [quantum] [Q] [S]
'''
version: v1 o v2 o v3
file: Ruta al archivo que contiene los procesos a ser simulados
quantum: La cantidad de ciclos maximos que cada proceso puede estar corriendo en la CPU.
Q: la cantidad de colas de procesos.
S: El tiempo antes de que se reinicien todos los procesos de vuelta a la primera cola.
*S solo esta presente en v2 o v3.*


#Integrantes:
=============
- Sebasthian von Bergen
- Francisco Paniagua
