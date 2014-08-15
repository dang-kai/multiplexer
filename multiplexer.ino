#define uchar unsigned char
#define uint unsigned int

// Pins
//chip select pins
const int C0 = 17;
const int C1 = 18;
const int C2 = 19;
//channel address pins
//read function channelSelect before change pin assignment
const int S0   =  4;
const int S1   =  5;
const int S2   =  6;
const int S3   =  7;
const int LED  = 13;
//const channels
const char CH_VCC = 47;
const char CH_GND = 46;

//global variables
char c_channel=-1; //current selected channel

// !!! unselect before select
void chipSelect(char chip)
{
  switch(chip)
  {
  case 0:
    digitalWrite(C0,LOW);
    break;
  case 1:
    digitalWrite(C1,LOW);
    break;
  case 2:
    digitalWrite(C2,LOW);
    break;
  default:
    digitalWrite(C0,HIGH);
    digitalWrite(C1,HIGH);
    digitalWrite(C2,HIGH);
    break;
  }
}

//channel number 0~47
char channelSelect(char channel)
{
  if(channel<0||channel>47)
  {
    return -1;
  }
//  if(c_channel==channel)
//  {
//    return 0;
//  }

  char chip=channel/16;
  char pin=channel%16;

  //unselect all chips first
  chipSelect(-1);
  //only works for S0-S3 = pin 4-7 
  char i;
  for(i=0;i<4;++i)
  {
    pin%2?digitalWrite(i+4,HIGH):digitalWrite(i+4,LOW);
    pin/=2;
  }
  chipSelect(chip);
  c_channel=channel;
  return 0;
}

//send binary stream throuth common output using VCC and GND
void sendBinUchar(uchar data,int d)
{
  uchar i,bins[8];
  for(i=0;i<8;++i)
  {
    bins[i]=(data>>(7-i))%2?1:0;
  }
  for(i=0;i<8;++i)
  {
    bins[i]?channelSelect(CH_VCC):channelSelect(CH_GND);
    delay(d);
  }
}

//channel
void channelSelectH(char ch,int d)
{
  sendBinUchar((uchar)ch,d);
  channelSelect(ch);
}

// the setup routine runs once when you press reset:
void setup() 
{         
  pinMode(C0,OUTPUT);
  pinMode(C1,OUTPUT);
  pinMode(C2,OUTPUT);     
  pinMode(S0,OUTPUT);     
  pinMode(S1,OUTPUT);     
  pinMode(S2,OUTPUT);     
  pinMode(S3,OUTPUT);
  pinMode(LED,OUTPUT); 
  chipSelect(-1);
}

// the loop routine runs over and over again forever:
// variables for loop
char ch=0;
void loop() 
{
  channelSelectH(ch++,20);
  delay(100);
  digitalWrite(LED,HIGH);
  delay(100);
  digitalWrite(LED,LOW);
  ch%=47;
}


