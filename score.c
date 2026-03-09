#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"struct.c"

void search();        //查询 

Student* sort(int choice);             				//排序主函数
Student* sortByScore(Student **head,int choice);   	//按成绩排序 
Student* sortByClass(Student **head);   			//按班级排序 
void saveSortedResult(FILE *file,Student *head);	//保存排序结果

Student* readStudent();		//读取学生成绩函数 
Student* showAllScores();  	//显示所有成绩函数 
void statistics();      	//统计各班各分数段人数 
void QandA();           	//答题并计算成绩

void swapStudentData(Student **a, Student **b);//交换节点数据
void errorBook(char *name);//错题集 


void search(char *target,int choice)//查询
{
	FILE *fp=fopen("stu.txt","r");
    if (fp==NULL){
        printf("找不到文件 stu.txt");
        getchar();
        return;
    }
    
    int flag=0;//查找标志
    int classScore[5]={0};
    int i;
    
	Student *head=readStudent(); 
	Student *p=head;
	Student targetStudent={0};//初始化 

	printf("%-15s %-5s %-10s %-8s\n", 
        	"专业", "班级", "姓名", "总分");
    for(;p!=NULL;p=p->next){
    	//如果找到查找目标 
    	if(strcmp(p->name,target)==0
		||strcmp(p->classroom,target)==0
		||strcmp(p->major,target)==0){
		flag=1; 
		//输出该学生信息
		printf("%-15s %-5s %-10s %-8.2f\n",
				p->major, p->classroom,p->name,p->totalScore); 
		//复制学生信息到 targetStudent以便后面统计该学生所在班级的各分数段人数 
		strcpy(targetStudent.major,p->major);
		strcpy(targetStudent.classroom,p->classroom);
		strcpy(targetStudent.name,p->name);
		targetStudent.totalScore=p->totalScore;
		}
	}
    if(flag==1){//如果有查到要找的学生 
    	//循环成绩列表统计该学所在班级各分数段人数 
    	for(p=head;p!=NULL;p=p->next){
    		if(strcmp(targetStudent.classroom,p->classroom)==0){
    			switch((int)(p->totalScore/10)){
    				case 10:
    				case 9://90-100
    					classScore[4]++;
    					break;
    				case 8://80-90
    					classScore[3]++;
    					break;
    				case 7://70-80
    					classScore[2]++;
    					break;
    				case 6://60-70
    					classScore[1]++;
    					break;
    				default://60以下
    					classScore[0]++;
    					break;
				}//end of switch
				
			}//end of if
    		
		}//end of for
		if(choice==1){//choice=1表示教师，choice=0表示学生 ，学生只显示个人成绩 
			printf("%s班成绩：\n",targetStudent.classroom);
			printf("60以下：%d人\n",classScore[0]);
			printf("60-70：%d人\n",classScore[1]);
			printf("70-80：%d人\n",classScore[2]);
			printf("80-90：%d人\n",classScore[3]);
			printf("90-100：%d人\n",classScore[4]);
		}

	} 
	else if(flag==0){
		printf("未找到相关信息！\n");
		return;
	}
} 


Student* readStudent()//读取学生成绩函数
{
	FILE *fp=fopen("stu.txt","r");
    if (fp==NULL){
        printf("找不到文件 stu.txt");
        getchar();
        return NULL;
    } 
    
    Student *head=NULL;
    Student *pr=NULL;
    Student *temp=NULL; 
    char line[256];
    
    fgets(line,sizeof(line),fp);//跳过标题行 
    while(fgets(line,sizeof(line),fp)){
    	Student *stu=(Student*)malloc(sizeof(Student));
    	stu->next=NULL;
    	//读取文件信息到链表 
    	sscanf(line,"%15s %5s %10s %5d %15s %c%f%f",
		&stu->major,&stu->classroom,&stu->name,&stu->num,
		&stu->question,&stu->answer,&stu->score,&stu->totalScore); 
		if(strcmp(stu->major,"专业")==0){//如果是标题行 
			free(stu);
			continue;
		}
			 
		if(head==NULL){
    		head=stu;
    		pr=stu;
    	}
    	else{
    		if(strcmp(pr->name,stu->name)==0){
    			pr->num=stu->num;
    			strcpy(pr->question,stu->question);
    			pr->answer=stu->answer;
    			pr->score=stu->score;
    			pr->totalScore=stu->totalScore;
    			free(stu);
    		}
    		else {
    			pr->next=stu;
    			pr=stu;
			}
		}	 

	}
	return head;
}
 
 
Student* sort(int choice)//排序主函数 
{
	//从stu.txt文件读取学生成绩,返回学生成绩链表 
	Student *studentList=readStudent();
	int flag;//排序方式 
	//链表冒泡排序（成绩或班级） 
	 if(choice==1){
	 	printf("1.降序：\n"); 
	 	printf("2.升序：\n"); 
	 	printf("请选择排序方式："); 
		if(scanf("%d",&flag)!=1||flag!=1&&flag!=2){
		 	printf("错误选择！返回菜单\n"); 
		 	return studentList;
		 } 
		 else{
			studentList=sortByScore(&studentList,flag);
		}
	}
	else{
	 	studentList=sortByClass(&studentList);
	}
	return studentList;
}


