#include <iostream>
#include <time.h>
#include <stdio.h>
#include <sched.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>
struct Threadtask
{
    int order,priority;
    struct timespec enter_thread_time,leave_thread_time,enter_critical_time,leave_critical_time;
};
FILE *fp=fopen("Preempt_Test_Result.txt","w");
struct timespec StartTime,EndTime;

void setSchedulingPolicy (int newPolicy, int priority)
{
    sched_param sched;
    int oldPolicy;
    if (pthread_getschedparam(pthread_self(), &oldPolicy, &sched))
    {
        perror("pthread_getschedparam()");
        exit(EXIT_FAILURE);
    }
    sched.sched_priority = priority;
    if (pthread_setschedparam(pthread_self(), newPolicy, &sched))
    {
        perror("pthread_setschedparam()");
        exit(EXIT_FAILURE);
    }
}
void workload_1ms (void)
{
    int repeat = 2000;
    int count[100]= {};
    for (int i = 0; i <= repeat; i++)
    {
        for (int j=0; j<100; j++)
            count[j]=j;
        for(int j=0; j<99; j++ )
        {
            if(count[j+1]>count[j])
            {
                int tmp=count[j+1];
                count[j+1]=count[j];
                count[j]=tmp;
            }
        }
    }
}
static void pinCPU(int cpu_number)
{
    cpu_set_t mask;
    CPU_ZERO(&mask);
    CPU_SET(cpu_number, &mask);
    if(sched_setaffinity(0, sizeof(cpu_set_t), &mask) == -1)
    {
        perror("sched_setaffinity");
        exit(EXIT_FAILURE);
    }
}

pthread_mutex_t mutex_section_1 ;

void *threadTask0(void *task_)
{
    Threadtask *task=(Threadtask *)task_;
    setSchedulingPolicy(SCHED_FIFO, task->priority);
    pinCPU(1);
    std::cout << task->priority;
    clock_gettime(CLOCK_REALTIME,&task->enter_thread_time);
    std::cout << "task 0 enter thread" << std::endl;
    {
        pthread_mutex_lock(&mutex_section_1);
        clock_gettime(CLOCK_REALTIME,&task->enter_critical_time);
        std::cout << "task 0 enter critical section" << std::endl;
        for(int i=0; i<5; ++i)
            workload_1ms();
        clock_gettime(CLOCK_REALTIME,&task->leave_critical_time);
        std::cout << "task 0 leave critical section" << std::endl;
        pthread_mutex_unlock(&mutex_section_1);
    }
    for(int i=0; i<3; ++i)
        workload_1ms();
    clock_gettime(CLOCK_REALTIME,&task->leave_thread_time);
    std::cout << "task 0 leave thread" << std::endl;
    return(nullptr);
}


void *threadTask1(void *task_)
{
    Threadtask *task=(Threadtask *)task_;
    setSchedulingPolicy(SCHED_FIFO, task->priority);
    pinCPU(1);
    std::cout << task->priority;
    clock_gettime(CLOCK_REALTIME,&task->enter_thread_time);
    std::cout << "task 1 enter thread" << std::endl;
    {
        pthread_mutex_lock(&mutex_section_1);
        clock_gettime(CLOCK_REALTIME,&task->enter_critical_time);
        std::cout << "task 1 enter critical section" << std::endl;
        for(int i=0; i<5; ++i)
            workload_1ms();
        clock_gettime(CLOCK_REALTIME,&task->leave_critical_time);
        std::cout << "task 1 leave critical section" << std::endl;
        pthread_mutex_unlock(&mutex_section_1);
    }
    clock_gettime(CLOCK_REALTIME,&task->leave_thread_time);
    std::cout << "task 1 leave thread" << std::endl;
    return(nullptr);
}


int main()
{
    if(!fp)
        printf("File BOOM\n");
    constexpr int TaskNum=2;
    pinCPU(0);
    long int switchingTime;
    pthread_t pthread[TaskNum];
    Threadtask task[TaskNum]= {{0,98},{1,98}};

    pthread_mutexattr_t mutexattr_prioceiling;
    int mutex_protocol, high_prio;
    high_prio = sched_get_priority_max(SCHED_FIFO);
    pthread_mutexattr_init (&mutexattr_prioceiling);
    pthread_mutexattr_getprotocol (&mutexattr_prioceiling,&mutex_protocol);
    pthread_mutexattr_setprotocol (&mutexattr_prioceiling,PTHREAD_PRIO_PROTECT);
    pthread_mutexattr_setprioceiling (&mutexattr_prioceiling,high_prio);
    pthread_mutex_init (&mutex_section_1, &mutexattr_prioceiling);

    for(int t=0; t<100; ++t)
    {
        pthread_create(&pthread[0], NULL, threadTask0, (void *)&task[0]);
        workload_1ms();
        pthread_create(&pthread[1], NULL, threadTask1, (void *)&task[1]);

        for(int i = 0; i < TaskNum; ++i)
        {
            if (pthread_join(pthread[i], NULL) != 0)
            {
                perror("pthread_join()");
                exit(1);
            }
        }
        switchingTime=abs(task[1].enter_critical_time.tv_nsec-task[0].leave_thread_time.tv_nsec); //ns
        std::cout<<switchingTime<<std::endl;
        fprintf(fp,"%ld\n",(switchingTime)/1000);
        fflush(stdout);
        sleep(1);
    }

    return 0;
}
