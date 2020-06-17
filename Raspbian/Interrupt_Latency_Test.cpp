#include <wiringPi.h>
#include <iostream>
#include <time.h>
#include <stdio.h>
#define LED 1
int main(void)
{
    FILE *fp=fopen("Interrupt_Latency_Test_Result.txt","a+");
    if(!fp)
        printf("File BOOM!\n");
    wiringPiSetup();
    struct timespec StartTime,EndTime;
    pinMode (LED, OUTPUT);
    digitalWrite(LED,LOW);
    if(digitalRead(LED)==LOW)
        std::cout<<"No lights now"<<std::endl;
    for(int i=0; i<100; i++)
    {
        clock_gettime(CLOCK_REALTIME,&StartTime);
        digitalWrite(LED, HIGH);
        while(digitalRead(LED)==LOW);
        clock_gettime(CLOCK_REALTIME,&EndTime);
        long int delta=EndTime.tv_nsec-StartTime.tv_nsec;  // ns
        std::cout<<delta<<std::endl;
        fprintf(fp,"%ld\n",delta/1000);
        digitalWrite(LED,LOW);

    }

}