Student* sortByScore(Student **head,int choice)//按成绩排序 
{
	int flag;//交换标志 
    Student *current;
    Student *sorted=NULL; //用于标记已排序部分，循环每次遇到已标记部分就不再往下比较 
	if(*head==NULL||(*head)->next==NULL){
        return; //链表为空/只有一个节点，没必要排
    }
	
	if(choice==1){//按成绩降序排序 
		do{
			flag=0;
    		current=*head;
       		 while(current->next!=sorted){
       		 	//比较相邻节点的成绩
        	    if(current->totalScore<current->next->totalScore){
         	       swapStudentData(&current,&current->next);//交换数据
         	       flag=1;
         	   	}
        	    current=current->next;
        	}
        	sorted=current;//最后交换的节点是已排序的   
    	} while (flag); //如果不再交换，说明已排完
    
   		 for(current=*head;current!=NULL;current=current->next){//输出排序后的结果 
    		printf("%-15s %-5s %-10s %-8.2f\n", 
      	      		current->major,current->classroom,current->name,current->totalScore);
		}
	}
	
	else{//按成绩升序排序 
		do{
			flag=0;
    		current=*head;
        	while(current->next!=sorted){
            	if(current->totalScore>current->next->totalScore){//比较相邻节点的成绩
              	  swapStudentData(&current,&current->next);//交换数据
               	 	flag=1;
            	}
            	current=current->next;
        	}
        	sorted=current;//最后交换的节点是已排序的   
    	} while (flag); //如果不再交换，说明已排完
    
    	for(current=*head;current!=NULL;current=current->next){//输出排序后的结果 
    		printf("%-15s %-5s %-10s %-8.2f\n", 
            		current->major,current->classroom,current->name,current->totalScore);
		}
	}
    
    
    //保存排序结果到文件 
    FILE *file=fopen("sort1.txt","w");
    if(file==NULL){
        printf("错误：无法打开文件进行写入！\n");
       	return;
    }
    saveSortedResult(file,*head);//保存排序结果
    
    return *head; 
}

 
 

