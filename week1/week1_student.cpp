#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <time.h>
#include <math.h>
#include <sys/time.h>
#include <stdint.h>
#include <signal.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <vector>

//gcc -o week1 week1_student.cpp -lwiringPi -lm -g
int setup_imu();
void calibrate_imu();      
void read_imu();    

#define RANGE 65535.0
#define DS_RANGE 2000.0
#define G_RANGE 6.0

//global variables
int accel_address,gyro_address;
float x_gyro_calibration=0;
float y_gyro_calibration=0;
float z_gyro_calibration=0;
float roll_calibration=0;
float pitch_calibration=0;
float accel_z_calibration=0;
float imu_data[6]; //accel xyz,  gyro xyz, 
long time_curr;
long time_prev;
struct timespec te;
float yaw=0;
float pitch_angle=0;
float roll_angle=0;

 
int main (int argc, char *argv[])
{

    setup_imu();
    calibrate_imu();    
    
    while(1)
    {
      read_imu();    
      printf("%10.5f %10.5f %10.5f %10.5f %10.5f\n\r",imu_data[3],imu_data[4],imu_data[5],pitch_angle,roll_angle);
      //sleep(1);
    }
  
}

void calibrate_imu()
{

  x_gyro_calibration=0;
  y_gyro_calibration=0;
  z_gyro_calibration=0;
  roll_calibration=0;
  pitch_calibration=0;
  accel_z_calibration=0;
  float x = 0.0;
  float y = 0.0;
  float z = 0.0;
  float gyro_x = 0.0;
  float gyro_y = 0.0;
  float gyro_z = 0.0;
  float average_roll = 0.0;
  float average_pitch = 0.0;

  for (int i = 0; i < 1000; ++i) {
    read_imu();
      x += imu_data[0];
      y += imu_data[1];
      z += imu_data[2];
      gyro_x += imu_data[3];
      gyro_y += imu_data[4];
      gyro_z += imu_data[5];
      average_roll += roll_angle;
      average_pitch += pitch_angle;
  }

  x_gyro_calibration = gyro_x / 1000;
  y_gyro_calibration = gyro_y / 1000;
  z_gyro_calibration = gyro_z / 1000;

  roll_calibration = average_roll / 1000;
  pitch_calibration = average_pitch / 1000;
  accel_z_calibration = z / 1000;

  printf("calibration complete, %f %f %f %f %f %f\n\r",x_gyro_calibration,y_gyro_calibration,z_gyro_calibration,roll_calibration,pitch_calibration,accel_z_calibration);
}


void read_imu()
{
  uint8_t address=0x12;//todo: set address value for accel x value 
  float ax=0;
  float az=0;
  float ay=0;
  int vh=0;
  int vl=0;
  int vw=0;

  //accel reads

  address=0x12;//use 0x00 format for hex
  vw=wiringPiI2CReadReg16(accel_address,address);    
  //convert from 2's complement
  if(vw>0x8000)
  {
    vw=vw ^ 0xffff;
    vw=-vw-1;
  }          
  
  imu_data[0]=(G_RANGE / RANGE) * vw;//convert to g's  
  address=0x14;//use 0x00 format for hex
  vw=wiringPiI2CReadReg16(accel_address,address);   
  //convert from 2's complement
  if(vw>0x8000)
  {
    vw=vw ^ 0xffff;
    vw=-vw-1;
  }          
  imu_data[1]=(G_RANGE / RANGE) * vw;//convert to g's  
  address=0x16;//use 0x00 format for hex
  vw=wiringPiI2CReadReg16(accel_address,address);
  //convert from 2's complement     
  if(vw>0x8000)
  {
    vw=vw ^ 0xffff;
    vw=-vw-1;
  }          
  imu_data[2]=(G_RANGE / RANGE) * vw; //convert to g's   

  //gyro reads
  address=0x02;//use 0x00 format for hex
  vw=wiringPiI2CReadReg16(gyro_address,address);   
  //convert from 2's complement          
  if(vw>0x8000)
  {
    vw=vw ^ 0xffff;
    vw=-vw-1;
  }          
  imu_data[3]=(DS_RANGE * vw / RANGE) - x_gyro_calibration;//convert to degrees/sec
  address=0x04;//use 0x00 format for hex
  vw=wiringPiI2CReadReg16(gyro_address,address);    
  //convert from 2's complement              
  if(vw>0x8000)
  {
    vw=vw ^ 0xffff;
    vw=-vw-1;
  }          
  imu_data[4]=(DS_RANGE * vw / RANGE) - y_gyro_calibration;//convert to degrees/sec
  address=0x06;//use 0x00 format for hex
  vw=wiringPiI2CReadReg16(gyro_address,address);   
  //convert from 2's complement               
  if(vw>0x8000)
  {
    vw=vw ^ 0xffff;
    vw=-vw-1;
  }          
  imu_data[5]=((DS_RANGE * vw / RANGE) - z_gyro_calibration) * -1;//convert to degrees/sec  

  float roll =  atan2(imu_data[2],imu_data[0]) * 180.0/M_PI;
  float pitch = atan2(imu_data[1],imu_data[0]) * 180.0/M_PI;

  roll_angle = roll - roll_calibration;
  pitch_angle = pitch - pitch_calibration;
}


int setup_imu()
{
  wiringPiSetup ();
  
  //setup imu on I2C
  accel_address=wiringPiI2CSetup (0x19) ; 
  
  gyro_address=wiringPiI2CSetup (0x69) ; 
  
  if(accel_address==-1)
  {
    printf("-----cant connect to accel I2C device %d --------\n",accel_address);
    return -1;
  }
  else if(gyro_address==-1)
  {
    printf("-----cant connect to gyro I2C device %d --------\n",gyro_address);
    return -1;
  }
  else
  {
    printf("all i2c devices detected\n");
    sleep(1);

    // page 25 location of accel registers
    wiringPiI2CWriteReg8(accel_address, 0x7d, 0x04); //power on accel    

    // 0x00 means +- 3g range setting...page 30 on the datasheet
    wiringPiI2CWriteReg8(accel_address, 0x41, 0x00); //accel range to +_3g    
    wiringPiI2CWriteReg8(accel_address, 0x40, 0x89); //high speed filtered accel

    // page 36 location of gyro registers map
    wiringPiI2CWriteReg8(gyro_address, 0x11, 0x00);//power on gyro
    wiringPiI2CWriteReg8(gyro_address, 0x0F, 0x01);//set gyro to +-1000dps

    // set data rate and bandwidth page 39
    wiringPiI2CWriteReg8(gyro_address, 0x10, 0x03);//set data rate and bandwith
    sleep(1);
  }
  return 0;
}


