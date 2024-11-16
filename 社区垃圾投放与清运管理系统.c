#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define _tou 0//垃圾投放 0
#define _qing 1//清运为1 
#define _ok 1//垃圾总量判断是否为0 
#define _no 0
typedef struct auibc
{
	char num[32];/*序号*/ 
	char date[32];/*日期*/
	float weight;/*重量*/
	int type;/*类别*/
	char _id[32];/*ID*/
	char phone[32];/*手机号码*/
	char detail[32];/*垃圾详情*/
	struct auibc *next;//链表处理数据 
}*link,link_node;
int num1[2]={1,1};//自动生成序号 
char type_str[4][32]={"可回收","不可回收","有害垃圾","其它垃圾"};
char pri_str[2][7][32]={{"序号","投放日期","重量","垃圾类别","垃圾投放者ID","手机号码","垃圾详情"},
						{"序号","清运日期","重量","垃圾类别","垃圾清运者ID","手机号码","垃圾详情"}};
char fname[2][1024]={{"./collect.dat"},{"./transfer.dat"}};/*文件名*/
float sum[4]={0,0,0,0};/*各个垃圾类别的总量*/
int _the_sym[2]={1,-1};/*存放垃圾投放还是清运的状态，1为入，-1为出*/
void count(link L,int type,float *type_sum); 
void save(link L,int type)/*保存文件*/ 
{
	link p=L->next;
	FILE *fp=fopen(fname[type],"wb");//打开文件 
	int i;
	for(i=0;i<7;i++)//打印七个信息小标题菜单 
		fprintf(fp,"%-14s",pri_str[type][i]);
	fprintf(fp,"\n");
	while(p)//将数据输出到磁盘文件 
	{
		fprintf(fp,"\n%-14s%-14s%-14.2f%-14s%-14s%-14s%-14s\n\n",
			p->num,p->date,p->weight,type_str[(p->type-1)%4],
			p->_id,p->phone,p->detail);
		p=p->next;
	}
	fclose(fp);//关闭文件 
}
int okno(int _the,float f)/*判断当前类别的总量是否够清运*/
{
	if(sum[_the]>=f)
		return _ok;
	return _no;
}
int ret_i(char *str)/*返回类别的下标值*/
{
	int i;
	for(i=0;i<4;i++)
		if(!strcmp(str,type_str[i]))
			return i+1;
	return i;
}
void read(link L,int type)/*读取文件*/
{
	FILE *fp=fopen(fname[type],"rb");//打开文件 
	link p;
	char str[1024];
	if(fp)
	{
		fgets(str,1024,fp);
		while(!feof(fp))//直到读完文件结束 
		{
			fscanf(fp,"%s",str);//读入磁盘文件数据存入链表中 
			if(!feof(fp))
			{
				p=(link)malloc(sizeof(link_node));
				p->next=L->next;
				L->next=p;
				strcpy(p->num,str);//自动存入序号 
				fscanf(fp,"%s%f%s",p->date,&p->weight,str);
				p->type=ret_i(str);//调用类型函数存入不同种类垃圾 
				fscanf(fp,"%s%s%s",p->_id,p->phone,p->detail);
			}
		}
		fclose(fp);
	}
}
void _list(link p,int type)//显示结点数据，type选择投放或者清运 
{
	int i;
	for(i=0;i<7;i++)//打印数据的七个内容信息标题 
		printf("%-14s",pri_str[type][i]);
	printf("\n%-14s%-14s%-14.2f%-14s%-14s%-14s%-14s\n\n",
			p->num,p->date,p->weight,type_str[(p->type-1)%4],
			p->_id,p->phone,p->detail);
}
void _printtype(void)/*输出类别选项小菜单*/
{
	int j;
	puts("----");
	for(j=0;j<4;j++)
		printf("%d %s\n",j+1,type_str[j]);
	puts("----");
}
int month(char *_mot_str,char *_son_str)/*判断是否为同一个月份*/
{
	int i;
	for(i=0;i<6;i++)
		if(_mot_str[i]!=_son_str[i])
			return _no;
	return _ok;
}
void creat(link L,int type)/*创建投放或者清运的垃圾的链表*/
{
	int i=1;
	char str[32];
	link p=(link)malloc(sizeof(link_node));//开辟新结点，分配内存空间 
	p->next=L->next;
	sprintf(p->num,"%d",num1[type]++);//自动生成序号 
	printf("请输入%s：\n",pri_str[type][i++]);//将小菜单标题存入数组pri_str通过调用数组打印菜单 
	scanf("%s",p->date);
	strcpy(str,p->date);
	printf("请输入%s：\n",pri_str[type][i++]);
	scanf("%f",&p->weight);
	printf("请输入%s：\n",pri_str[type][i++]);
	_printtype();
	scanf("%d",&p->type);
	while(p->type<1||p->type>4)//输入垃圾类别错误时提示有误 
	{
		printf("输入有误！请重新输入：\n");
		scanf("%d",&p->type);
	}
	if(type==_qing)//添加垃圾清运记录 
	{
		if(!okno(p->type,p->weight))//建立垃圾投放与清运关联，判断垃圾总量是否为0 
		{
			printf("垃圾总量不足，无法进行清运！\n");
			num1[type]--;
			free(p);//释放新申请的内存 
			return;
		}
	}
	printf("请输入%s：\n",pri_str[type][i++]);
	scanf("%s",p->_id);
	printf("请输入%s：\n",pri_str[type][i++]);
	scanf("%s",p->phone);
	printf("请输入%s：\n",pri_str[type][i++]);
	scanf("%s",p->detail);
	printf("添加成功！\n");
	sum[p->type-1]+=p->weight*_the_sym[type];
	L->next=p;
	save(L,type);
	p=L->next;
	while(p)
	{
		if(type==_tou)//添加垃圾投放记录 
		{
			if(!strcmp(str,p->date))
				_list(p,type);
		}
		else
		{
			if(month(str,p->date))//添加成功后显示当月的所有数据 
				_list(p,type);
		}
		p=p->next;
	}
}
void search(link L,int type)/*查询垃圾*/
{
	char _op[32],str[32];
	link p=L->next;
	int flag=0,d;//flag判断是否查找到 
	float f;
	printf("----\n1 %s\n2 %s\n3 %s\n4 %s\n0 返回\n----\n",
	pri_str[type][1],pri_str[type][4],
	pri_str[type][3],pri_str[type][2]);//打印小菜单标题，type选择投放还是清运 
	scanf("%s",_op);
	if(!strcmp(_op,"1"))
	{
		printf("请输入%s：\n",pri_str[type][1]);
		scanf("%s",str);
		while(p)
		{
			if(!strcmp(p->date,str))
			{
				_list(p,type);//输出数据 
				flag=1;
			}
			p=p->next;
		}
		if(!flag)
			printf("此数据不存在\n");
	}
	else if(!strcmp(_op,"2"))
	{
		printf("请输入%s：\n",pri_str[type][4]);
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
			printf("此数据不存在\n");
	}
	else if(!strcmp(_op,"3"))
	{
		printf("请输入%s：\n",pri_str[type][3]);
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
			printf("此数据不存在\n");
	}
	else if(!strcmp(_op,"4"))
	{
		printf("请输入%s：\n",pri_str[type][2]);
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
			printf("此数据不存在\n");
	}
	else if(!strcmp(_op,"0"))
		return;
	else
		printf("请输入0-4选项\n");
}
void change(link L,int type)/*修改垃圾信息*/
{
	char datee[32],_id[32];
	link p=L->next;
	int flag=0,i;
//flag判断是否查找到信息 
	printf("请输入待修改的%s和%s：\n",pri_str[type][1],pri_str[type][4]);
	scanf("%s%s",datee,_id);
	while(p)
	{
		if(!strcmp(datee,p->date)&&!strcmp(_id,p->_id))
		{
			i=0;
			putchar('\n');
			_list(p,type);
			printf("请输入新的%s：\n",pri_str[type][i++]);
			scanf("%s",p->num);
			printf("请输入新的%s：\n",pri_str[type][i++]);
			scanf("%s",p->date);
			sum[p->type-1]+=_the_sym[(type+1)%2]*p->weight;
			printf("请输入新的%s：\n",pri_str[type][i++]);
			scanf("%f",&p->weight);
			printf("请输入新的%s：\n",pri_str[type][i++]);
			_printtype();
			scanf("%d",&p->type);
			while(p->type<1||p->type>4)
			{
				printf("输入有误！请重新输入：\n");
				scanf("%d",&p->type);
			}
			sum[p->type-1]+=p->weight*_the_sym[type];//统计删除后数据的重量 
			printf("请输入新的%s：\n",pri_str[type][i++]);
			scanf("%s",p->_id);
			printf("请输入新的%s：\n",pri_str[type][i++]);
			scanf("%s",p->phone);
			printf("请输入新的%s：\n",pri_str[type][i++]);
			scanf("%s",p->detail);
			printf("修改成功！\n");
		
			flag=1;
		}
		p=p->next;
	}
	if(!flag)
		printf("此数据不存在\n");
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
void _delete(link L,int type)/*删除垃圾信息*/
{
	link p=L,s;
	char _id[32],datee[32];//输入要删除的信息 
	int flag=0;
	printf("请输入待删除的%s和%s：\n",pri_str[type][1],pri_str[type][4]);
	scanf("%s%s",datee,_id);
	while(p->next)
	{
		if(!strcmp(_id,p->next->_id)&&!strcmp(datee,p->next->date))//查找对应信息，flag作为是否查找到信息的标志 
		{
			_list(p,type);
			s=p->next;
			p->next=s->next;
			sum[s->type-1]+=s->weight*_the_sym[(type+1)%2];
			free(s);
			printf("删除成功！\n");
			flag=1;
			continue;
		}
		p=p->next;
	}
	if(!flag)
		printf("此数据不存在\n");
	else
		save(L,type);
}
void count(link L,int type,float *type_sum)/*统计垃圾的各类指定信息*/
{
	link p=L->next;
	int i;
	float sum=0;
	char _op[32],_op_time[32],_ed_time[32],_thetype[2][32]={"投放","清运"},str[32];
	//op_time起始时间，ed_tim终止日期 
	printf("----\n1 某个%s者的垃圾%s情况\n2 整个社区的垃圾%s情况\n3 各类垃圾的年%s总量\n0 返回\n----\n",
	_thetype[type],_thetype[type],_thetype[type],_thetype[type]);
	//type来选择投放和清运功能，0为投放，1为清运 
	scanf("%s",_op);//_op保存输入数字选择不同统计功能 
	if(!strcmp(_op,"1")||!strcmp(_op,"2"))//1,2,功能放一块，需要输入统计起始日期 
	{
		printf("请输入起始%s点：\n",pri_str[type][1]);
		scanf("%s",_op_time);
		printf("请输入终止%s点：\n",pri_str[type][1]);
		scanf("%s",_ed_time);
		if(!strcmp(_op,"1"))
		{
			printf("请输入%s：\n",pri_str[type][4]);//输入投放者ID 
			scanf("%s",str);
		}
		while(p)
		{
			if(strcmp(p->date,_op_time)>=0&&strcmp(p->date,_ed_time)<=0)//找到起始日期内的数据 
			{
				if(!strcmp(_op,"1"))
				{
					if(!strcmp(str,p->_id))//找到对应投放者id
					{
						sum+=p->weight;//累加总重量 
						type_sum[p->type-1]+=p->weight;//各种类重量 
					}
				}
				else//统计2功能 
				{
					sum+=p->weight;
					type_sum[p->type-1]+=p->weight;
				}
			}
			p=p->next;
		}
		for(i=0;i<4;i++)//输出统计结果 
			printf("%s重量：%.2f\t",type_str[i],type_sum[i]);
		printf("\n总重量：%.2f\n",sum);
	}
	else if(!strcmp(_op,"3"))//选项3的统计功能 
	{
		printf("请输入年份：\n");
		scanf("%s",str);
		while(p)
		{
			if(strstr(p->date,str))
				type_sum[p->type-1]+=p->weight;//累加重量 
			p=p->next;
		}
		for(i=0;i<4;i++)//输出统计结果 
			printf("%s年%s总量：%.2f\t",type_str[i],_thetype[type],type_sum[i]);
	}
	else if(!strcmp(_op,"0"))//退出 
		return;
	else
		printf("请输入合法选项0-3！\n");
}
void _work(link L,int type)/*投放或者清运的各个函数调用（type为0投放，1清运）*/
{
	char _op[32];
	float f[4]={0,0,0,0};
	while(1)
	{
		system("cls");//清屏 
		printf("1 添加记录\n2 查看记录\n3 修改记录\n4 删除记录\n5 统计数据\n0 返回\n");
		scanf("%s",_op);//输入选择保存到-op来选择功能 
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
			printf("输入错误，请输入0-5\n");
		system("pause");//冻结屏幕，观察执行结果 
	}
}
void _menu(link T,link Q)/*主菜单*/
{
	char _op[32];
	printf("--------\n1 垃圾投放管理\n2 垃圾清运管理\n0 退出系统\n------\n");
	scanf("%s",_op);//输入选择保存到-op来选择功能
	if(!strcmp(_op,"1"))
		_work(T,_tou);
	else if(!strcmp(_op,"2"))
		_work(Q,_qing);
	else if(!strcmp(_op,"0"))
	{
		save(T,_tou);
		save(Q,_qing);
		exit(0);//正常运行并退出程序 
	}
	else
		printf("输入错误，请输入0-2！\n");
}
int main(void)/*主函数*/
{
	link T=(link)malloc(sizeof(link_node));/*投放的链表*/ 
	link Q=(link)malloc(sizeof(link_node));/*清运的链表*/ 
	Q->next=NULL;
	T->next=NULL;
	read(Q,_qing);
	read(T,_tou);
	while(1)
	{
		system("cls");//清屏 
		_menu(T,Q);
		system("pause");//暂停，观察程序 
	}
	return 0;
}
