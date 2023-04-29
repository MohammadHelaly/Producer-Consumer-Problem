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

struct commodity
{
    string name;
    double price;
    deque<double> ps;
    deque<double> as;
    double avgprc;
    int flag=0;
    int flg=0;
};

struct currency
{
    int id;	
    double prc;
};

bool compareAlphabet(const commodity& a, const commodity& b)
{
    return a.name < b.name;
}

int main(int argc, char* argv[])
{
    
    int g,n,e=strtol(argv[1], NULL,10);;
    double curp=0.00;
    double cura=0.00;
    struct currency *arr;
    vector<commodity>x(11);
    vector<currency>p(e);
    
    key_t key;
    key=ftok("a.txt",0);
    int shmid=shmget(key,sizeof(struct currency)*(e+1), 0666 | IPC_CREAT);
    arr=(struct currency *)shmat(shmid, NULL, 0);
    
    union senum //arg;
    {
    int val;
    struct semid_ds*buf;
    ushort array[1];
    }arg;
    
    key_t key_s;
    key_s=ftok("b.txt",0);
    int s_semid=semget(key_s,1, 0666 | IPC_CREAT);
    arg.val=1;
    semctl(s_semid,0,SETVAL,arg);
    
    key_t key_n;
    key_n=ftok("c.txt",0);
    int n_semid=semget(key_n,1, 0666 | IPC_CREAT);
    arg.val=0;
    semctl(n_semid,0,SETVAL,arg);
    
    key_t key_e;
    key_e=ftok("d.txt",0);
    int e_semid=semget(key_e,1, 0666 | IPC_CREAT);
    arg.val=e;
    semctl(e_semid,0,SETVAL,arg);
    
    struct sembuf asem [1];
    asem [0].sem_num = 0;
    asem [0].sem_op = 0;
    asem [0].sem_flg = 0;
    
    for(g=0;g<=e;g++)
    {   
    arr[g].id=0;
    arr[g].prc=0.0;   
    }
    
    arr[0].id=1;
    
    for(n=0;n<11;n++)
    {    
    x[n].price=0.00;
    x[n].avgprc=0.00;
    x[n].ps.push_back(0.00);
    x[n].as.push_back(0.00);   
    }
    
    x[0].name="ALUMINIUM";
    x[1].name="COPPER";
    x[2].name="COTTON";
    x[3].name="CRUDEOIL";
    x[4].name="GOLD";
    x[5].name="LEAD";
    x[6].name="MENTHAOIL";
    x[7].name="NATURALGAS";
    x[8].name="NICKEL";
    x[9].name="SILVER";
    x[10].name="ZINC";
    
    while(true)
    {
    
    int i,j,m,y,z;

    for(y=1;y<=e;y++)
    {
    
    //semWait(n);
    asem [0].sem_op = -1;
    semop (n_semid, asem, 1);
    
    //semWait(s);
    asem [0].sem_op = -1;
    semop (s_semid, asem, 1);
    
    if(arr[y].id!=0)
    {
    p[y-1].id=arr[y].id;
    p[y-1].prc=arr[y].prc;
    }
    
    //semSignal(s);
    asem [0].sem_op = 1;
    semop (s_semid, asem, 1);
    
    //semSignal(e);
    asem [0].sem_op = 1;
    semop (e_semid, asem, 1);
    
    }
     
    for(z=0;z<e;z++)
    {
    
    string nm="0";
    
    if(p[z].id==1)
    {
    	nm="ALUMINIUM";
    }
    
    else if(p[z].id==2)
    {
    	nm="COPPER";
    }
    
    else if(p[z].id==3)
    {
    	nm="COTTON";
    }
    
    else if(p[z].id==4)
    {
    	nm="CRUDEOIL";
    }
    
    else if(p[z].id==5)
    {
    	nm="GOLD";
    }
    
    else if(p[z].id==6)
    {
    	nm="LEAD";
    }
    
    else if(p[z].id==7)
    {
    	nm="MENTHAOIL";
    }
    
    else if(p[z].id==8)
    {
    	nm="NATURALGAS";
    }
    
    else if(p[z].id==9)
    {
    	nm="NICKEL";
    }
    
    else if(p[z].id==10)
    {
    	nm="SILVER";
    }
    
    else if(p[z].id==11)
    {
    	nm="ZINC";
    }
    
    for (m=0;m<11;m++)
    {
    
    double sum=0;
    int cnt=0;
    
    if(x[m].name==nm)
    {
    
    x[m].price=p[z].prc;
    double tt=x[m].ps.back();
    
    if(p[z].prc>tt)
    {    
    x[m].flag=1;
    }
    
    else if (p[z].prc<tt)
    {
    x[m].flag=-1;
    }
    
    else if (x[m].ps.back()==0.00)
    {
    x[m].flag=0;
    }
    
    if(x[m].ps.size()>4)
    {
    x[m].ps.pop_front();
    }
    
    x[m].ps.push_back(p[z].prc);
    
    auto it =x[m].ps.begin();
       
    for(;it!=x[m].ps.end();it++)
    {
    sum+=*it;
    }
    
    cnt=x[m].ps.size();
    x[m].avgprc=(double)sum/(double)cnt;
    double aa=x[m].as.back();
    
    if(x[m].avgprc>aa)
    {
    x[m].flg=1;
    }
    
    else if (x[m].avgprc<aa)
    {
    x[m].flg=-1;
    }
    
    else if (x[m].avgprc=aa)
    {
    x[m].flg=0;
    }
    
    x[m].as.pop_back();
    x[m].as.push_back(x[m].avgprc);
    break;
    }
    
    }
    
    }
    
    sort(x.begin(), x.end(), compareAlphabet);
    std::cout << std::fixed;
    std::cout << std::setprecision(2);
    
    cout<<"+-------------------------------------+"<<endl;
    cout<<"| Currency      |  Price   | AvgPrice |"<<endl;
    cout<<"+-------------------------------------+"<<endl;
    
    for (i=0;i<11;i++)
    {
    
    cout<<"| "<<x[i].name;
    
    for (j=0;j<14-x[i].name.length();j++)
    {
    cout<<" ";
    }
    
    if(x[i].flag==-1)
    {
    printf("| ");
    printf("\x1b[31m%7.2f↓",x[i].price);
    printf("\x1b[0m");
    printf(" |");
    }
    
    else if(x[i].flag==1)
    {
    printf("| ");
    printf("\x1b[32m%7.2f↑",x[i].price);
    printf("\x1b[0m");
    printf(" |");
    }
    
    
    else if(x[i].flag==0)
    {
    printf("| ");
    printf("\x1b[36m%7.2f ",x[i].price);
    printf("\x1b[0m");
    printf(" |");
    }
    
    if(x[i].flg==1)
    {
    printf("\x1b[32m %7.2f↑",x[i].avgprc);
    printf("\x1b[0m");
    printf(" |\n");
    }
    
    if(x[i].flg==-1)
    {
    printf("\x1b[31m %7.2f↓",x[i].avgprc);
    printf("\x1b[0m");
    printf(" |\n");
    }
    
    if(x[i].flg==0)
    {
    printf("\x1b[36m %7.2f ",x[i].avgprc);
    printf("\x1b[0m");
    printf(" |\n");
    }
    
    }
    
    cout<<"+-------------------------------------+"<<endl;
    sleep(100/1000);
    printf("\e[1;1H\e[2J");
    
    }
    
    shmdt((void *) arr);
    shmctl(shmid, IPC_RMID, NULL);
    
    semctl (s_semid, 0, IPC_RMID);
    semctl (n_semid, 0, IPC_RMID);
    semctl (e_semid, 0, IPC_RMID);
    
    return 0;
}
