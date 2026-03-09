#include<stdlib.h>

#ifndef STUDENT_H//避免出现重定义的报错 
#define STUDENT_H
//学生答案结构 
typedef struct Student
{
	char major[10];       //专业
	char  classroom[5];   //班级
	char name[10];	      //姓名 
	int  num;             //题号
	char question[15];    //题目
	char answer;          //答案
	float score;          //得分
	float totalScore;     //总分（满分100）
	struct Student *next; //指向下一个节点的指针
	 
}Student;
#endif 


#ifndef QUESTION_H
#define QUESTION_H
//题目结构 
typedef struct Question 
{
	int  num;              //题号
	char question[15];     //题目
	char answer;           //答案 
	float score;           //分值（由题目数决定）
	struct Question *next; //指向下一道题的指针
	 
}Question;
#endif 


#ifndef FUNCTIONS1_H
#define FUNCTIONS1_H
//释放题目链表内存函数 
void freeQuestionList(Question *head){
    Question *current=head;    
    Question *next=NULL;       
    
    while(current!=NULL){
        next=current->next;  
        free(current);        
        current=next;        
    }
}
#endif


#ifndef FUNCTIONS2_H
#define FUNCTIONS2_H
//释放学生链表内存函数 
void freeStudentList(Student *head){
    Student *current=head;    
    Student *next=NULL;       
    
    while(current!=NULL){
        next=current->next;  
        free(current);        
        current=next;        
    }
}
#endif
