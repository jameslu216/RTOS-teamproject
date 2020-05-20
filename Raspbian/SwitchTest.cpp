#include <iostream>
#include <chrono>
#include <stdio.h>
#include <sched.h>
#include <pthread.h>
#include <unistd.h>
struct Threadtask {
    int order,priority;
};
FILE *fp=fopen("Switch_Test_Result.txt","a+");


void setSchedulingPolicy (int newPolicy, int priority) {
    sched_param sched;
    int oldPolicy;
    if (pthread_getschedparam(pthread_self(), &oldPolicy, &sched)) {
        perror("pthread_getschedparam()");
        exit(EXIT_FAILURE);
    }
    sched.sched_priority = priority;
    if (pthread_setschedparam(pthread_self(), newPolicy, &sched)) {
        perror("pthread_setschedparam()");
        exit(EXIT_FAILURE);
    }
}

void workload_1ms (void)
{
    int a=0;
}
static void pinCPU(int cpu_number) {
    cpu_set_t mask;
    CPU_ZERO(&mask);

    CPU_SET(cpu_number, &mask);

    if(sched_setaffinity(0, sizeof(cpu_set_t), &mask) == -1) {
        perror("sched_setaffinity");
        exit(EXIT_FAILURE);
    }
}

pthread_mutex_t mutex_section_1 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t count_hit_threshold=PTHREAD_COND_INITIALIZER;

void *threadTask(void *task_) {
    pinCPU(0);
    Threadtask *task=(Threadtask *)task_;
    setSchedulingPolicy(SCHED_FIFO, ((Threadtask *)task)->priority);
    pthread_mutex_lock(&mutex_section_1);
    //fprintf(fp,"Now doing Thread %d\n",task->order);
    workload_1ms();
    pthread_mutex_unlock(&mutex_section_1);
    return(nullptr);
}


int main() {
  if(!fp) printf("File BOOM\n");
  constexpr int TaskNum=2;
  pinCPU(0);
  std::chrono::system_clock::time_point perstartTime = std::chrono::system_clock::now();
  workload_1ms();
  workload_1ms();
  std::chrono::system_clock::time_point perendTime = std::chrono::system_clock::now();
  const long long int pertaskTime = std::chrono::duration_cast<std::chrono::nanoseconds>(perendTime - perstartTime).count();

  long long int switchingTime;
  pthread_t pthread[TaskNum];
  Threadtask task[TaskNum]={{0,98},{1,98}};

  for(int t=0;t<100;t++)
  {
    for(int i = 0;i < TaskNum;++i) {
      if(pthread_create(&pthread[i], NULL, threadTask, (void *)&task[i])) {
        perror("pthread_create()");
        exit(1);
      }
    }
    std::chrono::system_clock::time_point startTime=std::chrono::system_clock::now();
    for(int i = 0;i < TaskNum;++i) {
      if (pthread_join(pthread[i], NULL) != 0) {
        perror("pthread_join()");
        exit(1);
      }
    }
    std::chrono::system_clock::time_point endTime = std::chrono::system_clock::now();
    switchingTime=std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();
    std::cout<<pertaskTime<<std::endl<<switchingTime<<std::endl;
    fprintf(fp,"%d ns\n",switchingTime-pertaskTime);
    fflush(stdout);
    sleep(1);
  }
  return 0;
}
