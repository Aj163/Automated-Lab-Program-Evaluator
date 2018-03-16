#include "MySQL_Client.h"
#include <bits/stdc++.h>
#include <unistd.h>
using namespace std;

void uploadQuestion()
{
	system("clear");
	string readLine, questionPath, inputPath, outputPath;
	string question = "", testInput = "", testOutput = "";
	ifstream questionFile, inputFile, outputFile;

	//Question file
	printf("Enter path to question file : ");
	cin>>questionPath;
	questionFile.open(questionPath);

	while(!questionFile.eof())
	{
		getline(questionFile, readLine);
		question += readLine;
		question += "\n";
	}

	questionFile.close();

	//Input test cases file
	printf("Enter path to input test cases file : ");
	cin>>inputPath;
	inputFile.open(inputPath);

	while(!inputFile.eof())
	{
		getline(inputFile, readLine);
		question += readLine;
		question += "\n";
	}

	questionFile.close();

	//Output test case file
	printf("Enter path to question file : ");
	cin>>questionPath;
	questionFile.open(questionPath);

	while(!questionFile.eof())
	{
		getline(questionFile, readLine);
		question += readLine;
		question += "\n";
	}

	questionFile.close();
}

void getReport(){}
void viewSubmissionOfStudent(){} //Get file also

int main()
{
	uploadQuestion();
}