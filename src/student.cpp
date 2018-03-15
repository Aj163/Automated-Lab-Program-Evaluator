#include "submitFile.cc"

void displayQuestion(Teacher teacher)
{
	system("clear");
	int quesNo;

	printf("\n\n\tTotal number of questions present : %d", teacher.getNoOfQuestions());
	printf("\n\tEnter question number : ");
	cin>>quesNo;

	if(quesNo <=0 || quesNo > teacher.getNoOfQuestions())
	{
		printf("\n\tInvalid Question Number\n\tHit Enter to continue");
		getchar();
		getchar();
		return;
	}

	system("clear");
	cout<<teacher.getFile(quesNo, Teacher::QUESTION_FILE);
	printf("\n\nHit Enter to continue");
	getchar();
	getchar();
}

void getSubmissionLogs(Student student)
{
	system("clear");
	cout<<student.getSubmissionLogs();
}

void menu(Teacher teacher, Student student)
{
	int choice;

	do
	{
		system("clear");
		printf("\n\n\t1. View Question\n\t2. Submit Code\n\t3. View Submissions");
		printf("\n\tx. Exit\n\n\tEnter your choice : ");
		cin>>choice;

		switch(choice)
		{
			case 1:
				displayQuestion(teacher);
				break;
			case 2:
				submit(teacher, student);
				break;
			case 3:
				getSubmissionLogs(student);
				break;
		}
	}while(choice >=1 && choice <=3);
}

bool askTeacherId()
{
	system("clear");

	string teacherID;
	printf("\n\tEnter teacher username conducting this test : ");
	cin>>teacherID;

	MySQLInteract connector("127.0.0.1", "3306", "root", "root", "LOGIN");
	Admin admin(connector);

	if(!admin.isUserNamePresent(teacherID, MySQLInteract::IS_TEACHER))
	{
		printf("\tInvalid teacher username\n\tHit Enter to retry");
		getchar();
		getchar();
		return false;
	}

	Teacher teacher(teacherID, connector);
	Student student("temp", connector);
	//student.createStudent("temp"); //Create temp
	printf("\n\tYou have entered the test!!\n\tHit Enter to continue");
	getchar();
	getchar();
	menu(teacher, student);
	return true;
}

int main()
{
	while(askTeacherId() == false);
}