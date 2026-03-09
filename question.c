#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"struct.c"

Question* entryQuention(Question **head); //录入题目函数 
Question* addQuention(Question **head);   //添加题目函数 
Question* delQuestion(Question **head);   //删除题目函数 

void printList(Question **head);     //打印题目函数
void update(Question **head);        //重新计算每题分值并重新编号
void saveToFile(Question **head);    //保存题目到文件函数
Question* readQuestion();			 //从文件读取题目函数



Question* entryQuention(Question **head){//录入题目函数
	*head=NULL; 
	Question *p=NULL;//指向上一道题的指针 
	int count=0;//记录题目数量 
	int i;
	float eachScore;//计算每道题的分值 
	
	printf("输入要录入的题目数量(>=5): ");
	do{
		if(scanf("%d",&count)!=1){//如果输入的不是数字 
			printf("错误输入，请重新输入有效数字(>=5):");
			while(getchar()!='\n');//清空缓冲区 
			continue;
		} 
		
		if(count<5){//要录入的题目数量不够 
			printf("错误输入，题目不能少于5道！\n");
			printf("请重新输入题目数量：") ;
			continue;
		}
		else break;	
	}while(1);
	getchar();//吸收scanf留下的换行符
	eachScore=100.0/count;
	printf("tip:用()表示挖空处\n");
	//循环录入题目 
	for(i=0;i<count;i++){
		Question *newQuestion=(Question*)malloc(sizeof(Question));//开辟新空间给要录入的题目 
		newQuestion->next=NULL;//将next初始化为空，避免野指针 
        if (newQuestion==NULL){//指针为空 
            printf("内存分配失败！无法创建题目。");
            return;
        }
        if(*head==NULL){       //如果是第一道题 
        	*head=newQuestion;//头指针指向第一道题 
        	p=newQuestion;    //p指针指向第一道题 
		}
		else{
			p->next=newQuestion;//将上一题与当前题目链接 
			p=newQuestion;      //p指针指向当前题目 
		}
        newQuestion->num=i+1;//设置题号
        printf("请输入第%d/%d道题目:",i+1,count);
        gets(newQuestion->question);
        if (strcmp(" ",newQuestion->question)==0){
            printf("问题不能为空!请重新输入！");
            while(getchar()!='\n'); 
            free(newQuestion);
            i--; // 重新输入这道题
            continue;
		}
        printf("请录入正确答案: ");
        scanf("%c",&newQuestion->answer);
        getchar();//吸收换行符
        if (newQuestion->answer==' '
		||newQuestion->answer=='\0'
		||newQuestion->answer=='\n'){
            printf("答案不能为空!请重新输入！");
            while(getchar()!='\n'); 
            free(newQuestion);
            i--; // 重新输入这道题
            continue;
		}
		newQuestion->score=eachScore;//录入每道题的分值 
		
	
	}
	//存入timu文件中
	saveToFile(head);
	printf("==========题目列表==========\n"); 
	printf("题号\t题目\t答案\t分值\n");
	printList(head);    //打印题目，检验录入结果 
	printf("录入成功并已保存到timu.txt文件！\n");
	
	return *head;
} 
	 

Question* addQuention(Question **head)//增加题目函数
{
	int i;//循环变量 
	int position;//插入位置 
	int count=0;//计算题目数量 
	Question *p=*head;//读取timu文件的题目 
	Question *tail=*head; 
	
	for(;p!=NULL;p=p->next)
	{
		count++;
		tail=p;
	}
        
    printf("当前题目数量为%d，请输入添加题目的位置(>=1)：",count);
	do{
		if(scanf("%d",&position)!=1||position<=0){//如果输入的不是数字或者数字小于等于0 
			printf("错误输入，请重新输入插入位置:");
			while(getchar()!='\n');//清空缓冲区 
			continue;
		}
		if(position>count+1){//插入的位置太后，超过了题目的数量+1 
			printf("错误输入，当前没有这么多题目！\n");
			printf("请重新输入添加题目位置：") ;
			continue;
		}
		else{
			break;
		}	
	}while(1); 
	getchar();
	Question *newQuestion=(Question*)malloc(sizeof(Question));//开辟新空间给要录入的题目 
	printf("tip:用()表示挖空处\n");
	do{
		newQuestion->next=NULL;//将next初始化为空，避免野指针 
    	if (!newQuestion) {//指针为空 
        printf("内存分配失败！无法创建题目。");
        return;
    	}
    	printf("请输入要添加的题目:");
        gets(newQuestion->question);
        if (strcmp(" ",newQuestion->question)==0){
            printf("答案不能为空!请重新输入！\n");
            free(newQuestion);
            continue;
		}
        printf("请录入正确答案: ");
        scanf("%c",&newQuestion->answer);
        getchar();//吸收换行符
        if (newQuestion->answer==' '&&newQuestion->answer=='\0'){
            printf("答案不能为空!请重新输入！\n");
            free(newQuestion);
            continue;
		}
		else break;
	}while(1);
	
	
	
	if(position==1)//头插法 
	{
		newQuestion->next=*head;
		*head=newQuestion;
	}
	
	else if(position==count+1)//尾插法 
	{
		tail->next=newQuestion;
		tail=newQuestion;
	}
	
	else{//中间位置插入 
		for(i=1,p=*head;i<position-1;i++){//找到要插入的位置 
			p=p->next;
		}
		newQuestion->next=p->next;
		p->next=newQuestion;
	} 
 
	update(head);//重新计算每题分值并重新编号  
	
	printf("==========题目列表==========\n"); 
	printList(head);    //打印题目，检验添加结果
	
	saveToFile(head);//保存到timu文件中
    printf("录入成功并已保存到timu.txt文件！\n");
	

	
	return *head;
}


