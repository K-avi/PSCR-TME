# TME6

## Exercice 1.2

### Question 1

Il y a 10 processus engendrés.

Le main fait 1 fils puis sors. Il affiche mainpid=[pid]

Son fils F1 fait 2 (F2,F3)  fils puis sort. Il affiche "i:j 1:0"
F2 affiche "k:j 1:0" puis sort.

F3 affiche "i:j 2:3". F3 fait 3 fils (F4, F5, F6) puis sort.

F4 affiche "k:j 1:0" puis sort.
F5 affiche "k:j 2:0" puis sort.

F6 affiche "i:j 3:3". F6 fais 3 fils (F7, F8, F9) puis sort.
F7 affiche "k:j 1:0" puis sort.
F8 affiche "k:j 2:0" puis sort.
F9 affiche "k:j 3:0" puis sort.

## Exercice 1.3

### Question 4

La pause peut être interrompue par des signaux. Le processus peut donc sortir de nanosleep s'il est interrompu
par l'ordonnanceur.

### Question 5

on peut faire les appels à srand() dans des blocks différents. En utilisant par exemple 
time(NULL) et getpid() pour assurer des graines différentes. 

### Question 6

étant donné que le fils attend d'être attaqué, il gagnera chaque combat .
