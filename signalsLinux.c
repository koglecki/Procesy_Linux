#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <signal.h>

int endpoint = 0;
int alarmflag = 0;
int counter = 0;

void alarm_handler(int signal) { 
    alarmflag = 1;
}


void end(int sigN, siginfo_t *si, void *sc) {
    endpoint = 1;
}

void sigusr1_handler(int signo, siginfo_t *info, void *ct) {
    time_t ttime;
    struct tm *timeinfo;
    time(&ttime);
    timeinfo = localtime(&ttime);
    int random = info->si_value.sival_int;
    counter--;
    printf("                                [ %d ] [ %d ] %s \n", info->si_pid, random, asctime(timeinfo));
}

int main(int argc, char** argv)
{    
    int c;
    int wvalue;
    int mvalue;
    if (argc != 5)
    {
        fprintf(stderr, "Podano bledna liczbe argumentow!\n");
        return 1;
    }
    while((c = getopt(argc, argv, "w:m:")) != -1)
    {
        switch(c)
        {
            case 'w': 
                wvalue = atoi(optarg);
                break;
            case 'm': 
                mvalue = atoi(optarg); 
                break;
            default: 
                return 1;
        }
    }
    printf("%d %d\n", wvalue, mvalue);

    pid_t cpid;
    int r;
    struct sigaction sig;       //obsługa maina
    struct sigaction sig2;      //rodzic zbiera informacje
    sig2.sa_sigaction = sigusr1_handler;    
    sig2.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &sig2, NULL);
    
    sig.sa_flags = SA_SIGINFO;
    sig.sa_sigaction = end;
    sigaction(SIGINT, &sig, NULL);

    while(1) {
        alarmflag = 0;
        counter++;
        cpid = fork();
        srand(time(NULL));
        time_t ttime;
        struct tm *timeinfo;
        if (cpid == 0) {  
            time(&ttime);
            timeinfo = localtime(&ttime);      
            r = rand() % mvalue + 1;
            printf("[ %d ] [ %d ] %s \n", getpid(), r, asctime(timeinfo));
            signal(SIGALRM, alarm_handler);
            alarm(r);
            
            int silnia = 1;
            int i = 1;
            while(alarmflag == 0)
            {
                silnia = silnia * i;
                i++;
            }
            union sigval s;
            s.sival_int = r;
            sigqueue(getppid(), SIGUSR1, s);
            exit(0);
        }
        else {
            signal(SIGALRM, alarm_handler);
            alarm(wvalue);
            while(alarmflag == 0)
            {
                sleep(1);
            }
        }
        if (endpoint == 1) {
            int e = counter;
            for(int i = 0; i < e; i++)
                while(wait(NULL) > 0);
            break;
        }       
    }
    return 0;
}
