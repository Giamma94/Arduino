//installato su due A1R3 collegati tra loro via EIA RS485
//rende possibile l'invio di caratteri ASCII da tastiera del PC
//tra l'uno e l'altro in modo bidirezionale HALF DUPLEX (linea bilanciata con MAX485)
//Ciascun Arduino è collegato al proprio PC sul monitor dell'IDE
//via USB.


#include <SoftwareSerial.h> // per gestione seriale su pins digitali diversi da Rx e Tx 

#define pinTxRx 4 // Out D4 controlla direzione 
#define pinTx 3   // Out D3 per trasmettere
#define pinRx 2   // Out D2 per ricevere
#define TX 1
#define RX 0

SoftwareSerial rs485(pinRx, pinTx); // classe/oggetto-nome-I/O interessati

boolean flag;
int nch;
char str[7]; //definisci vettore per stringa in arrivo

void setup(){
  Serial.begin(9600); // Baudrate della seriale verso PC via USB
  rs485.begin(9600);  // Baudrate della seriale oggetto rs485 
  pinMode(pinTxRx,OUTPUT);    // il pin D4 sarà OUTPUT
  digitalWrite(pinTxRx, RX);  // poni il MAX485 in ricezione
}

void loop()
{
  nch=0;
  flag=true;
  digitalWrite(pinTxRx, TX);   //poni il MAX485 in trasmissione (Tx)    
  while(flag)
     {
        if(Serial.available()>0)                //se c'è un carattere nel buffer
          {
            if ((str[nch]=Serial.read())!= '>') //controlla che non sia il fine stringa stabilito
               {                                //e riempi il vettore stringa in arrivo
                 nch++;                         //calcola la lunghezza del vettore stringa in arrivo
               }
            else flag=false;                    //se leggi il fine stringa esci dal while
          }
     }
  if(nch==6)                            //se le lunghezze dei vettori coincidono
    {
       if((str[3]=='1')||(str[3]=='2'))
       {         
        rs485.println(str);  //spedisce il comando
        digitalWrite(pinTxRx, RX);   //torna il MAX485 in ricezione (Rx)
       }
       else Serial.println("Comando inserito non valido \nstringa tipo: <Sn_X>");      //se i caratteri NON coincidono 
    }
  else Serial.println("Comando inserito non valido \nstringa tipo: <Sn_X>");          //se le lunghezze dei vettori NON coincidono  
  
  
  if(rs485.available()>0) // se la seriale sul MAX485 contiene qualcosa (se si è ricevuto un carattere dalla rete 485)
    {          
     char risp[18];
     int k;
     
     k=0;
     while(rs485.available()>0)
     {
       risp[k]=rs485.read();
       k++;
     }
     if(str[3]==risp[3]) //controllo se ha risposto lo slave giusto
     {
       Serial.print("Slave ");
       Serial.print(str[2]);
       
       if((risp[4]==str[4])&&(risp[4]=='A')) //ALL
       {
         Serial.print("\nfrequenza: ");
         for(k=6;k<12;k++)
         {
           Serial.print(risp[k]);
         } 
         Serial.print("\nLuce: ");
         for(k=13;k<=16;k++)
         {
           Serial.print(risp[k]);
         }        
       }
       if((risp[5]==str[5])&&(risp[5]=='F'))  //FREQUENZA
       {
         Serial.print("\nfrequenza: ");
         for(k=6;k<12;k++)
         {
           Serial.print(risp[k]);
         }
       }
       if((risp[5]==str[5])&&(risp[5]=='L'))  //LUCE
       {
         Serial.print("\nLuce: ");
         for(k=7;k<=10;k++)
         {
           Serial.print(risp[k]);
         }
       }
     }
    }    
}  
