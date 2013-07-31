#define exitFailSafe 3
#define motorForward 5
#define motorReverse 6
#define motorForward7b 9
#define motorReverse7b 10
#define motorBrake 18
#define motorStop 96

#define resetPin1 22
#define errorPin1 23

#define resetPin2 24
#define errorPin2 25

#define MIDPOINT 1

#define MAXPOWER 1500
#define MAXDIR 1200

#define FWDPIN 8
#define BWDPIN 7
#define LPIN 6
#define RPIN 5

#include <SPI.h>
#include <Ethernet.h>

boolean Pressed=false;

byte mac[] = { 
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192,168,2,100);
EthernetServer server(80);

int movePower=0;
int dirPower=0;

int movePressed=0;
int dirPressed=0;

int fPower=0;
int lDir=0;
  
int moveMul=0;
int dirMul=0;

String a1="", a2="";

byte move=0, dir=0;
byte dmove=0, ddir=0;

void init_ethernet()
{
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
  Serial.print("\n");
}

void setup()
{
  Serial.begin(9600);
  init_ethernet();
}

void loop()
{
  
  EthernetClient client = server.available();
  if (client)
  {
    Serial.println("new client");
    while (client.connected()) {
      
      if(client.available()) 
      {
        
        Serial.write('New\n');
        char inByte=0;
        
        //get 
        byte command = client.read();
        
        switch(command)
        {
          case 0:
            break;
            
          case 1:
            move = client.read();
            dir = client.read();
            
            if(move!=MIDPOINT)
            {
              if(movePower<400)
                movePower=400;
              movePressed=1;
            }
            else
            {
              movePressed=0;
            }

            if(dir!=MIDPOINT)
            {
              if(dirPower<400)
                dirPower=400;
              dirPressed=1;
            }
            else
            {
              dirPressed=0;
            }
            
            break;
          case 2:
            move = client.read();
            dir = client.read();
            movePower=0;
            dirPower=0;
            
            break;
            
          case 3:
            //client.print();
            //client.print();
            
          default:
            ;
        }
        
        continue;
      }
    }
    Serial.println("client disconected");
  }
}



