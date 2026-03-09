#include<stdio.h> 

int  characterMenu();//用户选择菜单 
int  teacherMenu();//教师菜单
int  studentMenu();//学生菜单


int questionMenu(); //题目管理子菜单
int searchMenu();   //查询子菜单
int sortMenu();     //排序子菜单



int  characterMenu()//用户选择菜单 
{
	int choice;
	printf("========英语练习系统========\n");
	printf("欢迎使用英语练习系统！\n");
	printf("0.退出系统\n");
	printf("1.教师\n");
	printf("2.学生\n");
	printf("请选择你的身份：");
	scanf("%d",&choice); 
	return choice;
} 


int  teacherMenu()//教师菜单 
{
	int choice;
	printf("==========教师菜单==========\n");
	printf("1.录入/管理题目\n");
	printf("2.显示全部学生成绩\n");
	printf("3.查询学生成绩\n");
	printf("4.统计各班各分数段人数\n");
	printf("5.排序\n");
	printf("6.修改密码\n");
	printf("0.返回主菜单\n");
	printf("============================\n");
	printf("请输入数字进行相应操作：");
	scanf("%d",&choice); 
	return choice;
}


int  studentMenu()//学生菜单 
{
	int choice;
	printf("==========学生菜单==========\n");
	printf("1.答题并计算成绩\n");
	printf("2.查询成绩\n");
	printf("3.错题集\n");
	printf("0.返回主菜单\n");
	printf("============================\n");
	printf("请输入数字进行相应操作：");
	scanf("%d",&choice); 
	return choice;
}



int questionMenu(){//题目管理子菜单
	int choice;
	printf("========录入/管理题目========\n");
	printf("1.录入题目\n");
	printf("2.增加题目\n");
	printf("3.删除题目\n");
	printf("0.返回主菜单\n");
	printf("============================\n");
	printf("请输入数字进行相应操作：");
	scanf("%d",&choice); 
	return choice;
}

int searchMenu(){// 查询子菜单
	int choice; 
    printf("========查询学生成绩========\n");
    printf("1. 按姓名查询\n");
    printf("2. 按班级查询\n");
    printf("3. 按专业查询\n");
    printf("0. 返回主菜单\n");
    printf("============================\n");
    printf("请输入数字进行相应操作：");
    scanf("%d",&choice); 
	return choice;
}

int sortMenu(){// 排序子菜单
	int choice; 
    printf("========学生成绩排序========\n");
    printf("1. 按成绩排序\n");
    printf("2. 按班级排序\n");
    printf("0. 返回主菜单\n");
    printf("请输入数字进行相应操作：");
    scanf("%d",&choice); 
	return choice;
}
