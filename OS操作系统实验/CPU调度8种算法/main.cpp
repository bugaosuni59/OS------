#include <bits/stdc++.h>
using namespace std;
struct PCB{
	int pid;  			//���̱�ʶ
	float priority;  	//���ȼ�
	int arrivetime;  	//����ʱ��
	int cputime;  		//CPU Burstʱ��
	int finishtime;  	//���ʱ��
	int waitingtime;  	//�ȴ�ʱ��
	int runtime;		//����Ŀǰ��ִ�е�ʱ��
	int lefttime;		//����Ŀǰ����Ҫִ�е�ʱ�䣨��ʱ��-ִ��ʱ�䣩
	int turnaround;  	//��תʱ�䣨���ʱ��-����ʱ�̣�
	int next;			//������PCB�ų�ReadyQueue
};
struct ReadyQueue{		// �ò�������ṹ������ṹ������ 
	int currenttime;	//��ǰʱ��
	int processescount;	//��ǰʱ�̶������ܵĽ�����Ŀ
	int firstprocess;	//��ǰʱ�̴��ڶ���ͷ�Ľ���
	int lastprocess;	//��ǰʱ�̴��ڶ���β�Ľ���
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
int findArr(int time){// �ҵ�time֮�����time�ĵ���ʱ������ĽǱ�
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
int findArrIn(int time,int ok[]){// �ҵ�time֮ǰ����time��ʣ��ʱ����̵ĽǱ꣬ok[i]=1�Ĳ�����ѡ 
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
	printf("ƽ���ȴ�ʱ��:%.3lf; ƽ����תʱ��%.3lf\n",AveW,AveT);
}
void RR5(){
	cout<<"ʱ��ƬΪ5��RR:"<<endl;
	init();
	sort(pcb,pcb+Num_processes,cmp0);	// ��id���� 
	int time=0;	
	int RR=5;	
	queue<int> q;	// ��������id(��0��ʼ) 
	int n=0;		// ���������� 
	int jb=-1;		// ��ǰִ�еĽ��̽Ǳ� 
	while(n!=Num_processes){
		if(jb==-1){// û������ִ�еĽ���
			// ֱ�Ӵӵ�ǰʱ�䣬����ĳ���̵���������ͬһʱ�䵽���Ľ��̼������ 
			jb = findArr(time);
			for(int i=0;i<Num_processes;i++){
				if(pcb[i].arrivetime==pcb[jb].arrivetime&&i!=jb){
					q.push(i);
				}
			}
		}else{
			// �ȼ���½��̵Ľ��� 
			for(int i=0;i<Num_processes;i++){
				if(pcb[i].arrivetime>time&&pcb[i].arrivetime<=time+min(RR,pcb[jb].lefttime)){
					q.push(i);
					pcb[i].waitingtime-=min(RR,pcb[jb].lefttime);// ��������ظ����� 
				}
			}
			// ��ִ�е�ǰ���򣬲����µȴ������в���  
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
				n++;// ����һ����ɽ��� 
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
			// ��ѡ����һ������ 
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
	printf("ƽ���ȴ�ʱ��:%.3lf; ƽ����תʱ��%.3lf\n",AveW,AveT);
}
void RR10(){
	cout<<"ʱ��ƬΪ10��RR:"<<endl;
	init();
	sort(pcb,pcb+Num_processes,cmp0);	// ��id���� 
	int time=0;	
	int RR=10;	
	queue<int> q;	// ��������id(��0��ʼ) 
	int n=0;		// ���������� 
	int jb=-1;		// ��ǰִ�еĽ��̽Ǳ� 
	while(n!=Num_processes){
		if(jb==-1){// û������ִ�еĽ���
			// ֱ�Ӵӵ�ǰʱ�䣬����ĳ���̵���������ͬһʱ�䵽���Ľ��̼������ 
			jb = findArr(time);
			for(int i=0;i<Num_processes;i++){
				if(pcb[i].arrivetime==pcb[jb].arrivetime&&i!=jb){
					q.push(i);
				}
			}
		}else{
			// �ȼ���½��̵Ľ��� 
			for(int i=0;i<Num_processes;i++){
				if(pcb[i].arrivetime>time&&pcb[i].arrivetime<=time+min(RR,pcb[jb].lefttime)){
					q.push(i);
					pcb[i].waitingtime-=min(RR,pcb[jb].lefttime);// ��������ظ����� 
				}
			}
			// ��ִ�е�ǰ���򣬲����µȴ������в���  
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
				n++;// ����һ����ɽ��� 
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
			// ��ѡ����һ������ 
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
	printf("ƽ���ȴ�ʱ��:%.3lf; ƽ����תʱ��%.3lf\n",AveW,AveT);
}
void SJF(){
	cout<<"����ռ��SJF:"<<endl;
	init();
	sort(pcb,pcb+Num_processes,cmp0);	// ��id���� 
	int ok[17]={0};// �����Ƿ���� 
	int time=0;
	int jb=findArr(0);	
	time = pcb[jb].arrivetime;
	// �ҵ�ͬʱ���絽����̽��� 
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
		// ����һ������ 
		jb = findArrIn(time,ok);
		// ��������м��м��ʱ�� 
		if(jb==-1){
			jb=findArr(time);	
			time = pcb[jb].arrivetime;
			// �ҵ�ͬʱ���絽����̽��� 
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
	printf("ƽ���ȴ�ʱ��:%.3lf; ƽ����תʱ��%.3lf\n",AveW,AveT);
}
void addWait(int time,int que[],int size){
	for(int i=0;i<size;i++)
		pcb[que[i]].waitingtime+=time;
}
// û�з��� -1 
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
	// ��ռֻ���ܷ������½��̵��������½��̵��ܹ�����С�ڵ�ǰ����
	cout<<"����ռ��SRTF:"<<endl;
	init();
	sort(pcb,pcb+Num_processes,cmp0);	// ��id���� 
	int ok[17]={0};// �����Ƿ���� 
	vector<int> waitP;	// �ȴ�����
	int q[17];	// �ȴ����У�������ʽ
	vector<int> arr;	// ����ʱ��
	int jb;
	int time;
	// �ҵ���ʱ�� 
	for(int i=0;i<Num_processes;i++){
		arr.push_back(pcb[i].arrivetime);
	}sort(arr.begin(),arr.end());
	// ȥ�� 
	for(int i=1;i<arr.size();i++){
		if(arr[i-1]==arr[i]){
			arr.erase(arr.begin()+i);
			i--;
		}
	}
	jb=findArr(0);	
	time = pcb[jb].arrivetime;
	// �ҵ�ͬʱ���絽����̽��� 
	for(int i=0;i<Num_processes;i++){
		if(pcb[i].arrivetime==pcb[jb].arrivetime&&pcb[i].lefttime<pcb[jb].lefttime)
			jb=i;
	}
	for(int i=0;i<Num_processes;i++){
		printf("%d:%d\n",time,jb+1);
		// ����time~time+jb.lefttime�Ƿ���в��� 
		// ����У�������ռ��addWait��i--��continue
		int tjb=haveSeize(time,time+pcb[jb].cputime);
		if(tjb!=-1){
			int dtime = pcb[tjb].arrivetime-time;
			memset(q,0,sizeof q);
			// ���timeǰ�ȴ����� ������time��tjb.arrtime֮�䵽�������̵߳ȴ�ʱ�� 
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
			pcb[jb].waitingtime-=dtime;// ��������ظ����� 
			pcb[jb].lefttime-=dtime;
			jb=tjb;
			time+=dtime;
			i--;
			continue;
		} 
		// �����վ�ִ��������� 
		time+=pcb[jb].cputime;
		pcb[jb].turnaround=time-pcb[jb].arrivetime;
		ok[jb]=1;
		// ����һ������ 
		jb = findArrIn(time,ok);
		// ��������м��м��ʱ�� 
		if(jb==-1){
			jb=findArr(time);	
			time = pcb[jb].arrivetime;
			// �ҵ�ͬʱ���絽����̽��� 
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
	printf("ƽ���ȴ�ʱ��:%.3lf; ƽ����תʱ��%.3lf\n",AveW,AveT);

}
void n_pri(){
	cout<<"����ռ�����ȼ������㷨:"<<endl;
	init();
	sort(pcb,pcb+Num_processes,cmp0);	// ��id���� 
	int ok[17]={0};// �����Ƿ���� 
	int time=0;
	int jb=findArr(0);	
	// ѡͬʱ����������ȼ� 
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
	printf("ƽ���ȴ�ʱ��:%.3lf; ƽ����תʱ��%.3lf\n",AveW,AveT);
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
	cout<<"����ռ�����ȼ������㷨:"<<endl;
	init();
	sort(pcb,pcb+Num_processes,cmp0);	// ��id���� 
	int ok[17]={0};// �����Ƿ���� 
	int q[17];
	vector<int> waitP;
	int time=0;
	int jb=findArr(0);
	float pri;
	// ѡͬʱ����������ȼ� 
	for(int i=0;i<Num_processes;i++)
		if(pcb[i].arrivetime==pcb[jb].arrivetime&&pcb[i].priority<pcb[jb].priority)
			jb=i;
	for(int i=0;i<Num_processes;i++){
		printf("%d:%d\n",time,jb+1);
		// ����time~time+jb.cput֮���Ƿ��д��
		int tjb=haveSeize2(time,time+pcb[jb].cputime,pcb[jb].priority);
		if(tjb!=-1){
			int dtime = pcb[tjb].arrivetime-time;
			memset(q,0,sizeof q);
			// ���timeǰ�ȴ����� ������time��tjb.arrtime֮�䵽�������̵߳ȴ�ʱ�� 
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
			pcb[jb].waitingtime-=dtime;// ��������ظ����� 
			pcb[jb].lefttime-=dtime;
			jb=tjb;
			time+=dtime;
			i--;
			continue;
		}
		// ���û����ռ �ճ�ִ�� 
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
	printf("ƽ���ȴ�ʱ��:%.3lf; ƽ����תʱ��%.3lf\n",AveW,AveT);
}
void HRRN(){
	cout<<"����Ӧ�ȵ��������㷨:"<<endl;
	// pri=1+���ȴ�ʱ��/��ִ��ʱ���� 
	init();
	for(int i=0;i<Num_processes;i++){
		pcb[i].priority=1;
	}
	sort(pcb,pcb+Num_processes,cmp0);	// ��id���� 
	int ok[17]={0};// �����Ƿ���� 
	int time=0;
	int jb=findArr(0);	
	// ѡͬʱ����������ȼ� 
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
	printf("ƽ���ȴ�ʱ��:%.3lf; ƽ����תʱ��%.3lf\n",AveW,AveT);
}
int main(){
	FILE *fp;
	fp = fopen("Input.txt","r");
	fscanf(fp,"%d",&Num_processes); 			//���������
	for (int i=0; i< Num_processes; i++) {
		fscanf(fp,"%d",&pcb[i].pid);		 	//����������
		pcb[i].pid--;
		fscanf(fp,"%d",&pcb[i].arrivetime); 	//���뵽��ʱ��
		fscanf(fp,"%d",&pcb[i].cputime); 		//����CPUʱ��
		fscanf(fp,"%f",&pcb[i].priority); 		//���ȼ�
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