Student* sortByClass(Student **head)//按班级排序 
{
	int flag;//交换标志 
    Student *current;
    Student *sorted=NULL;//用于标记已排序部分，循环每次遇到已标记部分就不再往下比较 
	if(*head==NULL||(*head)->next==NULL){
        return; //链表为空/只有一个节点，没必要排
    }
    

    do{
		flag=0;
        current=*head;
        while(current->next!=sorted){
        	//用atoi将班级的数字型字符串转化成整型再比较
			if(atoi(current->classroom)-atoi(current->next->classroom)>0){ 
                swapStudentData(&current,&current->next);
                flag=1;
            }
            current=current->next;
        }
        sorted=current;//最后交换的节点是已排序的   
    } while (flag); //如果不再交换，说明已排完
    
    for(current=*head;current!=NULL;current=current->next){//输出排序后的结果 
    	printf("%-15s %-5s %-10s %-8.2f\n", 
            	current->major,current->classroom,current->name,current->totalScore);
	}
    
    //保存排序结果到文件 
    FILE *file=fopen("sort2.txt","w");
    if(file==NULL){
        printf("错误：无法打开文件进行写入！\n");
       	return;
    }
    saveSortedResult(file,*head);//保存排序结果
    
    return *head;
}
 
 
void saveSortedResult(FILE *file,Student *head)//保存排序结果
{

    Student *p=head;
    //写入文件标题
    fprintf(file, "%-15s %-5s %-10s %-8s\n", 
                      "专业", "班级", "姓名", "总分");
    while(p!=NULL){
         // 将排序好的成绩写入文件 
        fprintf(file, "%-15s %-5s %-10s %-8.2f\n", 
                       p->major,p->classroom,p->name,p->totalScore);
        p=p->next;
    }
    fclose(file);
	printf("已成功保存排序结果到文件中。\n");	
}  
 
 
 void statistics()//统计各班各分数段人数 
{
	Student *head=readStudent();//从stu.txt文件读取学生成绩
	Student *p=head; 
	int classScore[10][6]={0};
	int i;
	int flag; 
	for(;p!=NULL;p=p->next){
		flag=0;
		for(i=0;classScore[i][0]!=0&&i<10;i++){
			//atoi函数用于将数字字符型的字符串转化成int型(包含在stdlib库函数中)
			if(atoi(p->classroom)==classScore[i][0]) 
			{
				flag=1;
				break;	
			}
		}
		if(i==10){
				printf("抱歉，已超出最大统计班级个数\n");
				return;
		}
		switch(flag){
			case 0:
				classScore[i][0]=atoi(p->classroom);
			case 1:
				switch((int)(p->totalScore/10)){
					case 10:
    				case 9://90-100
    					classScore[i][5]++;
    					break;
    				case 8://80-90
    					classScore[i][4]++;
    					break;
    				case 7://70-80
    					classScore[i][3]++;
    					break;
    				case 6://60-70
    					classScore[i][2]++;
    					break;
    				default://60以下
    					classScore[i][1]++;
    					break;
				}
				
		}
		
	}
	for(i=0;classScore[i][0]!=0;i++){
		printf("%d班成绩：\n",classScore[i][0]);
		printf("60以下：%d人\n",classScore[i][1]);
		printf("60-70：%d人\n",classScore[i][2]);
		printf("70-80：%d人\n",classScore[i][3]);
		printf("80-90：%d人\n",classScore[i][4]);
		printf("90-100：%d人\n",classScore[i][5]);
	}
	
}



void QandA()//答题并计算成绩 
{
	FILE *fp=fopen("timu.txt","r");
    if (fp==NULL){
        printf("找不到文件 timu.txt");
        getchar();
        return;
    }
    char line[256];
    int questionNum;
    char question[50],answer,studentAnswer;
    float score;
    char major[10],name[10],cls[5];
    float obtainedScore,totalScore=0.0; 
    //打开 stu.txt写入答案
    FILE *stuFile=fopen("stu.txt","a");//先以追加模式打开 
    if (stuFile==NULL){//如果一开始没有该文件，则会返回空指针 
    	//以write模式打开文件，如果一开始没有该文件，则会创建一个文件
    	stuFile=fopen("stu.txt","w");
    	if (stuFile==NULL){
        	printf("无法创建文件 stu.txt");
        	getchar();
        	fclose(fp);
        	return;
    	}
    }
    //写入stu.txt的标题
    fprintf(stuFile, "%-15s %-5s %-10s %-5s %-15s %-5s %-8s %-8s\n", 
                      "专业", "班级", "姓名", "题号", "题目", "答案", "得分", "总分");
    printf("====================\n");
    //输入学生信息 
	printf("请输入你的专业:"); 
	scanf("%s",major);
    printf("请输入你的班级:");
    scanf("%s", &cls);
    printf("请输入你的姓名:");
    scanf("%s",name);
    printf("====================\n");
	printf("答题开始：\n"); 
	fgets(line,sizeof(line),fp);//跳过标题行 
	while (fgets(line,sizeof(line),fp)){
        // 解析文件中每行的题目信息
        //%[^\t]扫描集，获取非制表符的字符，遇到制表符后停止
        sscanf(line, "%d %[^\t] %c %f", &questionNum, question, &answer, &score); 
        printf("%d.%s:\n", questionNum,question);
        scanf(" %c", &studentAnswer);
        // 计算本题得分
        obtainedScore=(studentAnswer == answer)?score:0.0;
		totalScore+=obtainedScore;//累加总得分 
		if(totalScore>100){
			//避免因在计算每道题目分值时四舍五入导致的最后总分可能大于100的情况 
			totalScore=100;
		}
        // 将学生信息、题目、答案、得分写入 stu.txt
        fprintf(stuFile, "%-15s %-5s %-10s %-5d %-15s %-5c %-8.2f %-8.2f\n", 
                       major, cls, name, questionNum, question, studentAnswer, obtainedScore,totalScore);
    }
	printf("====================\n");
    printf("答题结束！总分为%.2f/100\n",totalScore);
    fclose(fp);
    fclose(stuFile);
}
 


