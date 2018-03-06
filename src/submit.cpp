//#include "MySQL_Client.h"
#include <bits/stdc++.h>
#include <unistd.h>
using namespace std;

void submit()
{
	char user_filepath[500];
	int quesNo;
	system("clear");

	printf("Enter question number : ");
	cin>>quesNo;

	// if(!(VALID_QUESTION))
	// {
	// 	printf("Invalid question number\n");
	// 	GO_BACK_TO_MENU;
	// }
	// else
	// {
	// 	//Get test cases
	// 	ofstream fout("input.txt");
	// 	fout<<GET_TEST_CASE_STRING;
	// 	fout.close();
	// }

	printf("Enter relative path to file to submit : ");
	cin>>user_filepath;

	char compile[] = "gcc -o exec.out ";
	strcat(compile, user_filepath);

	if(system(compile))
	{
		printf("\n== Compilation Error ==\n");
		// 	GO_BACK_TO_MENU;
	}

	if(system("./exec.out < input.txt > output.txt"))
	{
		printf("\n== Runtime Error ==");
		// 	GO_BACK_TO_MENU;
	}

	//Checking Output
	// string correct_ans = GET_ANSWER_STRING;
	// ofstream fout("answer.txt");
	// fout<<correct_ans;
	// fout.close();

	ifstream user("output.txt"), answer("answer.txt");
	int score=0, total=0;
	char s1[5000], s2[5000];
	bool flag = true;

	while(true)
	{	
		flag = true;

		while(true)
		{
			user>>s1;
			answer>>s2;

			if(strcmp(s1, s2))
				flag = false;

			if(strcmp(s2, "#") == 0)
			{
				while(strcmp(s1, "#"))
				{
					flag = false;
					user>>s1;
				}
				break;
			}
			if(strcmp(s1, "#") == 0)
			{
				while(strcmp(s2, "#"))
				{
					flag = false;
					answer>>s1;
				}
				break;
			}
		}

		score += flag;
		total++;

		if(answer.eof())
			break;
	}

	answer.close();
	user.close();
	score = score*1.0/total*100;

	// Files to be deleted
	// exec.out
	// answer.txt
	// input.txt
	// output.txt
}

int main()
{
	submit();
}