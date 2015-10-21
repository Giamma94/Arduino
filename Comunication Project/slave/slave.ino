#include <SoftwareSerial.h> // per gestione seriale su pins digitali diversi da Rx e Tx 
#include <TimerOne.h>
#define pinTxRx 4 // Out D4 controlla direzione 
#define pinTx 3   // Out D3 per trasmettere
#define pinRx 2   // Out D2 per ricevere
#define TX 1
#define RX 0

SoftwareSerial rs485(pinRx, pinTx); // classe/oggetto-nome-I/O interessati


int valore; 

boolean flag;
char parola[63];
char str[63];
int nch;
volatile unsigned long pulse_counter = 0;
volatile unsigned long freq = 0;
unsigned long oldfreq = 0;
boolean rs485_print=LOW;
void setup()
{
  rs485.begin(9600);  // Baudrate della seriale oggetto rs485 
  pinMode(pinTxRx,OUTPUT);    // il pin D4 sarà OUTPUT
  digitalWrite(pinTxRx, RX);  // poni il MAX485 in ricezione
  pinMode(2,INPUT);
  Timer1.initialize(1000000);           
  Timer1.attachInterrupt(timeoff);       
  attachInterrupt(0, pulse_in , RISING);
}
void loop()
{
   
 flag = true;  
nch=0; 
while(flag)
     {
        if(rs485.available()>0)                //se c'è un carattere nel buffer
          {
            if ((str[nch]=rs485.read())!= '>') //controlla che non sia il fine stringa stabilito
               {                                //e riempi il vettore stringa in arrivo
                 nch++;                         //calcola la lunghezza del vettore stringa in arrivo
               }
            else flag=false;                    //se leggi il fine stringa esci dal while
          }
     }
  
 
  if(str[3]='1')
  {if(str[5]='F')
    {if(s_print)
  {
    if ((freq > oldfreq+1)||(freq < oldfreq-1)||(freq==0))
    {
      
      oldfreq=freq;
    }
    
    rs485_print=LOW;      //disabilita l'invio su seriale
  }
  
     digitalWrite(pinTxRx, TX);   //poni il MAX485 in trasmissione (Tx)
     parola[0]='<';
     parola[1]='S';
     parola[2]='0';
     parola[3]='1';
     parola[4]='_';
     parola[5]='F';
     parola[6]='_';
     parola[7]=freq/100000;
     parola[8]=freq-(parola[7]*100000);
     parola[9]=(freq-(parola[7]*100000)-(parola[8]*10000))/1000;
     parola[10]=(freq-(parola[7]*100000)-(parola[8]*10000)-(parola[9]*1000))/100;
     parola[11]=(freq-(parola[7]*100000)-(parola[8]*10000)-(parola[9]*1000)-(parola[10]*100))/10;
     parola[12]=(freq-(parola[7]*100000)-(parola[8]*10000)-(parola[9]*1000)-(parola[10]*100)-(parola[11]*10);
     parola[13]='>';
   
     rs485.println(parola);  
   digitalWrite(pinTxRx, RX);   //torna il MAX485 in ricezione (Rx)
  }
  if(str[5]='L')
  { 
    parola[0]='<';
     parola[1]='S';
     parola[2]='0';
     parola[3]='1';
     parola[4]='_';
     parola[5]='F';
     parola[6]='_';
     parola[7]=lux/1000;
     parola[8]=lux-(parola[7]*1000);
     parola[9]=(lux-(parola[7]*1000)-(parola[8]*100))/10;
     parola[10]=(lux-(parola[7]*1000)-(parola[8]*100)-(parola[9]*10));
     parola[11]='>';
     rs485.println(parola);  
    digitalWrite(pinTxRx, RX);   //torna il MAX485 in ricezione (Rx)
    }
    if(str[5]='A')
    {if(s_print)
  {
    if ((freq > oldfreq+1)||(freq < oldfreq-1)||(freq==0))
    {
      
      oldfreq=freq;
    }
    
    rs485_print=LOW;      //disabilita l'invio su seriale
  }
  
  lux=analogRead(0);  
  parola[0]='<';
     parola[1]='S';
     parola[2]='0';
     parola[3]='1';
     parola[4]='_';
     parola[5]='F';
     parola[6]='_';
     parola[7]=freq/100000;
     parola[8]=freq-(parola[7]*100000);
     parola[9]=(freq-(parola[7]*100000)-(parola[8]*10000))/1000;
     parola[10]=(freq-(parola[7]*100000)-(parola[8]*10000)-(parola[9]*1000))/100;
     parola[11]=(freq-(parola[7]*100000)-(parola[8]*10000)-(parola[9]*1000)-(parola[10]*100))/10;
     parola[12]=(freq-(parola[7]*100000)-(parola[8]*10000)-(parola[9]*1000)-(parola[10]*100)-(parola[11]*10);
     parola[13]='_';
     parola[14]=lux/1000;
     parola[15]=lux-(parola[7]*1000);
     parola[16]=(lux-(parola[7]*1000)-(parola[8]*100))/10;
     parola[17]=(lux-(parola[7]*1000)-(parola[8]*100)-(parola[9]*10));
     parola[18]='>';
     rs485.println(parola);  
     digitalWrite(pinTxRx, RX);   //torna il MAX485 in ricezione (Rx)
      }
  
 }
}
void timeoff()
{
  detachInterrupt(0);
  freq=pulse_counter;                      //salva conteggio fronti di salita su D2
  // correggi un po il valore di freq
  //if(freq >= 1000 && freq < 10000){ freq=freq+1;}
  //else if(freq >= 10000 && freq < 100000) {freq=freq+5;}
  //else if(freq >= 100000) {freq=freq+50;}
  pulse_counter=0;
  s_print=HIGH;                            //abilita l'invio del conteggio sulla seriale
  attachInterrupt(0, pulse_in , RISING);
}
 
void pulse_in()
{
  pulse_counter = pulse_counter + 1;      //incrementa ad ogni fronte di salita su D2
}

