#include <Arduino.h>
#include<timer.h>
#include<ac.h>

int ballPin[16] = {10,2,14,15,16,17,18,19,11,3,4,5,6,7,8,9};
double ang;
int ball_num[16];
uint8_t ball_8bit[16];
double Sin[16];
double Cos[16];
int x,y;
timer Timer_ball;
void ball();
AC ac;
//デジタル12番

void setup() {
  Serial.begin(9600);
  ac.setup();
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
  //ボールのあるベクトルのデータ送るところ
  //x、yは元々-127~127だったものに127を足して0~254の値にしてあり、受信側で元のデータに戻してあります。
  Serial.write(254);
  delayMicroseconds(10);
  Serial.write(x);
  delayMicroseconds(10);
  Serial.write(y);
  delayMicroseconds(500);
}


void ball() {
  double ball_x = 0;
  double ball_y = 0;
  for(int i = 0; i < 16; i++){
    ball_num[i] = 0;
  }
  int best_num;
  int best_val = 0;

  Timer_ball.reset();

  for(int i = 0; i < 1000; i++){
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

    for(int i = 0; i < 16; i++){
      if(ball_8bit[i] == 0){
        ball_num[i]++;
      }
    }
  }

  for(int i = 0; i < 16; i++){
    if(best_val < ball_num[i]){
      best_val = ball_num[i];
      best_num = i;
    }
  }

  for(int i = -2; i <= 2; i++){
    int num = best_num + i;
    if(num < 0){
      num += 16;
    }
    else if(16 < num){
      num -= 16;
    }
    ball_x += ball_num[num] * Cos[num];
    ball_y += ball_num[num] * Sin[num];
  }

  ball_x *= 0.05;
  ball_y *= 0.05;
  if(127 < abs(ball_x)){  //データが127を超えたら127にする
    ball_x = (ball_x < 0 ? -127 : 127);
  }
  if(127 < abs(ball_y)){
    ball_y = (ball_y < 0 ? -127 : 127);
  }
  x = ball_x + 127;  //UARTで送るためにデータを正にする
  y = ball_y + 127;  ////UARTで送るためにデータを正にする
}



// void serialEvent(){
//   Serial.write(38);
// }