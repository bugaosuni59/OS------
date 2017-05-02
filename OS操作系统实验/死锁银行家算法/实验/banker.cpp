#include <stdio.h>
#include <string.h>
#include <windows.h>
#define MAX 10

int Num_resource,Num_processes;//资源数，进程数

int available[MAX];//可用资源数
int state[MAX];//进程状态
int state2[MAX];
int requests[MAX];//当前请求资源数
int max[MAX][MAX];//最大需求矩阵
int need[MAX][MAX];//当前需求矩阵
int allocated[MAX][MAX];//当前分配矩阵

int bank_algorithm(int id)//银行家算法 需要大家自己填写 
{
    //返回值是1，表示可以分配
    //返回值是0，表示超过当前可用资源，不能分配
    //其他返回值，表示可能引发死锁，不能分配 
    int ne[MAX][MAX];
    int ava[MAX];
    memcpy(ne,need,sizeof(need));
    memcpy(ava,available,sizeof(available));
    // 检查是否可申请资源 
    for(int i=1;i<=Num_resource;i++){
    	if(requests[i]>ava[i])
    		return 0;
	}
	// 检查是否会发生死锁 
    for(int i=1;i<=Num_resource;i++){
		ava[i]-=requests[i];
		ne[id][i]-=requests[i];
	}
	int cnt=0;	// 已经考虑的进程数量 
	int ok=0;	// 可否给当前进程分配满资源 
	int i;
	while(cnt!=Num_processes){
		for(i=1;i<=Num_processes;i++){
			ok=1;
			for(int j=1;j<=Num_resource;j++){
				if(ne[i][j]>ava[j])ok=0;
			}
			if(ok){// 释放资源 
				for(int j=0;j<Num_resource;j++){
					ava[j]+=max[i][j]-ne[i][j];
					ne[i][j]=0;
				}
				cnt++;
				break;
			}
		}
		// 死锁了 
		if(i==Num_processes+1){
			return -1;
		}
	}
    // 如果可以分配 还得更新 available，need，allocated 
	 for(int i=1;i<=Num_resource;i++){
		available[i]-=requests[i];
		need[id][i]-=requests[i];
		allocated[id][i]+=requests[i];
	}
	bool isfinished=1;
	// 查看该线程有没有完成
	for(int i=1;i<=Num_resource;i++){
		if(need[id][i]!=0)isfinished=0;
	}
	if(isfinished){
		for(int i=1;i<=Num_resource;i++){
			available[i]+=allocated[id][i];
			allocated[id][i]=0;
		}
	}
    return 1;
}

int main()
{
    int i,j,p_num,ans ;
    FILE *fp;
    
    printf("请确保输入文件input.txt存在，结果输出到output.txt\n");
   	if ((fp = fopen("Input.txt","r")) == NULL)
		printf("Read File Error\n");
    
    fscanf(fp,"%d",&Num_resource);                  //读入资源种类数 
    for (i=1; i<=Num_resource; i++)
        fscanf(fp,"%d",&available[i]);              //读入初始资源数 

    fscanf(fp,"%d",&Num_processes);                 //读入进程数
    
    for (i=1; i<=Num_processes; i++)
    {
        for (j=1; j<=Num_resource; j++)
        {
            fscanf(fp,"%d",&max[i][j]);             //读入MAX矩阵
            need[i][j] = max[i][j];
            allocated[i][j] = 0;
        }
    }
    //输出初始情况 
    printf("Initial: \n");
    for (i=1;i<=Num_resource;i++)
        printf("%c ",'A'+i-1);
    printf("\n");
    for (i=1;i<=Num_resource;i++)
        printf("%d ",available[i]);
    printf("\n");
    
    while(fscanf(fp,"%d",&p_num)!=EOF)
    {
        for (i=1; i<=Num_resource; i++)
            fscanf(fp, "%d",&requests[i]);
        printf("Process %d requests (",p_num);
        for (i=1; i<=Num_resource; i++)
        {
            printf("%d",requests[i]);
            if(i!=Num_resource) printf(",");
        }
        printf(")");
        
        ans = bank_algorithm(p_num);
        if(ans==1)                                               //允许分配
        {
            printf(" – granted\n");
        }
        else if(ans==0)                                          //不允许分配 超过最大可用
        {
            printf("– refused (extend the amount of available)\n");
        }
        else
        {
            printf(" – refused (Deadlock is possible)\n");
        }
        
        for (i=1; i<=Num_resource; i++)
        {
            if(need[p_num][i]!=0)
                break;
        }
        if(i==Num_resource+1)
        {
            printf("Process %d finishes\n",p_num);
        }

    }
    
    system("pause"); 
}