Student* showAllScores()//显示所有成绩
{
	Student *p=readStudent();//从stu.txt文件读取学生成绩
	printf("%-15s %-5s %-10s %-8s\n", 
        	"专业", "班级", "姓名",  "总分");
	for(;p!=NULL;p=p->next){//循环输出学生成绩 
		printf("%-15s %-5s %-10s %-8.2f\n",
		p->major,p->classroom,p->name,p->totalScore);
	}
	return p; 
}



void swapStudentData(Student **a, Student **b){//交换两个节点的数据(不改变指针连接)
    Student temp;
    
    // 复制a到temp
    strcpy(temp.major,(*a)->major);
    strcpy(temp.classroom,(*a)->classroom);
    strcpy(temp.name,(*a)->name);
    temp.num=(*a)->num;
    strcpy(temp.question,(*a)->question);
    temp.answer=(*a)->answer;
    temp.score=(*a)->score;
    temp.totalScore=(*a)->totalScore;
    
    // 复制b到a
    strcpy((*a)->major, (*b)->major);
    strcpy((*a)->classroom, (*b)->classroom);
    strcpy((*a)->name, (*b)->name);
    (*a)->num = (*b)->num;
    strcpy((*a)->question, (*b)->question);
    (*a)->answer = (*b)->answer;
    (*a)->score = (*b)->score;
    (*a)->totalScore = (*b)->totalScore;
    
    // 复制temp到b
    strcpy((*b)->major,temp.major);
    strcpy((*b)->classroom,temp.classroom);
    strcpy((*b)->name,temp.name);
    (*b)->num=temp.num;
    strcpy((*b)->question,temp.question);
    (*b)->answer=temp.answer;
    (*b)->score=temp.score;
    (*b)->totalScore=temp.totalScore;
}

void errorBook(char *name)//错题集
{
	FILE *fp=fopen("stu.txt","r");//打开学生文件 
    if (fp==NULL){
        printf("找不到文件 stu.txt");
        getchar();
        return;
    }
    
	Student *head=NULL;
    Student *pr=NULL;
	Student *p=NULL; 
    char line[256];
	int flag1=0,flag2=0;
	
	//读取文件信息 
	fgets(line,sizeof(line),fp);//跳过标题行 
    while(fgets(line,sizeof(line),fp)){
    	Student *stu=(Student*)malloc(sizeof(Student));
    	stu->next=NULL;
    	//读取文件信息到链表 
    	sscanf(line,"%15s %5s %10s %5d %15s %c%f%f",
		&stu->major,&stu->classroom,&stu->name,&stu->num,
		&stu->question,&stu->answer,&stu->score,&stu->totalScore); 
		if(strcmp(stu->major,"专业")==0){//如果是标题行 
			free(stu);
			continue;
		}	 
		if(head==NULL){//如果是头指针 
    		head=stu;
    		pr=stu;
    	}
    	else{
    			pr->next=stu;
    			pr=stu;	
		}	 
	}
	
	
	printf("错题集：\n");
	printf("%-15s %-5s %-10s %-5s %-15s %-5s %-8s\n", 
            "专业", "班级", "姓名", "题号", "题目", "答案", "得分"); 
	for(p=head;p!=NULL;p=p->next){
		if(strcmp(name,p->name)==0){//寻找学生姓名 
			flag1=1;
			if(p->score==0){//查找该学生的错题 
				flag2=1;
				printf("%-15s %-5s %-10s %-5d %-15s %-5c %-8.2f\n", 
                       p->major, p->classroom, p->name, p->num, p->question,
					    p->answer, p->score);
			}
		}
	}
	if(flag1==1&&flag2==0){//找到该学生但未找到该学生错题 
			printf("全对，真棒！");
	}
	else if(flag1==0){
		printf("未找到该学生答题信息。\n"); 
	} 
}
 
