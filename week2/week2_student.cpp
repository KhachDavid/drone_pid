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
void update_filter();

#define RANGE 65535.0
#define DS_RANGE 2000.0
#define G_RANGE 6.0
#define ROLL_LOWER_LIMIT -45.0
#define ROLL_UPPER_LIMIT 45.0
#define PITCH_LOWER_LIMIT -45.0
#define PITCH_UPPER_LIMIT 45.0
#define GYRO_RATE 300.0
#define JOYSTICK_TIMEOUT 0.35

#define A_DELTA 0.02

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

 
float prev_roll = 0;
float current_roll = 0;
float prev_pitch = 0;
float current_pitch = 0;
float intl_pitch = 0;
float intl_roll = 0;

//global variables to add

struct Joystick
{
  int key0;
  int key1;
  int key2;
  int key3;
  int pitch;
  int roll;
  int yaw;
  int thrust;
  int sequence_num;
};

Joystick* shared_memory; 
int run_program=1;

int main (int argc, char *argv[])
{

    setup_imu();
    calibrate_imu();    
    
    // Joystick setup
    setup_joystick();
    signal(SIGINT, &trap);

    while(run_program == 1)
    {
      Joystick joystick_data = *shared_memory;

      read_imu(); 
      update_filter();
      safety_check();  
      //printf("%10.5f %10.5f %10.5f %10.5f %10.5f\n\r",imu_data[3],imu_data[4],imu_data[5],pitch_angle,roll_angle);
      //sleep(1);
      printf("Pitch: %10.5f %10.5f %10.5f\n", pitch_angle, intl_pitch, current_pitch);
      printf("Roll: %10.5f %10.5f %10.5f\n", roll_angle, intl_roll, current_roll);
      
    }

    return 0;
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

  //x_gyro_calibration = gyro_x / 1000;
  //y_gyro_calibration = gyro_y / 1000;
  //z_gyro_calibration = gyro_z / 1000;
  x_gyro_calibration = 0;
  y_gyro_calibration = 0;
  z_gyro_calibration = 0;


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


void update_filter()
{
  //get current time in nanoseconds
  timespec_get(&te,TIME_UTC);
  time_curr=te.tv_nsec;

  //compute time since last execution
  float imu_diff=time_curr-time_prev;
  //check for rollover
  if(imu_diff<=0)
  {
    imu_diff+=1000000000;
  }
  //convert to seconds
  imu_diff=imu_diff/1000000000;
  time_prev=time_curr;

  //comp. filter for roll, pitch here:
  // 
  //Roll_t=roll_accel*A+(1-A)*(roll_gyro_delta+Rollt-1),
  //Where A << 1 (try .02)
  intl_roll = (imu_data[4] * imu_diff + intl_roll);
  intl_pitch = (imu_data[5] * imu_diff + intl_pitch);
  
  current_roll = roll_angle * A_DELTA + 
                 (1 - A_DELTA) * (imu_data[4] * imu_diff + prev_roll);
  prev_roll = current_roll;

  current_pitch = pitch_angle * A_DELTA + 
                 (1 - A_DELTA) * (imu_data[5] * imu_diff + prev_pitch);
  prev_pitch = current_pitch;

}

void safety_check() {
  
}

void setup_joystick()
{

  int segment_id;   
  struct shmid_ds shmbuffer; 
  int segment_size; 
  const int shared_segment_size = 0x6400; 
  int smhkey=33222;
  
  /* Allocate a shared memory segment.  */ 
  segment_id = shmget (smhkey, shared_segment_size,IPC_CREAT | 0666); 
  /* Attach the shared memory segment.  */ 
  shared_memory = (Joystick*) shmat (segment_id, 0, 0); 
  printf ("shared memory attached at address %p\n", shared_memory); 
  /* Determine the segment's size. */ 
  shmctl (segment_id, IPC_STAT, &shmbuffer); 
  segment_size  =               shmbuffer.shm_segsz; 
  printf ("segment size: %d\n", segment_size); 
  /* Write a string to the shared memory segment.  */ 
  //sprintf (shared_memory, "test!!!!."); 

}


void trap(int signal)

{ 
   printf("ending program\n\r");

   run_program=0;
}
