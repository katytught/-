//============================智宇科技========================================
//  智能小车红外遥控实验
//  实验中所用接收红外信号为配送遥控器的信号，也可打印出信号数值，配合其他红外信号控制
//  参考4.视频教程\Arduino套件视频\视频教程\例程19-红外遥控.mp4
//  本实验不可调节电机速度，调节pwm值会影响红外的信号接收
//=============================================================================
#include <IRremote.h>//包含红外库  关键点
int RECV_PIN = A4;//端口声明
IRrecv irrecv(RECV_PIN);
decode_results results;//结构声明
int on = 0;//标志位
unsigned long last = millis();
int Distance = 0;
int i=0;
int arr[5];
const int SensorRight_2 = 5;     //中间红外避障传感器()
int Echo = A1;  // Echo回声脚(P2.0)
int Trig =A0;  //  Trig 触发脚(P2.1)
int SR_2;
int beep=A3;//定义蜂鸣器 数字A3 接口

long run_car = 0x00FF629D;//按键CH
long back_car = 0x00FFA857;//按键+
long left_car = 0x00FF22DD;//按键<<
long right_car = 0x00FFC23D;//按键>||
long stop_car = 0x00FF02FD;//按键>>|
long left_turn = 0x00ffE01F;//按键-
long right_turn = 0x00FF906F;//按键EQ
long distance = 0x00FFA25D;//CH-
long avoid_type = 0x00FFE21D;//CH+

//==============================
int Left_motor_go=8;     //左电机前进(IN1)
int Left_motor_back=9;     //左电机后退(IN2)

int Right_motor_go=10;    // 右电机前进(IN3)
int Right_motor_back=11;    // 右电机后退(IN4)

void setup()
{
  //初始化电机驱动IO为输出方式
  pinMode(Left_motor_go,OUTPUT); // PIN 8 (PWM)
  pinMode(Left_motor_back,OUTPUT); // PIN 9 (PWM)
  pinMode(Right_motor_go,OUTPUT);// PIN 10 (PWM) 
  pinMode(Right_motor_back,OUTPUT);// PIN 11 (PWM)
  pinMode(13, OUTPUT);////端口模式，输出
  Serial.begin(115200);	//波特率9600
  irrecv.enableIRIn(); // Start the receiver
}
void run()     // 前进
{
   digitalWrite(Right_motor_go,HIGH);  // 右电机前进
   digitalWrite(Right_motor_back,LOW);     
  //analogWrite(Right_motor_go,200);//PWM比例0~255调速，左右轮差异略增减
  //analogWrite(Right_motor_back,0);
  digitalWrite(Left_motor_go,LOW);  // 左电机前进
  digitalWrite(Left_motor_back,HIGH);
  //analogWrite(Left_motor_go,0);//PWM比例0~255调速，左右轮差异略增减
  //analogWrite(Left_motor_back,200);
  //delay(time * 100);   //执行时间，可以调整 
}

void brake()         //刹车，停车
{
  digitalWrite(Right_motor_go,LOW);
  digitalWrite(Right_motor_back,LOW);
  digitalWrite(Left_motor_go,LOW);
  digitalWrite(Left_motor_back,LOW);
  //delay(time * 100);//执行时间，可以调整  
}

void left()         //左转(左轮不动，右轮前进)
{
  digitalWrite(Right_motor_go,HIGH);	// 右电机前进
  digitalWrite(Right_motor_back,LOW);
  //analogWrite(Right_motor_go,200); 
  //analogWrite(Right_motor_back,0);//PWM比例0~255调速
  digitalWrite(Left_motor_go,LOW);   //左轮后退
  digitalWrite(Left_motor_back,LOW);
  //analogWrite(Left_motor_go,0); 
  //analogWrite(Left_motor_back,0);//PWM比例0~255调速
  //delay(time * 100);	//执行时间，可以调整 
}

void spin_left()         //左转(左轮后退，右轮前进)
{
  digitalWrite(Right_motor_go,HIGH);	// 右电机前进
  digitalWrite(Right_motor_back,LOW);
  //analogWrite(Right_motor_go,200); 
  //analogWrite(Right_motor_back,0);//PWM比例0~255调速
  digitalWrite(Left_motor_go,HIGH);   //左轮后退
  digitalWrite(Left_motor_back,LOW);
  //analogWrite(Left_motor_go,200); 
  //analogWrite(Left_motor_back,0);//PWM比例0~255调速
  //delay(time * 100);	//执行时间，可以调整 
}

void right()        //右转(右轮不动，左轮前进)
{
  digitalWrite(Right_motor_go,LOW);   //右电机后退
  digitalWrite(Right_motor_back,LOW);
  //analogWrite(Right_motor_go,0); 
  //analogWrite(Right_motor_back,0);//PWM比例0~255调速
  digitalWrite(Left_motor_go,LOW);//左电机前进
  digitalWrite(Left_motor_back,HIGH);
  //analogWrite(Left_motor_go,0); 
 // analogWrite(Left_motor_back,200);//PWM比例0~255调速
 // delay(time * 100);	//执行时间，可以调整 
}

