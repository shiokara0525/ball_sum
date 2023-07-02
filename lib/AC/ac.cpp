#include<ac.h>



double AC::getAC_val(){  //姿勢制御の値返す関数
  double kkp = 0;  //比例制御の値
  double kkd = 0;  //積分制御の値
  double kk_i = 0;

  bno.getEvent(&event);  //方向チェック
  
  dir = event.orientation.x - dir_target;  //現在の方向を取得
  
  if(dir > 180){
    dir -= 360;  //方向を0~360から-180~180に変換
  }

  kkp = -dir;  //比例制御の値を計算
  kkd = -(kkp - kkp_old);  //微分制御の値を計算
  
  val = kkp * kp + kkd * kd;  //最終的に返す値を計算

  Serial.print(" pゲイン : ");
  Serial.print(kkp * kp);
  Serial.print(" dゲイン : ");
  Serial.print(kkd * kd);
  Serial.print(" iゲイン : ");
  Serial.print(kk_i * ki);
  

  kkp_old = kkp;  //前Fの方向を更新

  return val;  //値返す
}



float AC::getnowdir(){
  bno.getEvent(&event);
  dir = event.orientation.x - dir_target;
  if(180 < abs(dir)){
    dir += (dir < 0 ? 360 : -360);
  }
  
  return dir;
}


void AC::print(){  //現在の角度、正面方向、姿勢制御の最終的な値を表示
  Serial.print(" 角度 : ");
  Serial.print(dir);
  Serial.print(" 正面方向 : ");
  Serial.print(dir_target);
}




void AC::setup(){  //セットアップ
  kki.setLenth(1000);
  bno.begin();
  bno.getEvent(&event);  //方向入手
  delay(100);
  bno.getEvent(&event);

  if(event.orientation.x > 180){
    event.orientation.x -= 360;  //方向を0~360から-180~180に変換
  }

  dir_target = event.orientation.x;  //正面方向決定
}

void AC::setup_2(){
  bno.getEvent(&event);

   if(event.orientation.x > 180){
    event.orientation.x -= 360;  //方向を0~360から-180~180に変換
  }
  dir_target = event.orientation.x;
}