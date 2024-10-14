# TME 4 

## Question 2 : 

Le transfert (la méthode transfert de la classe banque) n'est pas une opération atomique. On peut débiter un compte et le compte 
que l'on souhaite créditer peut être accedé avant qu'on ne le crédite. 

Cela peut poser un problème dans la méthode débiter. Une opération peut ne pas être effectuée car un compte 
n'a "pas encore" été crédité.
```cpp 
bool doit = solde >= val;
	if (doit) {
		solde-=val ;
	} 
``` 

## Question 4 :

Lorsque l'on souhaite effectuer un transfert, on verouille les mutex associés aux 2 comptes que l'on souhaite manipuler, 
on effectue le transfert de manière atomique et on les libère. 

## Question 5 :    
  
Le programme se bloque si l'on essaie de transférer d'un compte vers celui-ci, i.e si l'on essaie de verouiller 2 fois 
le même compte.

## Question 6 :

Soit t1, qui effectue un transfert de C1 vers C2 
soit t2, qui effectue un transfert de C2 vers C1 

On suppose que t1 prend le verrou sur C1 et t2 le verrou sur C2. 
On voit que l'on se trouve dans une situation de deadlock.  


## Question 7 : 

La méthode pour comptabiliser n'est actuellement pas synchronisée avec les transactions, si le thread comptable accède à  
un compte après qu'il soit débité et au compte suivant avant qu'il soit crédité, il y aura une faute. Afin de synchroniser 
un thread comptable, il faut garantir l'exclusion des threads de transfert et du thread comptable. 
 
