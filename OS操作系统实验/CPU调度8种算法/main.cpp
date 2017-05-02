#include <bits/stdc++.h>
using namespace std;
struct PCB{
	int pid;  			//进程标识
	float priority;  	//优先级
	int arrivetime;  	//到达时刻
	int cputime;  		//CPU Burst时间
	int finishtime;  	//完成时刻
	int waitingtime;  	//等待时间
	int runtime;		//截至目前已执行的时间
	int lefttime;		//截至目前还需要执行的时间（总时间-执行时间）
	int turnaround;  	//周转时间（完成时刻-到达时刻）
	int next;			//用来将PCB排成ReadyQueue
};
struct ReadyQueue{		// 用不上这个结构，这个结构不好用 
	int currenttime;	//当前时刻
	int processescount;	//当前时刻队列中总的进程数目
	int firstprocess;	//当前时刻处于队列头的进程
	int lastprocess;	//当前时刻处于队列尾的进程
};
int Num_processes;
PCB pcb[17];
ReadyQueue rq;
bool cmp0(PCB &pcb1,PCB &pcb2){return pcb1.pid<pcb2.pid;}
bool cmp1(PCB &pcb1,PCB &pcb2){return pcb1.arrivetime<pcb2.arrivetime;}
void init(){
	for(int i=0;i<Num_processes;i++){
		pcb[i].lefttime=pcb[i].cputime;
		pcb[i].runtime=0;
		pcb[i].waitingtime=0;
		pcb[i].finishtime=0;
		pcb[i].turnaround=0;
	}
}
int findArr(int time){// 找到time之后包括time的到达时间最早的角标
	int jb=-1;
	int atime=1<<30; 
	for(int i=0;i<Num_processes;i++){
		if(pcb[i].arrivetime>=time){
			if(pcb[i].arrivetime<atime){
				jb=i;
				atime=pcb[i].arrivetime;
			}
		}
	}return jb;
}
int findArrIn(int time,int ok[]){// 找到time之前包括time的剩余时间最短的角标，ok[i]=1的不可以选 
	vector<int> v;
	for(int i=0;i<Num_processes;i++){
		if(!ok[i]&&pcb[i].arrivetime<=time){
			v.push_back(i);
		}
	}
	int len=v.size();
	if(len>=1){
		int jb=pcb[v[0]].pid;
		int ltime=1<<30;
		for(int i=0;i<len;i++){
			if(pcb[v[i]].lefttime<ltime){
				jb=pcb[v[i]].pid;
				ltime=pcb[v[i]].lefttime;
			}
		}
		return jb;
	}
	return -1;
}
void FCFS(){
	cout<<"1.FCFS:"<<endl;
	int time=0;
	double AveW=0;
	double AveT=0;
	sort(pcb,pcb+Num_processes,cmp1);
	for(int i=0;i<Num_processes;i++){
		printf("%d:%d\n",time,pcb[i].pid+1);
		pcb[i].waitingtime=time-pcb[i].arrivetime;
		time+=pcb[i].cputime;
		pcb[i].finishtime=time;
		pcb[i].turnaround=pcb[i].finishtime-pcb[i].arrivetime;
		AveW+=pcb[i].waitingtime;
		AveT+=pcb[i].turnaround;
	} 
	AveW/=Num_processes;
	AveT/=Num_processes;
	printf("平均等待时间:%.3lf; 平均周转时间%.3lf\n",AveW,AveT);
}
void RR5(){
	cout<<"时间片为5的RR:"<<endl;
	init();
	sort(pcb,pcb+Num_processes,cmp0);	// 按id排序 
	int time=0;	
	int RR=5;	
	queue<int> q;	// 就绪进程id(从0开始) 
	int n=0;		// 结束进程数 
	int jb=-1;		// 当前执行的进程角标 
	while(n!=Num_processes){
		if(jb==-1){// 没有正在执行的进程
			// 直接从当前时间，跳到某进程到来，并将同一时间到来的进程加入队列 
			jb = findArr(time);
			for(int i=0;i<Num_processes;i++){
				if(pcb[i].arrivetime==pcb[jb].arrivetime&&i!=jb){
					q.push(i);
				}
			}
		}else{
			// 先检查新进程的进入 
			for(int i=0;i<Num_processes;i++){
				if(pcb[i].arrivetime>time&&pcb[i].arrivetime<=time+min(RR,pcb[jb].lefttime)){
					q.push(i);
					pcb[i].waitingtime-=min(RR,pcb[jb].lefttime);// 避免后面重复计算 
				}
			}
			// 再执行当前程序，并更新等待队列中参数  
			if(pcb[jb].lefttime<=RR){
				printf("%d:%d\n",time,jb+1);
				time+=pcb[jb].lefttime;
				int len = q.size();
				int twait=pcb[jb].lefttime;
				pcb[jb].lefttime=0;
				pcb[jb].finishtime=time;
				pcb[jb].turnaround=pcb[jb].finishtime-pcb[jb].arrivetime;
				for(int j=0;j<len;j++){
					pcb[q.front()].waitingtime+=twait;
					q.push(q.front());
					q.pop();
				}
				n++;// 更新一个完成进程 
			}else{
				printf("%d:%d\n",time,jb+1);
				time+=RR;
				int len = q.size();
				int twait=RR;
				pcb[jb].lefttime-=RR;
				for(int j=0;j<len;j++){
					pcb[q.front()].waitingtime+=twait;
					q.push(q.front());
					q.pop();
				}
				q.push(jb);	
			}
			// 再选择下一个程序 
			if(q.size()>=1){
				jb=q.front();
				q.pop();
			}else{
				jb=-1;
			}
		}
	}
	double AveW=0;
	double AveT=0;
	for(int i=0;i<Num_processes;i++){
		AveW+=pcb[i].waitingtime;
		AveT+=pcb[i].turnaround;
	}
	AveW/=Num_processes;
	AveT/=Num_processes;
	printf("平均等待时间:%.3lf; 平均周转时间%.3lf\n",AveW,AveT);
}
void RR10(){
	cout<<"时间片为10的RR:"<<endl;
	init();
	sort(pcb,pcb+Num_processes,cmp0);	// 按id排序 
	int time=0;	
	int RR=10;	
	queue<int> q;	// 就绪进程id(从0开始) 
	int n=0;		// 结束进程数 
	int jb=-1;		// 当前执行的进程角标 
	while(n!=Num_processes){
		if(jb==-1){// 没有正在执行的进程
			// 直接从当前时间，跳到某进程到来，并将同一时间到来的进程加入队列 
			jb = findArr(time);
			for(int i=0;i<Num_processes;i++){
				if(pcb[i].arrivetime==pcb[jb].arrivetime&&i!=jb){
					q.push(i);
				}
			}
		}else{
			// 先检查新进程的进入 
			for(int i=0;i<Num_processes;i++){
				if(pcb[i].arrivetime>time&&pcb[i].arrivetime<=time+min(RR,pcb[jb].lefttime)){
					q.push(i);
					pcb[i].waitingtime-=min(RR,pcb[jb].lefttime);// 避免后面重复计算 
				}
			}
			// 再执行当前程序，并更新等待队列中参数  
			if(pcb[jb].lefttime<=RR){
				printf("%d:%d\n",time,jb+1);
				time+=pcb[jb].lefttime;
				int len = q.size();
				int twait=pcb[jb].lefttime;
				pcb[jb].lefttime=0;
				pcb[jb].finishtime=time;
				pcb[jb].turnaround=pcb[jb].finishtime-pcb[jb].arrivetime;
				for(int j=0;j<len;j++){
					pcb[q.front()].waitingtime+=twait;
					q.push(q.front());
					q.pop();
				}
				n++;// 更新一个完成进程 
			}else{
				printf("%d:%d\n",time,jb+1);
				time+=RR;
				int len = q.size();
				int twait=RR;
				pcb[jb].lefttime-=RR;
				for(int j=0;j<len;j++){
					pcb[q.front()].waitingtime+=twait;
					q.push(q.front());
					q.pop();
				}
				q.push(jb);	
			}
			// 再选择下一个程序 
			if(q.size()>=1){
				jb=q.front();
				q.pop();
			}else{
				jb=-1;
			}
		}
	}
	double AveW=0;
	double AveT=0;
	for(int i=0;i<Num_processes;i++){
		AveW+=pcb[i].turnaround-pcb[i].cputime;
		AveT+=pcb[i].turnaround;
	}
	AveW/=Num_processes;
	AveT/=Num_processes;
	printf("平均等待时间:%.3lf; 平均周转时间%.3lf\n",AveW,AveT);
}
void SJF(){
	cout<<"非抢占的SJF:"<<endl;
	init();
	sort(pcb,pcb+Num_processes,cmp0);	// 按id排序 
	int ok[17]={0};// 进程是否完成 
	int time=0;
	int jb=findArr(0);	
	time = pcb[jb].arrivetime;
	// 找到同时最早到的最短进程 
	for(int i=0;i<Num_processes;i++){
		if(pcb[i].arrivetime==pcb[jb].arrivetime&&pcb[i].lefttime<pcb[jb].lefttime)
			jb=i;
	}
	pcb[jb].waitingtime=time-pcb[jb].arrivetime;
	for(int i=0;i<Num_processes;i++){
		printf("%d:%d\n",time,jb+1);
		time+=pcb[jb].cputime;
		pcb[jb].turnaround=time-pcb[jb].arrivetime;
		ok[jb]=1;
		// 找下一个任务 
		jb = findArrIn(time,ok);
		// 考虑如果中间有间断时间 
		if(jb==-1){
			jb=findArr(time);	
			time = pcb[jb].arrivetime;
			// 找到同时最早到的最短进程 
			for(int j=0;j<Num_processes;j++){
				if(pcb[j].arrivetime==pcb[jb].arrivetime&&pcb[j].lefttime<pcb[jb].lefttime)
					jb=j;
			}
		}else{
			pcb[jb].waitingtime=time-pcb[jb].arrivetime;
		}
	}
	double AveW=0;
	double AveT=0;
	for(int i=0;i<Num_processes;i++){
		AveW+=pcb[i].waitingtime;
		AveT+=pcb[i].turnaround;
	}
	AveW/=Num_processes;
	AveT/=Num_processes;
	printf("平均等待时间:%.3lf; 平均周转时间%.3lf\n",AveW,AveT);
}
void addWait(int time,int que[],int size){
	for(int i=0;i<size;i++)
		pcb[que[i]].waitingtime+=time;
}
// 没有返回 -1 
int haveSeize(int start,int end){
	vector<int> v;
	for(int i=0;i<Num_processes;i++){
		if(pcb[i].arrivetime>start&&pcb[i].arrivetime<end&&pcb[i].arrivetime+pcb[i].cputime<end)
			v.push_back(i);
	}
	int len=v.size();
	if(len<1)return -1;
	int jb=v[0];
	for(int i=0;i<len;i++){
		if(pcb[v[i]].arrivetime<pcb[jb].arrivetime){
			jb=v[i];
		}
	}
	return jb;
}
void SRTF(){
	// 抢占只可能发生在新进程到来，且新进程的总工作量小于当前进程
	cout<<"可抢占的SRTF:"<<endl;
	init();
	sort(pcb,pcb+Num_processes,cmp0);	// 按id排序 
	int ok[17]={0};// 进程是否完成 
	vector<int> waitP;	// 等待队列
	int q[17];	// 等待队列，数组形式
	vector<int> arr;	// 到达时间
	int jb;
	int time;
	// 找到达时间 
	for(int i=0;i<Num_processes;i++){
		arr.push_back(pcb[i].arrivetime);
	}sort(arr.begin(),arr.end());
	// 去重 
	for(int i=1;i<arr.size();i++){
		if(arr[i-1]==arr[i]){
			arr.erase(arr.begin()+i);
			i--;
		}
	}
	jb=findArr(0);	
	time = pcb[jb].arrivetime;
	// 找到同时最早到的最短进程 
	for(int i=0;i<Num_processes;i++){
		if(pcb[i].arrivetime==pcb[jb].arrivetime&&pcb[i].lefttime<pcb[jb].lefttime)
			jb=i;
	}
	for(int i=0;i<Num_processes;i++){
		printf("%d:%d\n",time,jb+1);
		// 考察time~time+jb.lefttime是否会有插入 
		// 如果有，进行抢占，addWait，i--，continue
		int tjb=haveSeize(time,time+pcb[jb].cputime);
		if(tjb!=-1){
			int dtime = pcb[tjb].arrivetime-time;
			memset(q,0,sizeof q);
			// 检查time前等待队列 并更新time到tjb.arrtime之间到来的新线程等待时间 
			waitP.clear();
			for(int k=0;k<Num_processes;k++){
				if(!ok[k]&&k!=jb&&pcb[k].arrivetime<=time){
					waitP.push_back(k);
				}else if(!ok[k]&&k!=jb&&pcb[k].arrivetime<time){
					pcb[k].waitingtime+=pcb[tjb].arrivetime-pcb[k].arrivetime;
				}
			}
			for(int k=0;k<waitP.size();k++){
				q[k]=waitP[k];
			}
			addWait(dtime,q,waitP.size());
			pcb[jb].waitingtime-=dtime;// 避免后面重复计算 
			pcb[jb].lefttime-=dtime;
			jb=tjb;
			time+=dtime;
			i--;
			continue;
		} 
		// 无则照旧执行下面代码 
		time+=pcb[jb].cputime;
		pcb[jb].turnaround=time-pcb[jb].arrivetime;
		ok[jb]=1;
		// 找下一个任务 
		jb = findArrIn(time,ok);
		// 考虑如果中间有间断时间 
		if(jb==-1){
			jb=findArr(time);	
			time = pcb[jb].arrivetime;
			// 找到同时最早到的最短进程 
			for(int j=0;j<Num_processes;j++){
				if(pcb[j].arrivetime==pcb[jb].arrivetime&&pcb[j].lefttime<pcb[jb].lefttime)
					jb=j;
			}
		}else{
			pcb[jb].waitingtime=time-pcb[jb].arrivetime;
		}
	}
	double AveW=0;
	double AveT=0;
	for(int i=0;i<Num_processes;i++){
		AveW+=pcb[i].waitingtime;
		AveT+=pcb[i].turnaround;
	}
	AveW/=Num_processes;
	AveT/=Num_processes;
	printf("平均等待时间:%.3lf; 平均周转时间%.3lf\n",AveW,AveT);

}
void n_pri(){
	cout<<"非抢占的优先级调度算法:"<<endl;
	init();
	sort(pcb,pcb+Num_processes,cmp0);	// 按id排序 
	int ok[17]={0};// 进程是否完成 
	int time=0;
	int jb=findArr(0);	
	// 选同时到达最高优先级 
	for(int i=0;i<Num_processes;i++)
		if(pcb[i].arrivetime==pcb[jb].arrivetime&&pcb[i].priority<pcb[jb].priority)
			jb=i;
	for(int i=0;i<Num_processes;i++){
		printf("%d:%d\n",time,jb+1);
		pcb[jb].waitingtime=time-pcb[jb].arrivetime;
		time+=pcb[jb].cputime;
		ok[jb]=1;
		pcb[jb].finishtime=time;
		pcb[jb].turnaround=pcb[jb].finishtime-pcb[jb].arrivetime;
		double pri=1<<30;
		for(int j=0;j<Num_processes;j++){
			if(!ok[j]&&pcb[j].arrivetime<time&&pcb[j].priority<pri){
				jb=j;
				pri=pcb[j].priority;
			}
		}
	} 
	double AveW=0;
	double AveT=0;
	for(int i=0;i<Num_processes;i++){
		AveW+=pcb[i].waitingtime;
		AveT+=pcb[i].turnaround;
	}
	AveW/=Num_processes;
	AveT/=Num_processes;
	printf("平均等待时间:%.3lf; 平均周转时间%.3lf\n",AveW,AveT);
}
int haveSeize2(int start,int end,float pri){
	vector<int> v;
	for(int i=0;i<Num_processes;i++){
		if(pcb[i].arrivetime>start&&pcb[i].arrivetime<end&&pcb[i].priority<pri)
			v.push_back(i);
	}
	int len=v.size();
	if(len<1)return -1;
	int jb=v[0];
	for(int i=0;i<len;i++){
		if(pcb[v[i]].arrivetime<pcb[jb].arrivetime){
			jb=v[i];
		}
	}
	return jb;
}
void pri(){
	cout<<"可抢占的优先级调度算法:"<<endl;
	init();
	sort(pcb,pcb+Num_processes,cmp0);	// 按id排序 
	int ok[17]={0};// 进程是否完成 
	int q[17];
	vector<int> waitP;
	int time=0;
	int jb=findArr(0);
	float pri;
	// 选同时到达最高优先级 
	for(int i=0;i<Num_processes;i++)
		if(pcb[i].arrivetime==pcb[jb].arrivetime&&pcb[i].priority<pcb[jb].priority)
			jb=i;
	for(int i=0;i<Num_processes;i++){
		printf("%d:%d\n",time,jb+1);
		// 考察time~time+jb.cput之间是否有打断
		int tjb=haveSeize2(time,time+pcb[jb].cputime,pcb[jb].priority);
		if(tjb!=-1){
			int dtime = pcb[tjb].arrivetime-time;
			memset(q,0,sizeof q);
			// 检查time前等待队列 并更新time到tjb.arrtime之间到来的新线程等待时间 
			waitP.clear();
			for(int k=0;k<Num_processes;k++){
				if(!ok[k]&&k!=jb&&pcb[k].arrivetime<=time){
					waitP.push_back(k);
				}else if(!ok[k]&&k!=jb&&pcb[k].arrivetime<time){
					pcb[k].waitingtime+=pcb[tjb].arrivetime-pcb[k].arrivetime;
				}
			}
			for(int k=0;k<waitP.size();k++){
				q[k]=waitP[k];
			}
			addWait(dtime,q,waitP.size());
			pcb[jb].waitingtime-=dtime;// 避免后面重复计算 
			pcb[jb].lefttime-=dtime;
			jb=tjb;
			time+=dtime;
			i--;
			continue;
		}
		// 如果没有抢占 照常执行 
		pcb[jb].waitingtime=time-pcb[jb].arrivetime;
		time+=pcb[jb].lefttime;
		ok[jb]=1;
		pcb[jb].finishtime=time;
		pcb[jb].turnaround=pcb[jb].finishtime-pcb[jb].arrivetime;
		for(int j=0;j<Num_processes;j++){
			double pri=1<<30;
			if(!ok[j]&&pcb[j].arrivetime<time&&pcb[j].priority<pri){
				jb=j;
				pri=pcb[j].priority;
			}
		} 
	}
	double AveW=0;
	double AveT=0;
	for(int i=0;i<Num_processes;i++){
		AveW+=pcb[i].waitingtime;
		AveT+=pcb[i].turnaround;
	}
	AveW/=Num_processes;
	AveT/=Num_processes;
	printf("平均等待时间:%.3lf; 平均周转时间%.3lf\n",AveW,AveT);
}
void HRRN(){
	cout<<"高响应比调度优先算法:"<<endl;
	// pri=1+（等待时间/总执行时长） 
	init();
	for(int i=0;i<Num_processes;i++){
		pcb[i].priority=1;
	}
	sort(pcb,pcb+Num_processes,cmp0);	// 按id排序 
	int ok[17]={0};// 进程是否完成 
	int time=0;
	int jb=findArr(0);	
	// 选同时到达最高优先级 
	for(int i=0;i<Num_processes;i++)
		if(pcb[i].arrivetime==pcb[jb].arrivetime&&pcb[i].priority>pcb[jb].priority)
			jb=i;
	for(int i=0;i<Num_processes;i++){
		printf("%d:%d\n",time,jb+1);
		pcb[jb].waitingtime=time-pcb[jb].arrivetime;
		time+=pcb[jb].cputime;
		ok[jb]=1;
		pcb[jb].finishtime=time;
		pcb[jb].turnaround=pcb[jb].finishtime-pcb[jb].arrivetime;
		double pri=0;
		for(int j=0;j<Num_processes;j++){
			pcb[j].priority=1+(time-pcb[j].arrivetime)*1.0/pcb[j].cputime;
			if(!ok[j]&&pcb[j].arrivetime<time&&pcb[j].priority>pri){
				jb=j;
				pri=pcb[j].priority;
			}
		}
	} 
	double AveW=0;
	double AveT=0;
	for(int i=0;i<Num_processes;i++){
		AveW+=pcb[i].waitingtime;
		AveT+=pcb[i].turnaround;
	}
	AveW/=Num_processes;
	AveT/=Num_processes;
	printf("平均等待时间:%.3lf; 平均周转时间%.3lf\n",AveW,AveT);
}
int main(){
	FILE *fp;
	fp = fopen("Input.txt","r");
	fscanf(fp,"%d",&Num_processes); 			//读入进程数
	for (int i=0; i< Num_processes; i++) {
		fscanf(fp,"%d",&pcb[i].pid);		 	//读入进程序号
		pcb[i].pid--;
		fscanf(fp,"%d",&pcb[i].arrivetime); 	//读入到达时间
		fscanf(fp,"%d",&pcb[i].cputime); 		//读入CPU时间
		fscanf(fp,"%f",&pcb[i].priority); 		//优先级
	}
	FCFS();
	RR5();
	RR10();
	SJF();
	SRTF();
	n_pri();
	pri();
	HRRN();
	return 0;
}
