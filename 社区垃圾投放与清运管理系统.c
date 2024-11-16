#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define _tou 0//����Ͷ�� 0
#define _qing 1//����Ϊ1 
#define _ok 1//���������ж��Ƿ�Ϊ0 
#define _no 0
typedef struct auibc
{
	char num[32];/*���*/ 
	char date[32];/*����*/
	float weight;/*����*/
	int type;/*���*/
	char _id[32];/*ID*/
	char phone[32];/*�ֻ�����*/
	char detail[32];/*��������*/
	struct auibc *next;//���������� 
}*link,link_node;
int num1[2]={1,1};//�Զ�������� 
char type_str[4][32]={"�ɻ���","���ɻ���","�к�����","��������"};
char pri_str[2][7][32]={{"���","Ͷ������","����","�������","����Ͷ����ID","�ֻ�����","��������"},
						{"���","��������","����","�������","����������ID","�ֻ�����","��������"}};
char fname[2][1024]={{"./collect.dat"},{"./transfer.dat"}};/*�ļ���*/
float sum[4]={0,0,0,0};/*����������������*/
int _the_sym[2]={1,-1};/*�������Ͷ�Ż������˵�״̬��1Ϊ�룬-1Ϊ��*/
void count(link L,int type,float *type_sum); 
void save(link L,int type)/*�����ļ�*/ 
{
	link p=L->next;
	FILE *fp=fopen(fname[type],"wb");//���ļ� 
	int i;
	for(i=0;i<7;i++)//��ӡ�߸���ϢС����˵� 
		fprintf(fp,"%-14s",pri_str[type][i]);
	fprintf(fp,"\n");
	while(p)//����������������ļ� 
	{
		fprintf(fp,"\n%-14s%-14s%-14.2f%-14s%-14s%-14s%-14s\n\n",
			p->num,p->date,p->weight,type_str[(p->type-1)%4],
			p->_id,p->phone,p->detail);
		p=p->next;
	}
	fclose(fp);//�ر��ļ� 
}
int okno(int _the,float f)/*�жϵ�ǰ���������Ƿ�����*/
{
	if(sum[_the]>=f)
		return _ok;
	return _no;
}
int ret_i(char *str)/*���������±�ֵ*/
{
	int i;
	for(i=0;i<4;i++)
		if(!strcmp(str,type_str[i]))
			return i+1;
	return i;
}
void read(link L,int type)/*��ȡ�ļ�*/
{
	FILE *fp=fopen(fname[type],"rb");//���ļ� 
	link p;
	char str[1024];
	if(fp)
	{
		fgets(str,1024,fp);
		while(!feof(fp))//ֱ�������ļ����� 
		{
			fscanf(fp,"%s",str);//��������ļ����ݴ��������� 
			if(!feof(fp))
			{
				p=(link)malloc(sizeof(link_node));
				p->next=L->next;
				L->next=p;
				strcpy(p->num,str);//�Զ�������� 
				fscanf(fp,"%s%f%s",p->date,&p->weight,str);
				p->type=ret_i(str);//�������ͺ������벻ͬ�������� 
				fscanf(fp,"%s%s%s",p->_id,p->phone,p->detail);
			}
		}
		fclose(fp);
	}
}
void _list(link p,int type)//��ʾ������ݣ�typeѡ��Ͷ�Ż������� 
{
	int i;
	for(i=0;i<7;i++)//��ӡ���ݵ��߸�������Ϣ���� 
		printf("%-14s",pri_str[type][i]);
	printf("\n%-14s%-14s%-14.2f%-14s%-14s%-14s%-14s\n\n",
			p->num,p->date,p->weight,type_str[(p->type-1)%4],
			p->_id,p->phone,p->detail);
}
void _printtype(void)/*������ѡ��С�˵�*/
{
	int j;
	puts("----");
	for(j=0;j<4;j++)
		printf("%d %s\n",j+1,type_str[j]);
	puts("----");
}
int month(char *_mot_str,char *_son_str)/*�ж��Ƿ�Ϊͬһ���·�*/
{
	int i;
	for(i=0;i<6;i++)
		if(_mot_str[i]!=_son_str[i])
			return _no;
	return _ok;
}
void creat(link L,int type)/*����Ͷ�Ż������˵�����������*/
{
	int i=1;
	char str[32];
	link p=(link)malloc(sizeof(link_node));//�����½�㣬�����ڴ�ռ� 
	p->next=L->next;
	sprintf(p->num,"%d",num1[type]++);//�Զ�������� 
	printf("������%s��\n",pri_str[type][i++]);//��С�˵������������pri_strͨ�����������ӡ�˵� 
	scanf("%s",p->date);
	strcpy(str,p->date);
	printf("������%s��\n",pri_str[type][i++]);
	scanf("%f",&p->weight);
	printf("������%s��\n",pri_str[type][i++]);
	_printtype();
	scanf("%d",&p->type);
	while(p->type<1||p->type>4)//��������������ʱ��ʾ���� 
	{
		printf("�����������������룺\n");
		scanf("%d",&p->type);
	}
	if(type==_qing)//����������˼�¼ 
	{
		if(!okno(p->type,p->weight))//��������Ͷ�������˹������ж����������Ƿ�Ϊ0 
		{
			printf("�����������㣬�޷��������ˣ�\n");
			num1[type]--;
			free(p);//�ͷ���������ڴ� 
			return;
		}
	}
	printf("������%s��\n",pri_str[type][i++]);
	scanf("%s",p->_id);
	printf("������%s��\n",pri_str[type][i++]);
	scanf("%s",p->phone);
	printf("������%s��\n",pri_str[type][i++]);
	scanf("%s",p->detail);
	printf("��ӳɹ���\n");
	sum[p->type-1]+=p->weight*_the_sym[type];
	L->next=p;
	save(L,type);
	p=L->next;
	while(p)
	{
		if(type==_tou)//�������Ͷ�ż�¼ 
		{
			if(!strcmp(str,p->date))
				_list(p,type);
		}
		else
		{
			if(month(str,p->date))//��ӳɹ�����ʾ���µ��������� 
				_list(p,type);
		}
		p=p->next;
	}
}
void search(link L,int type)/*��ѯ����*/
{
	char _op[32],str[32];
	link p=L->next;
	int flag=0,d;//flag�ж��Ƿ���ҵ� 
	float f;
	printf("----\n1 %s\n2 %s\n3 %s\n4 %s\n0 ����\n----\n",
	pri_str[type][1],pri_str[type][4],
	pri_str[type][3],pri_str[type][2]);//��ӡС�˵����⣬typeѡ��Ͷ�Ż������� 
	scanf("%s",_op);
	if(!strcmp(_op,"1"))
	{
		printf("������%s��\n",pri_str[type][1]);
		scanf("%s",str);
		while(p)
		{
			if(!strcmp(p->date,str))
			{
				_list(p,type);//������� 
				flag=1;
			}
			p=p->next;
		}
		if(!flag)
			printf("�����ݲ�����\n");
	}
	else if(!strcmp(_op,"2"))
	{
		printf("������%s��\n",pri_str[type][4]);
		scanf("%s",str);
		while(p)
		{
			if(!strcmp(str,p->_id))
			{
				_list(p,type);
				flag=1;
			}
			p=p->next;
		}
		if(!flag)
			printf("�����ݲ�����\n");
	}
	else if(!strcmp(_op,"3"))
	{
		printf("������%s��\n",pri_str[type][3]);
		_printtype();
		scanf("%d",&d);
		while(p)
		{
			if(d==p->type)
			{
				_list(p,type);
				flag=1;
			}
			p=p->next;
		}
		if(!flag)
			printf("�����ݲ�����\n");
	}
	else if(!strcmp(_op,"4"))
	{
		printf("������%s��\n",pri_str[type][2]);
		scanf("%f",&f);
		while(p)
		{
			if(f==p->weight)
			{
				_list(p,type);
				flag=1;
			}
			p=p->next;
		}
		if(!flag)
			printf("�����ݲ�����\n");
	}
	else if(!strcmp(_op,"0"))
		return;
	else
		printf("������0-4ѡ��\n");
}
void change(link L,int type)/*�޸�������Ϣ*/
{
	char datee[32],_id[32];
	link p=L->next;
	int flag=0,i;
//flag�ж��Ƿ���ҵ���Ϣ 
	printf("��������޸ĵ�%s��%s��\n",pri_str[type][1],pri_str[type][4]);
	scanf("%s%s",datee,_id);
	while(p)
	{
		if(!strcmp(datee,p->date)&&!strcmp(_id,p->_id))
		{
			i=0;
			putchar('\n');
			_list(p,type);
			printf("�������µ�%s��\n",pri_str[type][i++]);
			scanf("%s",p->num);
			printf("�������µ�%s��\n",pri_str[type][i++]);
			scanf("%s",p->date);
			sum[p->type-1]+=_the_sym[(type+1)%2]*p->weight;
			printf("�������µ�%s��\n",pri_str[type][i++]);
			scanf("%f",&p->weight);
			printf("�������µ�%s��\n",pri_str[type][i++]);
			_printtype();
			scanf("%d",&p->type);
			while(p->type<1||p->type>4)
			{
				printf("�����������������룺\n");
				scanf("%d",&p->type);
			}
			sum[p->type-1]+=p->weight*_the_sym[type];//ͳ��ɾ�������ݵ����� 
			printf("�������µ�%s��\n",pri_str[type][i++]);
			scanf("%s",p->_id);
			printf("�������µ�%s��\n",pri_str[type][i++]);
			scanf("%s",p->phone);
			printf("�������µ�%s��\n",pri_str[type][i++]);
			scanf("%s",p->detail);
			printf("�޸ĳɹ���\n");
		
			flag=1;
		}
		p=p->next;
	}
	if(!flag)
		printf("�����ݲ�����\n");
	else
	{
		save(L,type);
		p=L->next;
		while(p)
		{
			_list(p,type);
			p=p->next;
		}
	}
}
void _delete(link L,int type)/*ɾ��������Ϣ*/
{
	link p=L,s;
	char _id[32],datee[32];//����Ҫɾ������Ϣ 
	int flag=0;
	printf("�������ɾ����%s��%s��\n",pri_str[type][1],pri_str[type][4]);
	scanf("%s%s",datee,_id);
	while(p->next)
	{
		if(!strcmp(_id,p->next->_id)&&!strcmp(datee,p->next->date))//���Ҷ�Ӧ��Ϣ��flag��Ϊ�Ƿ���ҵ���Ϣ�ı�־ 
		{
			_list(p,type);
			s=p->next;
			p->next=s->next;
			sum[s->type-1]+=s->weight*_the_sym[(type+1)%2];
			free(s);
			printf("ɾ���ɹ���\n");
			flag=1;
			continue;
		}
		p=p->next;
	}
	if(!flag)
		printf("�����ݲ�����\n");
	else
		save(L,type);
}
void count(link L,int type,float *type_sum)/*ͳ�������ĸ���ָ����Ϣ*/
{
	link p=L->next;
	int i;
	float sum=0;
	char _op[32],_op_time[32],_ed_time[32],_thetype[2][32]={"Ͷ��","����"},str[32];
	//op_time��ʼʱ�䣬ed_tim��ֹ���� 
	printf("----\n1 ĳ��%s�ߵ�����%s���\n2 ��������������%s���\n3 ������������%s����\n0 ����\n----\n",
	_thetype[type],_thetype[type],_thetype[type],_thetype[type]);
	//type��ѡ��Ͷ�ź����˹��ܣ�0ΪͶ�ţ�1Ϊ���� 
	scanf("%s",_op);//_op������������ѡ��ͬͳ�ƹ��� 
	if(!strcmp(_op,"1")||!strcmp(_op,"2"))//1,2,���ܷ�һ�飬��Ҫ����ͳ����ʼ���� 
	{
		printf("��������ʼ%s�㣺\n",pri_str[type][1]);
		scanf("%s",_op_time);
		printf("��������ֹ%s�㣺\n",pri_str[type][1]);
		scanf("%s",_ed_time);
		if(!strcmp(_op,"1"))
		{
			printf("������%s��\n",pri_str[type][4]);//����Ͷ����ID 
			scanf("%s",str);
		}
		while(p)
		{
			if(strcmp(p->date,_op_time)>=0&&strcmp(p->date,_ed_time)<=0)//�ҵ���ʼ�����ڵ����� 
			{
				if(!strcmp(_op,"1"))
				{
					if(!strcmp(str,p->_id))//�ҵ���ӦͶ����id
					{
						sum+=p->weight;//�ۼ������� 
						type_sum[p->type-1]+=p->weight;//���������� 
					}
				}
				else//ͳ��2���� 
				{
					sum+=p->weight;
					type_sum[p->type-1]+=p->weight;
				}
			}
			p=p->next;
		}
		for(i=0;i<4;i++)//���ͳ�ƽ�� 
			printf("%s������%.2f\t",type_str[i],type_sum[i]);
		printf("\n��������%.2f\n",sum);
	}
	else if(!strcmp(_op,"3"))//ѡ��3��ͳ�ƹ��� 
	{
		printf("��������ݣ�\n");
		scanf("%s",str);
		while(p)
		{
			if(strstr(p->date,str))
				type_sum[p->type-1]+=p->weight;//�ۼ����� 
			p=p->next;
		}
		for(i=0;i<4;i++)//���ͳ�ƽ�� 
			printf("%s��%s������%.2f\t",type_str[i],_thetype[type],type_sum[i]);
	}
	else if(!strcmp(_op,"0"))//�˳� 
		return;
	else
		printf("������Ϸ�ѡ��0-3��\n");
}
void _work(link L,int type)/*Ͷ�Ż������˵ĸ����������ã�typeΪ0Ͷ�ţ�1���ˣ�*/
{
	char _op[32];
	float f[4]={0,0,0,0};
	while(1)
	{
		system("cls");//���� 
		printf("1 ��Ӽ�¼\n2 �鿴��¼\n3 �޸ļ�¼\n4 ɾ����¼\n5 ͳ������\n0 ����\n");
		scanf("%s",_op);//����ѡ�񱣴浽-op��ѡ���� 
		if(!strcmp(_op,"1"))
			creat(L,type);
		else if(!strcmp(_op,"2"))
			search(L,type);
		else if(!strcmp(_op,"3"))
			change(L,type);
		else if(!strcmp(_op,"4"))
			_delete(L,type);
		else if(!strcmp(_op,"5"))
			count(L,type,f);
		else if(!strcmp(_op,"0"))
			return;
		else
			printf("�������������0-5\n");
		system("pause");//������Ļ���۲�ִ�н�� 
	}
}
void _menu(link T,link Q)/*���˵�*/
{
	char _op[32];
	printf("--------\n1 ����Ͷ�Ź���\n2 �������˹���\n0 �˳�ϵͳ\n------\n");
	scanf("%s",_op);//����ѡ�񱣴浽-op��ѡ����
	if(!strcmp(_op,"1"))
		_work(T,_tou);
	else if(!strcmp(_op,"2"))
		_work(Q,_qing);
	else if(!strcmp(_op,"0"))
	{
		save(T,_tou);
		save(Q,_qing);
		exit(0);//�������в��˳����� 
	}
	else
		printf("�������������0-2��\n");
}
int main(void)/*������*/
{
	link T=(link)malloc(sizeof(link_node));/*Ͷ�ŵ�����*/ 
	link Q=(link)malloc(sizeof(link_node));/*���˵�����*/ 
	Q->next=NULL;
	T->next=NULL;
	read(Q,_qing);
	read(T,_tou);
	while(1)
	{
		system("cls");//���� 
		_menu(T,Q);
		system("pause");//��ͣ���۲���� 
	}
	return 0;
}
