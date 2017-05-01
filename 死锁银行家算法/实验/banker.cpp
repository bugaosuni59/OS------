#include <stdio.h>
#include <string.h>
#include <windows.h>
#define MAX 10

int Num_resource,Num_processes;//��Դ����������

int available[MAX];//������Դ��
int state[MAX];//����״̬
int state2[MAX];
int requests[MAX];//��ǰ������Դ��
int max[MAX][MAX];//����������
int need[MAX][MAX];//��ǰ�������
int allocated[MAX][MAX];//��ǰ�������

int bank_algorithm(int id)//���м��㷨 ��Ҫ����Լ���д 
{
    //����ֵ��1����ʾ���Է���
    //����ֵ��0����ʾ������ǰ������Դ�����ܷ���
    //��������ֵ����ʾ�����������������ܷ��� 
    int ne[MAX][MAX];
    int ava[MAX];
    memcpy(ne,need,sizeof(need));
    memcpy(ava,available,sizeof(available));
    // ����Ƿ��������Դ 
    for(int i=1;i<=Num_resource;i++){
    	if(requests[i]>ava[i])
    		return 0;
	}
	// ����Ƿ�ᷢ������ 
    for(int i=1;i<=Num_resource;i++){
		ava[i]-=requests[i];
		ne[id][i]-=requests[i];
	}
	int cnt=0;	// �Ѿ����ǵĽ������� 
	int ok=0;	// �ɷ����ǰ���̷�������Դ 
	int i;
	while(cnt!=Num_processes){
		for(i=1;i<=Num_processes;i++){
			ok=1;
			for(int j=1;j<=Num_resource;j++){
				if(ne[i][j]>ava[j])ok=0;
			}
			if(ok){// �ͷ���Դ 
				for(int j=0;j<Num_resource;j++){
					ava[j]+=max[i][j]-ne[i][j];
					ne[i][j]=0;
				}
				cnt++;
				break;
			}
		}
		// ������ 
		if(i==Num_processes+1){
			return -1;
		}
	}
    // ������Է��� ���ø��� available��need��allocated 
	 for(int i=1;i<=Num_resource;i++){
		available[i]-=requests[i];
		need[id][i]-=requests[i];
		allocated[id][i]+=requests[i];
	}
	bool isfinished=1;
	// �鿴���߳���û�����
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
    
    printf("��ȷ�������ļ�input.txt���ڣ���������output.txt\n");
   	if ((fp = fopen("Input.txt","r")) == NULL)
		printf("Read File Error\n");
    
    fscanf(fp,"%d",&Num_resource);                  //������Դ������ 
    for (i=1; i<=Num_resource; i++)
        fscanf(fp,"%d",&available[i]);              //�����ʼ��Դ�� 

    fscanf(fp,"%d",&Num_processes);                 //���������
    
    for (i=1; i<=Num_processes; i++)
    {
        for (j=1; j<=Num_resource; j++)
        {
            fscanf(fp,"%d",&max[i][j]);             //����MAX����
            need[i][j] = max[i][j];
            allocated[i][j] = 0;
        }
    }
    //�����ʼ��� 
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
        if(ans==1)                                               //�������
        {
            printf(" �C granted\n");
        }
        else if(ans==0)                                          //��������� ����������
        {
            printf("�C refused (extend the amount of available)\n");
        }
        else
        {
            printf(" �C refused (Deadlock is possible)\n");
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