void spin_right()        //右转(右轮后退，左轮前进)
{
  digitalWrite(Right_motor_go,LOW);   //右电机后退
  digitalWrite(Right_motor_back,HIGH);
  //analogWrite(Right_motor_go,0); 
  //analogWrite(Right_motor_back,200);//PWM比例0~255调速
  digitalWrite(Left_motor_go,LOW);//左电机前进
  digitalWrite(Left_motor_back,HIGH);
  //analogWrite(Left_motor_go,0); 
  //analogWrite(Left_motor_back,200);//PWM比例0~255调速
  //delay(time * 100);	//执行时间，可以调整
}

void back()          //后退
{
  digitalWrite(Right_motor_go,LOW);  //右轮后退
  digitalWrite(Right_motor_back,HIGH);
  //analogWrite(Right_motor_go,0);
  //analogWrite(Right_motor_back,150);//PWM比例0~255调速
  digitalWrite(Left_motor_go,HIGH);  //左轮后退
  digitalWrite(Left_motor_back,LOW);
  //analogWrite(Left_motor_go,150);
  //analogWrite(Left_motor_back,0);//PWM比例0~255调速
  //delay(time * 100);     //执行时间，可以调整    
}

void dump(decode_results *results)
{
  int count = results->rawlen;
  if (results->decode_type == UNKNOWN) 
  {
    //Serial.println("Could not decode message");
    brake();
  } 
//串口打印，调试时可以打开，实际运行中会影响反应速度，建议屏蔽
/*
  else 
  {

    if (results->decode_type == NEC) 
    {
      Serial.print("Decoded NEC: ");
    } 
    else if (results->decode_type == SONY) 
    {
      Serial.print("Decoded SONY: ");
    } 
    else if (results->decode_type == RC5) 
    {
      Serial.print("Decoded RC5: ");
    } 
    else if (results->decode_type == RC6) 
    {
      Serial.print("Decoded RC6: ");
    }
    Serial.print(results->value, HEX);
    Serial.print(" (");
    Serial.print(results->bits, DEC);
    Serial.println(" bits)");
    
  }
  Serial.print("Raw (");
  Serial.print(count, DEC);
  Serial.print("): ");

  for (int i = 0; i < count; i++) 
  {
    if ((i % 2) == 1) 
    {
      Serial.print(results->rawbuf[i]*USECPERTICK, DEC);
    } 
    else  
    {
      Serial.print(-(int)results->rawbuf[i]*USECPERTICK, DEC);
    }
    Serial.print(" ");
  }
  Serial.println("");
*/
}
void Distance_test()   // 量出前方距离 
{
  digitalWrite(Trig, LOW);   // 给触发脚低电平2μs
  delayMicroseconds(2);
  digitalWrite(Trig, HIGH);  // 给触发脚高电平10μs，这里至少是10μs
  delayMicroseconds(10);
  digitalWrite(Trig, LOW);    // 持续给触发脚低电
  float Fdistance = pulseIn(Echo, HIGH);  // 读取高电平时间(单位：微秒)
  Fdistance= Fdistance/58;       //为什么除以58等于厘米，  Y米=（X秒*344）/2
  Distance = Fdistance;
  arr[i]=Distance;
}  

void Distance_display()//显示距离
{
  int resull=0;
  for(int j=0;j<4;j++){
    resull+=arr[j];
  }
  Serial.print(resull/5);
  Serial.print("\r\n");
}

void test_dis(){
  while(i<5)
  {
    Distance_test();//测量前方距离
    delay(200);
    i++;
    if(i==4){
      Distance_display();//液晶屏显示距离
      i=0;
    }
  }
}
void Robot_Avoidance()                   //机器人避障子程序
{
 //有信号为LOW  没有信号为HIGH
    SR_2 = digitalRead(SensorRight_2);
    if (SR_2==HIGH)//前面没有障碍物
    {
      run();   //调用前进函数
      //digitalWrite(LED,LOW);    //LED不亮
    } 
    else if ( SR_2 == LOW)// 前面探测到有障碍物，有信号返回，向左转 
       { 
//         digitalWrite(beep,HIGH);   //蜂鸣器响
         //digitalWrite(LED,HIGH);    //LED亮
         brake();//停止200MS
         delay(300);
         back();//后退500MS
         delay(400);
         left();//调用左转函数  延时500ms 
         delay(500);        
     }
}
void loop()
{
  if (irrecv.decode(&results)) //调用库函数：解码
  {
    // If it's been at least 1/4 second since the last
    // IR received, toggle the relay
    if (millis() - last > 250) //确定接收到信号
    {
      on = !on;//标志位置反
      digitalWrite(13, on ? HIGH : LOW);//板子上接收到信号闪烁一下led
      dump(&results);//解码红外信号
    }
    if (results.value == run_car )//按键CH
      run();//前进
    if (results.value == back_car )//按键+
      back();//后退
    if (results.value == left_car )//按键<<
      left();//左转
    if (results.value == right_car )//按键>||
      right();//右转
    if (results.value == stop_car )//按键>>|
      brake();//停车
    if (results.value == left_turn )//按键-
      spin_left();//左旋转
    if (results.value == right_turn )//按键EQ
      spin_right();//右旋转
    if (results.value == distance){
      test_dis();//测距
    }
    if(results.value == avoid_type){
      Robot_Avoidance(); // 调用避障子程序
    }
    last = millis();      
    irrecv.resume(); // Receive the next value
  }
}
