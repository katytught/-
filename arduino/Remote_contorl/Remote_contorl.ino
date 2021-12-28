#include <IRremote.h>//包含红外库  关键点
#include <Metro.h>
int RECV_PIN = A4;//端口声明
IRrecv irrecv(RECV_PIN);
decode_results results;//结构声明
int on = 0;//标志位
unsigned long last = millis();
int Distance = 0;
int i=0;
int beepstate=0;
int avoidstate=0;
int arr[5];
const int SensorRight_2 = 5;     //中间红外避障传感器()
int Echo = A1;  // Echo回声脚(P2.0)
int Trig =A0;  //  Trig 触发脚(P2.1)
int SR_2;
int beep=A3;//定义蜂鸣器 数字A3 接口
char res[] = "0x00FF629D\r\n";
char temp;
char comdata[] = "";//字符串函数
int mark;

long run_car = 0x00FF629D;//按键CH
long back_car = 0x00FFA857;//按键+
long left_car = 0x00FF22DD;//按键<<
long right_car = 0x00FFC23D;//按键>||
long stop_car = 0x00FF02FD;//按键>>|
long left_turn = 0x00ffE01F;//按键-
long right_turn = 0x00FF906F;//按键EQ
long distance = 0x00FFA25D;//CH-
long avoid_type = 0x00FFE21D;//CH+
long beepalarm = 0x00FF6897;//0
long left_test = 0x00FF9867;//100+
long right_test = 0x00FFB04F;//200+
long front_test = 0x00FF30CF;//
 
boolean blink1State = false;   //定义 blink1State 为false状态
Metro blink1Metro = Metro(100);   //把 blink1Metro 实例化 Metro 对象 ，并设置间隔时间

boolean blink2State = false;
Metro blink2Metro = Metro(50);

//==============================
int Left_motor_go=8;     //左电机前进(IN1)
int Left_motor_back=9;     //左电机后退(IN2)

int Right_motor_go=10;    // 右电机前进(IN3)
int Right_motor_back=11;    // 右电机后退(IN4)
int servopin=2;//设置舵机驱动脚到数字口2
int pulsewidth;//定义脉宽变量

void setup()
{
  //初始化电机驱动IO为输出方式
  pinMode(servopin,OUTPUT);//设定舵机接口为输出接口
  pinMode(Left_motor_go,OUTPUT); // PIN 8 (PWM)
  pinMode(Left_motor_back,OUTPUT); // PIN 9 (PWM)
  pinMode(Right_motor_go,OUTPUT);// PIN 10 (PWM) 
  pinMode(Right_motor_back,OUTPUT);// PIN 11 (PWM)
  pinMode(13, OUTPUT);////端口模式，输出
  pinMode(Echo, INPUT);    // 定义超声波输入脚
  pinMode(Trig, OUTPUT);   // 定义超声波输出脚
  pinMode(beep, OUTPUT);
  Serial.begin(115200);  //波特率9600
  pinMode(SensorRight_2, INPUT); //定义中间避障传感器为输入
  irrecv.enableIRIn(); // Start the receiver
}
void run()     // 前进
{
   digitalWrite(Right_motor_go,HIGH);  // 右电机前进
   digitalWrite(Right_motor_back,LOW);     
  digitalWrite(Left_motor_go,LOW);  // 左电机前进
  digitalWrite(Left_motor_back,HIGH);
}

void brake()         //刹车，停车
{
  digitalWrite(Right_motor_go,LOW);
  digitalWrite(Right_motor_back,LOW);
  digitalWrite(Left_motor_go,LOW);
  digitalWrite(Left_motor_back,LOW);
}

void right()         //左转(左轮不动，右轮前进)
{
  digitalWrite(Right_motor_go,HIGH);  // 右电机前进
  digitalWrite(Right_motor_back,LOW);
  digitalWrite(Left_motor_go,LOW);   //左轮后退
  digitalWrite(Left_motor_back,LOW);
}

void spin_left()         //左转(左轮后退，右轮前进)
{
  digitalWrite(Right_motor_go,HIGH);  // 右电机前进
  digitalWrite(Right_motor_back,LOW);
  digitalWrite(Left_motor_go,HIGH);   //左轮后退
  digitalWrite(Left_motor_back,LOW);
}

void left()        //右转(右轮不动，左轮前进)
{
  digitalWrite(Right_motor_go,LOW);   //右电机后退
  digitalWrite(Right_motor_back,LOW);
  digitalWrite(Left_motor_go,LOW);//左电机前进
  digitalWrite(Left_motor_back,HIGH);
}

void spin_right()        //右转(右轮后退，左轮前进)
{
  digitalWrite(Right_motor_go,LOW);   //右电机后退
  digitalWrite(Right_motor_back,HIGH);
  digitalWrite(Left_motor_go,LOW);//左电机前进
  digitalWrite(Left_motor_back,HIGH);
}

void back()          //后退
{
  digitalWrite(Right_motor_go,LOW);  //右轮后退
  digitalWrite(Right_motor_back,HIGH);
  digitalWrite(Left_motor_go,HIGH);  //左轮后退
  digitalWrite(Left_motor_back,LOW); 
}

