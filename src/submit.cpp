#include "MySQL_Client.h"
#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
using namespace std;

#define maxNumberOfTestCases 100
#define MAXTIME 1000000
//1 second

pid_t pid=-1;

bool compareFiles(const char file1[], const char file2[])
{
	string readLineFile1, readLineFile2;
	ifstream fin1(file1), fin2(file2);

	bool flag = true;
	while(true)
	{
		fin1>>readLineFile1;
		fin2>>readLineFile2;

		if(readLineFile2.compare(readLineFile1))
		{
			flag = false;
			break;
		}

		if(fin1.eof() && fin2.eof())
			break;
		if(fin2.eof() || fin1.eof())
		{
			flag = false;
			break;
		}
	}

	fin1.close();
	fin2.close();

	return !flag;
}

void handler(int sig){
	wait(NULL);
}

int executeCode(const char runCommand[])
{
	pid = fork();
	int ret=0;
	if(pid==0)
		ret = system(runCommand);
	else
	{
		signal(SIGCHLD,handler);
		usleep(MAXTIME);

		if(kill(pid,0)==0){
			kill(pid,SIGKILL);
			return 2;
		}
		else{
			if(ret == -1)
				return 3;
			return 0;
		}
	}

	return 0;
}

void submit(string teacherID, const MySQLInteract& connector)
{
	Teacher tid(teacherID, connector);
	char user_filepath[500];
	int quesNo;
	system("clear");

	printf("Enter question number : ");
	cin>>quesNo;

	if(tid.getNoOfQuestions() < quesNo || quesNo<=0)
	{
		printf("Invalid Question Number\n");
		return;
	}

	printf("Enter relative path to file to submit : ");
	cin>>user_filepath;

	char compile[] = "gcc -o exec.out ";
	strcat(compile, user_filepath);

	if(system(compile))
	{
		printf("\n== Compilation Error ==\n");
		return;
	}

	int numberOfTestCases=2, score=0;
	int results[maxNumberOfTestCases];
		//0 - AC
		//1 - WA
		//2 - TLE
		//3 - RE

	//GET TEST CASES, and number of test cases
	//Put into folder
	string testInput = tid.getFile(quesNo, Teacher::TEST_IN_FILE);
	string testOutput = tid.getFile(quesNo, Teacher::TEST_OUT_FILE);
	
	//system("mkdir testCases");


	system("clear");
	for(int i=0; i<numberOfTestCases; i++)
	{
		char runCommand[] = "./exec.out < testCases/input";
		char tmp[5];
		tmp[0] = '0' + i/10;
		tmp[1] = '0' + i%10;
		tmp[2] = '\0';
		strcat(runCommand, tmp);
		strcat(runCommand, ".txt > output.txt");

		printf("Running on test case %2d ", i+1);
		fflush(stdout);
		for(int i=0; i<3; i++)
		{
			usleep(200000);
			printf(".");
			fflush(stdout);
		}
		printf("\n");

		int codeResult = executeCode(runCommand);
		if(pid == 0)
			return;
		if(codeResult == 3){
			printf("Runtime Error\n\n");
			results[i] = 3;
			continue;
		}
		else if(codeResult == 2){
			printf("Time Limit Exceeded\n\n");
			results[i] = 2;
			continue;
		}


		char answerPath[100] = "testCases/output";
		strcat(answerPath, tmp);
		strcat(answerPath, ".txt");

		if(compareFiles("output.txt", answerPath)){
			results[i] = 1;
			printf("Wrong Answer\n\n");
		}
		else{
			results[i] = 0;
			printf("Correct Answer\n\n");
		}

	}

	score = score*1.0/numberOfTestCases*100;

	// Files to be deleted
	system("rm exec.out");
	system("rm output.txt");
	// Folder and its files
}

int main()
{
	string x = "16CO104";

	MySQLInteract connector("127.0.0.1", "3306", "root", "root", "test");

	Teacher teacher(x,connector);
	//teacher.createTeacher(x);
	//teacher.deleteQuestion(3);
	//teacher.deleteQuestion(4);
	//teacher.addQuestion("./question.txt","./input.txt","./outputdb.txt",1);
	//teacher.addQuestion("./question.txt","./input.txt","./outputdb.txt",5);
	//teacher.addQuestion("./question.txt","./input.txt","./outputdb.txt",6);
	//cout<<teacher.questionExists(10)<<" "<<teacher.questionExists(6)<<endl;
	//cout<<teacher.getNoOfQuestions()<<endl;
	//string testInput = teacher.getFile(1, Teacher::QUESTION_FILE);
	//cout<<testInput<<endl;

	submit(x, connector);
}