Question* delQuestion(Question **head)//删除题目函数
{
	Question *temp=*head;
	Question *pr=NULL;
	int target;
	printf("请输入要删除的题号：");
	scanf("%d",&target); 
	while(temp!=NULL&&temp->num!=target){
		pr=temp;
		temp=temp->next;
	}
	if(temp==NULL){
		printf("未找到该题号！\n");
		return;
	}
	if(pr==NULL){//要找的题目再第一个 
		*head=temp->next; 
	}
	else{
		pr->next=temp->next;
	}
	free(temp);
	
	 
	update(head);//重新计算每题分值并重新编号 
	
	printf("==========题目列表==========\n"); 
	printList(head);    //打印题目，检验删除结果
	saveToFile(head);//保存到timu文件中
    printf("删除成功并已更新到timu.txt文件！\n");
    
    return *head;
	
}


void printList(Question **head)//打印题目函数
{
	Question *p=*head;
	printf("%-5s%-15s%5s%8s\n","题号","题目","答案","分值");
	for(p=*head;p!=NULL;p=p->next){
		printf("%-5d%-15s%5c%8.2f\n",p->num,p->question,p->answer,p->score);	
	}
}


void update(Question **head)//重新计算每题分值并重新编号
{
	int i;
	int count=0;//计算题目数量 
	float eachScore;//计算每道题的分值
	Question *p=*head;
	for(;p!=NULL;p=p->next){//计算题目数量 
		count++;
	}
	eachScore=100.0/count;	//插入后的每题分值
	for(p=*head,i=1;p!=NULL;p=p->next){
		p->num=i++;
		p->score=eachScore;	
	}  
}


void saveToFile(Question **head){//保存题目到文件函数
    FILE *file=fopen("timu.txt","w");
    if(file==NULL){
        printf("错误：无法打开文件进行写入！\n");
        return;
    }
    Question *p=*head;
    fprintf(file, "题号\t题目\t答案\t分值\n");
    while(p!=NULL){
        // 按格式写入：题号 题目 答案 分值
        fprintf(file, "%d\t%s\t%c\t%.2f\n", 
                p->num, p->question, p->answer, p->score);
        p=p->next;
    }
    fclose(file);
}


Question* readQuestion()//读取题目函数
{
	FILE *fp=fopen("timu.txt","r");
    if (fp==NULL){
        printf("找不到文件timu.txt");
        getchar();
        return NULL;
    } 
    
    Question *head=NULL;
    Question *pr=NULL;
    Question *temp=NULL; 
    char line[256];
    
    fgets(line,sizeof(line),fp);//跳过标题行 
    while(fgets(line,sizeof(line),fp)){
    	Question *newQuestion=(Question*)malloc(sizeof(Question));
    	newQuestion->next=NULL;
    	//读取文件信息到链表 
    	sscanf(line,"%d\t%s\t%c\t%.2f\n",
		&newQuestion->num,&newQuestion->question,&newQuestion->answer,&newQuestion->score); 

			 
		if(head==NULL){//若是头指针 
    		head=newQuestion;
    		pr=newQuestion;
    	}
		else{
    		pr->next=newQuestion;
    		pr=newQuestion;
		}

	}
	return head;
}

