#include <lee.h>
#include <Led.h>
#include <SRB-M-tinynode.h>
#define BUILD_FOR_SRB_MASTER
#include <readonly/SRB-app-dumotor-share.h>
#include <readonly/SRB-app-PS2handle-share.h>


//---------------- SRB 节点的初始化 ---------------------------------
SrbTinyNode node_ps2; //定义一个节点
Joystick::sDataRs ps2_data;

SrbTinyNode node_motor; //定义一个节点
MotorX2::sDataRs motor_data; //定义一个双电机模块的数据空间


void nodeInit(){
	//节点初始化设置
	node_motor.address = 4;//设置节点地址
	node_motor.datas = (uint8*)(&motor_data);//设置节点数据空间
	node_motor.mapping[1] = (sMapping*)(MotorX2::Mapping1);//设置节点访问映射表

	node_ps2.address = 12;
	node_ps2.datas = (uint8*)(&ps2_data);//设置节点数据空 间
	node_ps2.mapping[1] = (sMapping*)(Joystick::Mapping1);//设置节点访问映射表
	//初始化完毕
}

//--------- system -----------------------------
void setup() {
	SrbTinyNode::SRBinit();
	nodeInit();

	/*

	Serial.begin(9600);
	while (!Serial) {
	; // wait for serial port to connect. Needed for native USB port only
}*/
/*
delay(1000);
Serial.print("PS2 data size is :");
Serial.print(sizeof(Joystick::sDataRs));
*/


delay(100);
node_ps2.access(1);
ps2_data.handle.joy.l.x = 127;
ps2_data.handle.joy.l.y = 127;
delay(100);
SrbTinyNode::addressLedDisplayBroadcast(SHOW_ADDR_CLOSE);

}
uint8 trag_down = 1;
uint8 addr_led_CMD = SHOW_ADDR_LOW;
void loop() {
	node_ps2.access(1);
	int x = ps2_data.handle.joy.l.x;
	int y = ps2_data.handle.joy.l.y;
	//读取到的数据是0到255,松开手柄读到的是128.
	x-=128;
	y-=128;//电机为左右差速,设置电机速度到节点数据
	//ma表示 motorA,mb表示motorB
	motor_data.ma.u16 = joy2Motor(-(-y+x));
	//前面的负号是因为两个车轮的机械结构对称,车前运行时,电机一个正转一个反转
	motor_data.mb.u16 = joy2Motor((-y-x));
	node_motor.access(1);//将刚刚写好的数据发往电机.
	/*
	Serial.print(ps2_data.handle.joy.l.x);      Serial.print(" ");
	Serial.print(ps2_data.handle.joy.l.y);      Serial.print(" ");
	Serial.print(ps2_data.handle.joy.r.x);      Serial.print(" ");
	Serial.print(ps2_data.handle.joy.r.y);      Serial.print(" ");

	Serial.print(motor_data.ma.speed);      Serial.print(" ");
	Serial.print(motor_data.mb.speed);      Serial.print(" \n");
	*/
	if(trag_down == 1)
	{
		if(ps2_data.handle.key.trag==0)
		{
			SrbTinyNode::addressLedDisplayBroadcast(addr_led_CMD);
			if(addr_led_CMD != SHOW_ADDR_CLOSE)
			{
				addr_led_CMD++;
			}
			else{
				addr_led_CMD = SHOW_ADDR_LOW;
			}
		}
	}
	trag_down = ps2_data.handle.key.trag;
	delay(20);
}
//根据读取到的电机速度, 转换为发往电机的数值
uint16 joy2Motor(int val){
	MotorX2::sMotor m;
	if(val >200)  {//速度的最大值能达到255,将最大值设为200
		val = 200;
	}
	if(val<-200){
		val = -200;
	}
	if((val>-2)&&(val<2))
	{
		m.speed = 0;
		m.brake = 0;
	}
	else  {
		if(val>0)    {
			m.speed = (val*3 + 40)/4 ;
			m.brake = 0;
		}
		else{
			m.speed = (val*3 - 40)/4 ;
			m.brake = 0;

		}
	}
	return m.u16;
}
