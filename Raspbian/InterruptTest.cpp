#include <wiringPi.h>
#include <iostream>
#include <chrono>
#include <stdio.h>
#define LED 1
int main(void) {
FILE *fp=fopen("Interrupt_Test_Result.txt","a+");
if(!fp) printf("File BOOM!\n");
wiringPiSetup();
pinMode (LED, OUTPUT);
digitalWrite(LED,LOW);
if(digitalRead(LED)==LOW)
    std::cout<<"No lights now"<<std::endl;
for(int i=0;i<100;i++) {
  std::chrono::system_clock::time_point startTime = std::chrono::system_clock::now();
  digitalWrite(LED, HIGH);
  while(digitalRead(LED)==LOW);
  std::chrono::system_clock::time_point endTime = std::chrono::system_clock::now();
  int delta = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();
  std::cout<<delta<<std::endl;
  fprintf(fp,"%d ns\n",delta);
  digitalWrite(LED,LOW);

  }

}
