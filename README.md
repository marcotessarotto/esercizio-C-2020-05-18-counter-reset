# esercizio-C-2020-05-18-counter-reset


un processo crea due thread

```
#define LIMIT 100000
#define RESET_LIMIT 100
```
variabili globali:

```
 int counter;
 int reset;
```

usare mutex per regolare l'accesso concorrente alle variabili globali


thread1 e thread2 fanno questo:
```  
   counter++;
   if (counter > LIMIT) {
      counter = counter - LIMIT;
      reset++;
     }
     
   if (reset > RESET_LIMIT)
     ...
     // thread termina
``` 
   
il main thread dopo aver avviato i due thread, aspetta la conclusione dei due thread poi termina.
