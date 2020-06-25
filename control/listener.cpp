
#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <stdio.h>
#include <wiringPi.h>


#define PwmPin 18


#define F1 11
#define F2 12
#define B1 13
#define B2 14

void forward(){
digitalWrite(F1,HIGH);
digitalWrite(F2,LOW);
digitalWrite(B1,HIGH);
digitalWrite(B2,LOW);
}

void backward(){
digitalWrite(F1,LOW);
digitalWrite(F2,HIGH);
digitalWrite(B1,LOW);
digitalWrite(B2,HIGH);
}

void stop(){
digitalWrite(F1,LOW);
digitalWrite(F2,LOW);
digitalWrite(B1,LOW);
digitalWrite(B2,LOW);
}


void dir_callback(const geometry_msgs::Twist& msg ) {
ROS_INFO_STREAM( " recieved distance =  "<< msg.linear.x << "recieved Angle = "<< msg.angular.z) ;

float mapped_angle =((msg.angular.z/180)*100)+100 ; //duty cycle mapped from milliseconds to scale 2000 ,equavilent to angle needed
pwmWrite(PwmPin , mapped_angle);

if (msg.linear.x > 0)forward();
else if (msg.linear.x < 0) backword(); 
else stop();




}

int main(int argc , char **argv){

ros::init(argc ,argv,"listener");
ros::NodeHandle nh ;

cout<<("Starting servo test\n");
  int rc = wiringPiSetupGpio();

  if (rc != 0) {
    cout<<("Failed to wiringPiSetupGpio()\n");
    return 0;
  }

  pinMode(PwmPin, PWM_OUTPUT);
// dc motors pins
  pinMode(F1,OUTPUT);
  pinMode(F2,OUTPUT);
  pinMode(B1,OUTPUT);
  pinMode(B2,OUTPUT);

  pwmSetMode(PWM_MODE_MS);
  pwmSetClock(192);
  pwmSetRange(2000);

ros::Subscriber sub = nh.subscribe("car_dir" ,1000, &dir_callback);
ros::spin();

}