void dump(decode_results *results)
{
  int count = results->rawlen;
  if (results->decode_type == UNKNOWN) 
  {
    //Serial.println("Could not decode message");
    brake();
  } 
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
    }
  }
}
void Robot_Avoidance()                   //机器人避障子程序
{
  Distance_test();
  
}
void left_detection()
{
  Serial.println("in");
  for(int j=0;j<=15;j++) //产生PWM个数，等效延时以保证能转到响应角度
  {
    servopulse(servopin,125);//模拟产生PWM
  }
}

void right_detection()
{
  for(int j=0;j<=15;j++) //产生PWM个数，等效延时以保证能转到响应角度
  {
    servopulse(servopin,5);//模拟产生PWM
  }
}
void front_detection()
{
  //此处循环次数减少，为了增加小车遇到障碍物的反应速度
  for(int i=0;i<=5;i++) //产生PWM个数，等效延时以保证能转到响应角度
  {
    servopulse(servopin,65);//模拟产生PWM
  }
}
void servopulse(int servopin,int myangle)/*定义一个脉冲函数，用来模拟方式产生PWM值舵机的范围是0.5MS到2.5MS 1.5MS 占空比是居中周期是20MS*/ 
{
  pulsewidth=(myangle*11)+500;//将角度转化为500-2480 的脉宽值 这里的myangle就是0-180度  所以180*11+50=2480  11是为了换成90度的时候基本就是1.5MS
  Serial.println(pulsewidth);
  digitalWrite(servopin,HIGH);//将舵机接口电平置高                                      90*11+50=1490uS  就是1.5ms
  delayMicroseconds(pulsewidth);//延时脉宽值的微秒数  这里调用的是微秒延时函数
  digitalWrite(servopin,LOW);//将舵机接口电平置低
 // delay(20-pulsewidth/1000);//延时周期内剩余时间  这里调用的是ms延时函数
  delay(20);//延时周期内剩余时间  这里调用的是ms延时函数
}
int count=0;
void loop()
{
  if(Serial.available())
  {
    count=0;
      digitalWrite(7,HIGH);
      String comdata = "";//缓存清零
      while (Serial.available() > 0)//循环串口是否有数据
      {
        temp = char(Serial.read());//叠加数据到comdata
        comdata+=temp;
        count++;
        delay(2);//延时等待响应
      } 
      Serial.print(count);
      Serial.print("\r\n");
      if((count)==5){
        run();//前进
      }
      if((count)==6){
        back();//后退
      }
      if((count)==7){
        left();//左转
      }
      if((count)==8){
        right();//右转
      }
      if((count)==9){
        brake();//停车
      }
      if((count)==10){
        i=0;
        test_dis();//测距
      }
      if((count)==11){
        avoidstate=!avoidstate;
        if(avoidstate==0){
          brake();
        }
      }
      if((count)==12){
        beepstate=!beepstate;
      }
      if((count)==13){
        front_detection();
      }
      if((count)==14){
        left_detection();
      }
      if((count)==15){
        right_detection();
      }
      if((count)==16){
        
      }
      if(mark==1){
        digitalWrite(7,HIGH);
        delay(200);
        digitalWrite(7,LOW);
      }
   }
   if(!Serial.available()){
    digitalWrite(7,LOW);
   }
  if(blink1Metro.check()){  //检查blink1Metro间隔(我的理解是计时器每隔100毫秒就会返回true，就执行以下程序)
    blink1State = !blink1State;   //反转blink1State的逻辑真或假（false or true）
    if(beepstate){
      if(digitalRead(beep)==1){
        digitalWrite(beep,LOW);
      }
      else{
        digitalWrite(beep,HIGH);
      }
    }
  }
  if(blink2Metro.check()){  //检查blink1Metro间隔(我的理解是计时器每隔100毫秒就会返回true，就执行以下程序)
    blink2State = !blink2State;   //反转blink1State的逻辑真或假（false or true）
    if(avoidstate){
        //有信号为LOW  没有信号为HIGH  有障碍物输出0  没有障碍物输出1
        SR_2 = digitalRead(SensorRight_2);
        if (SR_2==HIGH)//前面没有障碍物
        {
          run();   //调用前进函数
        } 
        else if ( SR_2 == LOW)// 前面探测到有障碍物，有信号返回 
           { 
             brake();//停止300MS
             delay(200);
             back();//后退400MS
             delay(400);
             left();//调用左转函数  延时500ms     
             delay(500);    
         }
          
    }
  }
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
      digitalWrite(7,HIGH);
      delay(200);
      digitalWrite(7,LOW);
      delay(200);
      i=0;
      test_dis();//测距
    }
    if(results.value == avoid_type){
      avoidstate=!avoidstate;
      if(avoidstate==0){
        brake();
      }
    }
    if(results.value == beepalarm){
      beepstate=!beepstate;
    }
    if(results.value == left_test){
      left_detection();
    }
    if(results.value == right_test){
      right_detection();
    }
    if(results.value == front_test){
      front_detection();
    }
    last = millis();      
    irrecv.resume(); // Receive the next value
  }
}
