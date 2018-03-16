#include "MySQL_Client.h"
#include <bits/stdc++.h>
#include <unistd.h>
using namespace std;

void uploadQuestion()
{
	system("clear");
	string readLine, questionPath, testCasesFolderPath, inputPath, outputPath;
	string question = "", testInput = "", testOutput = "";
	ifstream questionFile, inputFile, outputFile;
	int numberOfTestCases;

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


	printf("Enter number of test cases : ");
	cin>>numberOfTestCases;

	printf("Enter path to folder containing all test cases : ");
	cin>>testCasesFolderPath;

	testInput = to_string(numberOfTestCases);
	testInput += '\n';
	for(int i=0; i<numberOfTestCases; i++)
	{
		//Input test cases file
		inputPath = testCasesFolderPath + "/input";
		inputPath += '0'+i/10;
		inputPath += '0'+i%10;
		inputPath += ".txt";

		inputFile.open(inputPath);
		while(!inputFile.eof())
		{
			getline(inputFile, readLine);
			testInput += readLine;
			testInput += "\n";
		}

		testInput += "#\n";
		inputFile.close();

		//Input test cases file
		outputPath = testCasesFolderPath + "/output";
		outputPath += '0'+i/10;
		outputPath += '0'+i%10;
		outputPath += ".txt";

		outputFile.open(outputPath);
		while(!outputFile.eof())
		{
			getline(outputFile, readLine);
			testOutput += readLine;
			testOutput += "\n";
		}

		testOutput += "#\n";
		outputFile.close();
	}

	//Upload question, testInput, testOutput
}

void getReport(){}
void viewSubmissionOfStudent(){} //Get file also

int main()
{
	uploadQuestion();
}