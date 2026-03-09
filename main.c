#include"struct.c" 
#include"score.c"
#include"menu.c"
#include"question.c"

int main()
{
	Question *questionHead=readQuestion();//从timu文件读取题目 
	Student *studentHead=readStudent();//从stu文件读取学生信息 
	char name[10],cls[5],major[10];
	
	int count=0;
	char accountKey[7]="";
	char correctKey[7]="";
	//读取key文件中的密码 
	FILE *keyfile=fopen("key.txt","r"); 
	fgets(correctKey,sizeof(correctKey),keyfile);
	fclose(keyfile);

	while(1)//循环菜单 
	{
		switch(characterMenu()){
			case 0:
				//退出系统 
				printf("感谢使用英语练习系统！");
				getchar(); 
				return 0;
				
			case 1://教师
				//先验证教师身份 
				count=0;
				printf("请输入六位数密码(初始密码123456):\n");
				do{
					scanf("%s",&accountKey);
					if(strcmp(correctKey,accountKey)!=0){//如果密码错误 
						count++;
						if(count==5){
							printf("密码错误，机会已用尽，返回菜单");
							break;
						}
						printf("密码错误，请重新输入六位数密码(还剩%d次机会):",5-count);
						while(getchar()!='\n');//清空缓冲区 
						continue;
					} //end of if
					else break; 
				
				}while(1);
				if(count==5){
					break;
				}
				while(1){
					switch(teacherMenu()){
					
						case 0://返回主页 
							printf("Return...\n");
							break;
						
						case 1://录入/增删题目 
							system("cls");
							switch(questionMenu()){
							
								case 0:
									printf("Return...\n");
									break;
								
								case 1:
									//录入题目 
									system("cls");
									questionHead=entryQuention(&questionHead);
									continue;
								
								case 2:
									//增加题目 
									system("cls");
									questionHead=addQuention(&questionHead);
									continue;
								
								case 3:
									//删除题目 
									system("cls");
									questionHead=delQuestion(&questionHead);
									continue; 
								
								default:
									printf("无效选择！\n");
									getchar();
									continue; 
							} //end of questionMenu()
							continue;
						
						case 2:
							//显示所有成绩 
			 				system("cls");
			 				studentHead=showAllScores();	  
							continue;
						
						case 3:
							//查询成绩 
							system("cls"); 
							switch(searchMenu()){
								case 0: 
									printf("Return...\n");
									break;
								case 1:
									//按姓名查询 
									system("cls");
									printf("请输入要查询的学生姓名："); 
									scanf("%s",name);
									search(name,1);
									continue;
								case 2:
									//按班查询 
									system("cls");
									printf("请输入要查询的班级：");
									scanf("%s",cls);
									search(cls,1);
									continue;
								case 3:
									//按专业查询 
									system("cls");
									printf("请输入要查询的专业：");
									scanf("%s",major);
									search(major,1);
									continue; 
								default:
									printf("无效选择！\n");
									getchar();
									continue; 
							}//end of searchMenu()
							continue;
						
						case 4:
							//统计各班各分数段人数 
							system("cls"); 
							statistics();
							continue;
						
						case 5:
							//排序
							system("cls");
							switch(sortMenu()){
								case 0:
									printf("Return...\n");
									break;
								case 1:
									//按成绩排序 
									system("cls");
									studentHead=sort(1); 
									continue;
								case 2:
									//按班排序 
									system("cls");
									studentHead=sort(2); 
									continue;
								default:
									printf("无效选择！\n");
									getchar();
									continue; 
							}//end of sortMenu()
							continue;
					
						case 6://修改密码
							keyfile=fopen("key.txt","w"); 
							printf("请重新输入六位数密码：");
							do{
								scanf("%s",&correctKey);
								if(strlen(correctKey)!=6){
									printf("密码格式错误，请重新输入六位数密码:");
									while(getchar()!='\n');//清空缓冲区 
									continue;
								}
								else{
									fprintf(keyfile,"%s",correctKey);
									fclose(keyfile);
									printf("密码更改成功！");
									break; 
								} 
							}while(1); 
							break;		 
						default:
							printf("无效选择！\n");
							getchar();
							break;
						 
					}//end of teacherMenu()	
					break;
				}//end of cycle 
				break;
				
			case 2://学生 
				switch(studentMenu()){
					
					case 0:
						printf("Return...\n");
						break;
						
					case 1:
						//答题并计算成绩 
						system("cls");
						QandA();
						break;
						
					case 2:
						//按姓名查询 
						printf("请输入你的姓名："); 
						scanf("%s",name);
						search(name,0);
						break; 
						
					case 3://查看错题 
						printf("请输入你的姓名：");
						scanf("%s",&name);
						errorBook(name);
						break;
						
					default:
						printf("无效选择！\n");
						getchar();
						break;
				}//end of StudentMenu()
				break;
				
			default:
				printf("无效选择！\n");
				getchar();
				break;
			
		}//end of switch 
		
		system("pause");//让程序暂停一下 
		system("cls"); //清屏 
	}//end of cycle 
	
	//释放链表内存 
	freeStudentList(studentHead);
	freeQuestionList(questionHead); 
	return 0;
}
