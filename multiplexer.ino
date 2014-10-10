#define uchar unsigned char
#define uint unsigned int

// Pins (Serial communication is no longer avaliable)
//chip select pins
const int C0 = 7;
const int C1 = 6;
const int C2 = 5;
//channel address pins
//read function channelSelect before change pin assignment
const int S0   =  3;
const int S1   =  2;
const int S2   =  1;
const int S3   =  0;
const int LED  = 13;
//const channels
const char CH_VCC = 47;
const char CH_VSS = 46;

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
  
  char chip=channel/16;
  char pin=channel%16;

  //unselect all chips first
  chipSelect(-1);
  
  char i;
  int addrPin[4]={S0,S1,S2,S3};
  for(i=0;i<4;++i)
  {
    pin%2?digitalWrite(addrPin[i],HIGH):digitalWrite(addrPin[i],LOW);
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
    bins[i]?channelSelect(CH_VCC):channelSelect(CH_VSS);
    delayMicroseconds(d);
  }
}

//channel
void channelSelectH(char ch,int d)
{
  channelSelect(CH_VCC);
  sendBinUchar((uchar)ch,d);
  channelSelect(ch);
}

void channelSelectB(char ch,int d)
{
  ch==0?channelSelect(CH_VSS):channelSelect(CH_VCC);
  delayMicroseconds(d);
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
  for(ch=26;ch<46;++ch)
  {
    channelSelectH(ch,0.5*1000);
    delay(1000);
    digitalWrite(LED,LOW);
    delay(1990);
    digitalWrite(LED,HIGH);
  }
  for(ch=0;ch<16;++ch)
  {
    channelSelectH(ch,0.5*1000);
    delay(1000);
    digitalWrite(LED,LOW);
    delay(1990);
    digitalWrite(LED,HIGH);
  }
}


