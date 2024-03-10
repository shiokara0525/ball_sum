#include <Arduino.h>
#include<timer.h>
#include<MA.h>

int ballPin[16] = {10,2,14,15,16,17,18,19,11,3,4,5,6,7,8,9};
double ang;
int ball_num[16];
int ball_g[2];
uint8_t ball_8bit[16];
uint8_t ball_get_8[2];
double Sin[16];
double Cos[16];
int16_t x,y;
int ball_get;
timer Timer_ball;
timer Timer;
int Time;
MA ma;
int A;
void ball();
void ball_print();
void led();
int LED = 13;
int Serial_flag = 0; //0だったらメインに通信　1だったらデバッグ
//デジタル12番

void setup() {
  Serial.begin(115200);
  ma.setLenth(10);
  for(int i = 0; i < 16; i++){
    Sin[i] = sin(radians(22.5 * i));
    Cos[i] = cos(radians(22.5 * i));
  }
  DDRB &= ~(_BV(0) | _BV(1) | _BV(2) | _BV(3));
  DDRC &= ~(_BV(1) | _BV(1) | _BV(2) | _BV(3) | _BV(4) | _BV(5));
  DDRD &= ~(_BV(2) | _BV(3) | _BV(4) | _BV(5) | _BV(6) | _BV(7));
}

void loop(){
  ball();
  int sendBuf_int[3];
  byte sendBuf_byte[6];
  //データを格納
  sendBuf_int[1] = x;
  sendBuf_int[2] = y;
  //データをバイトに直す
  sendBuf_byte[0] = 0xFF;
  sendBuf_byte[1] = byte( sendBuf_int[1] >> 8 ); //ビットシフトで上位側の８Bitを取り出し、バイト型に型変換をする。
  sendBuf_byte[2] = byte( sendBuf_int[1] & 0xFF ); //論理和で下位側の８Bitを取り出し、バイト型に型変換をする。
  sendBuf_byte[3] = byte( sendBuf_int[2] >> 8 ); //ビットシフトで上位側の８Bitを取り出し、バイト型に型変換をする。
  sendBuf_byte[4] = byte( sendBuf_int[2] & 0xFF ); //論理和で下位側の８Bitを取り出し、バイト型に型変換をする。
  sendBuf_byte[5] = A;
  sendBuf_byte[6] = 0xAA;
  // ６バイトのデータ送信
  // delayMicroseconds(1000);
  // ball_print();
  Serial.write(sendBuf_byte, 7);
}

void ball_print(){
  Time = Timer.read_ms();
  // Serial.print(" x : ");
  // Serial.print(x);
  // Serial.print(" y : ");
  // Serial.print(y);
  Serial.print(" time : ");
  Serial.print(Time);
  Serial.print(" ang : ");
  Serial.print(degrees(atan2(y,x)));
  Serial.print(" 0 : ");
  Serial.print(ball_g[0]);
  Serial.print(" 1 : ");
  Serial.print(ball_g[1]);
  Serial.print(" A : ");
  Serial.print(A);
  Serial.print(" get : ");
  Serial.print(ball_get);
  // for(int i = 0; i < 16; i++){
  //   Serial.print(" ");
  //   Serial.print(ball_num[i]);
  // }
  Serial.println();
  Timer.reset();
}

void ball() {
  double ball_x = 0;
  double ball_y = 0;
  ball_g[0] = 0;
  ball_g[1] = 0;
  for(int i = 0; i < 16; i++){
    ball_num[i] = 0;
  }
  int best_num;
  int best_val = 0;

  Timer_ball.reset();

  for(int i = 0; i < 100; i++){
    ball_8bit[0] = PINB & _BV(2);
    ball_8bit[1] = PIND & _BV(2);
    ball_8bit[2] = PINC & _BV(0);
    ball_8bit[3] = PINC & _BV(1);
    ball_8bit[4] = PINC & _BV(2);
    ball_8bit[5] = PINC & _BV(3);
    ball_8bit[6] = PINC & _BV(4);
    ball_8bit[7] = PINC & _BV(5);
    ball_8bit[8] = PINB & _BV(3);
    ball_8bit[9] = PIND & _BV(3);
    ball_8bit[10] = PIND & _BV(4);
    ball_8bit[11] = PIND & _BV(5);
    ball_8bit[12] = PIND & _BV(6);
    ball_8bit[13] = PIND & _BV(7);
    ball_8bit[14] = PINB & _BV(0);
    ball_8bit[15] = PINB & _BV(1);

    ball_get_8[0] = PINB & _BV(5);
    ball_get_8[1] = PINB & _BV(4);

    for(int i = 0; i < 16; i++){
      if(ball_8bit[i] == 0){
        ball_num[i]++;
      }
    }
    if(ball_get_8[0] == 0){
      ball_g[0]++;
    }
    if(ball_get_8[1] == 0){
      ball_g[1]++;
    }
  }

  for(int i = 0; i < 16; i++){
    if(ball_num[i] == 100){
      ball_num[i] = 20;
    }
    if(best_val < ball_num[i]){
      best_val = ball_num[i];
      best_num = i;
    }
  }

  for(int i = -4; i <= 4; i++){
    int num = best_num + i;
    if(num < 0){
      num += 16;
    }
    else if(15 < num){
      num -= 16;
    }
    if(ball_num[num] == 250){
      ball_num[num] = 1000;
    }
    ball_x += ball_num[num] * Cos[num];
    ball_y += ball_num[num] * Sin[num];
  }

  // A = ma.demandAve(ball_g[0] + ball_g[1]);
  A = ball_g[0] + ball_g[1];
  if(70 < A){
    ball_get = 1;
  }
  else if(60 < A){
    ball_get = 2;
  }
  else{
    ball_get = 0;
  }
  x = -ball_x;
  y = -ball_y;
}


void led(){
  digitalWrite(LED,HIGH);
  delay(500);
  digitalWrite(LED,LOW);
  delay(500);
}