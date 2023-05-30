#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <cstdlib>
#include <time.h>
#include <sys/time.h>

using namespace std;

//semaphore s=1,n=0,e=bsize;

struct currency
{
    int id;	
    double prc;
};

string printtime()
{
    char date[256];
    char res[256];
    struct timeval tv;
    struct tm *tm;
    
    gettimeofday(&tv,NULL);
    tm=localtime(&tv.tv_sec);
    strftime(date,256,"%m/%d/%Y",tm);
    snprintf(res,256,"[%s %02d:%02d:%02d.%03ld] ",date,tm->tm_hour,tm->tm_min,tm->tm_sec,(tv.tv_usec/1000));
    string str(res);
    
    return res;
}

int main(int argc, char* argv[])
{
    int i=0,j,T,e;
    struct currency *arr;
    string name,time;
    double mean,standev;
    
    name=argv[1];
    mean=atof(argv[2]);
    standev=atof(argv[3]);
    T=strtol(argv[4], NULL,10);
    e=strtol(argv[5], NULL,10);
    //setvbuf(stderr,NULL,_IOLBF,1024);
    
    key_t key;
    key=ftok("a.txt",0);
    int shmid=shmget(key,sizeof(struct currency)*(e+1), 0666 | IPC_CREAT);
    arr=(struct currency *)shmat(shmid, NULL, 0);
    
    union senum
    {
    int val;
    struct semid_ds*buf;
    ushort array[1];
    }arg;
    
    key_t key_s;
    key_s=ftok("b.txt",0);
    int s_semid=semget(key_s,1, 0666);
    arg.val=1;
    semctl(s_semid,0,SETVAL,arg);
    
    key_t key_n;
    key_n=ftok("c.txt",0);
    int n_semid=semget(key_n,1, 0666);
    arg.val=0;
    semctl(n_semid,0,SETVAL,arg);
    
    key_t key_e;
    key_e=ftok("d.txt",0);
    int e_semid=semget(key_e,1, 0666);
    arg.val=e;
    semctl(e_semid,0,SETVAL,arg);
    
    struct sembuf asem [1];
    asem [0].sem_num = 0;
    asem [0].sem_op = 0;
    asem [0].sem_flg = 0;
    
    std::default_random_engine generator;
    std::normal_distribution<double> distribution(mean,standev);
    
    while (true)
    {
    currency p;
    if(name=="ALUMINIUM")
    {
    	p.id=1;
    }
    else if(name=="COPPER")
    {
    	p.id=2;
    }
    else if(name=="COTTON")
    {
    	p.id=3;
    }
    else if(name=="CRUDEOIL")
    {
    	p.id=4;
    }
    else if(name=="GOLD")
    {
    	p.id=5;
    }
    else if(name=="LEAD")
    {
    	p.id=6;
    }
    else if(name=="MENTHAOIL")
    {
    	p.id=7;
    }
    else if(name=="NATURALGAS")
    {
    	p.id=8;
    }
    else if(name=="NICKEL")
    {
    	p.id=9;
    }
    else if(name=="SILVER")
    {
    	p.id=10;
    }
    else if(name=="ZINC")
    {
    	p.id=11;
    }
    
    double price=0.00;
    price=distribution(generator);
    p.prc=price; 
    time= printtime();
    cerr<<time<<name;
    fprintf(stderr,": Generating new value %7.2f\n",price);
    
    //semWait(e);
    time=printtime();
    cerr<<time<<name;
    fprintf(stderr,": Attempting to acquire mutex on shared buffer\n");
    asem [0].sem_op = -1;
    semop (e_semid, asem, 1);
    
    //semWait(s);
    asem [0].sem_op = -1;
    semop (s_semid, asem, 1);
    
    if (arr[0].id>e)
    {
    arr[0].id=1;
    }
    time=printtime();
    cerr<<time<<name;
    fprintf(stderr,": Placing %7.2f on shared buffer\n",price);
    arr[arr[0].id]=p;
    arr[0].id+=1;
    
    //semSignal(s);
    asem [0].sem_op = 1;
    semop (s_semid, asem, 1);
    
    //semSignal(n);
    asem [0].sem_op = 1;
    semop (n_semid, asem, 1);
    
    time=printtime();
    cerr<<time<<name;
    fprintf(stderr,": Sleeping for %d ms\n",T);
    sleep(T/1000);
    }
    
    shmdt((void *) arr);
    shmctl(shmid, IPC_RMID, NULL);
    
    semctl (s_semid, 0, IPC_RMID);
    semctl (n_semid, 0, IPC_RMID);
    semctl (e_semid, 0, IPC_RMID);
    
    return 0;
}
