// #include "student.cpp"
// #include "teacher.cpp"
// #include "admin.cpp"
#include "MySQL_Client.h"
using namespace std;


int main()
{
	string username,password,choice;
	int type;

	do{

		system("clear");
		printf("\n\n\tUsername : ");
		cin>>username;
		printf("\n\tPassword : ");
		cin>>password;
		printf("\n\tType of User :\n\t\t1. Admin\n\t\t2. Teacher\n\t\t3. Student\n\t\tEnter your choice : ");
		cin>>type;

		MySQLInteract connector("127.0.0.1", "3306", "root", "root", "LOGIN");
		Admin admin(connector);
		

		switch(type){
			case 1:
				if(!admin.isUserNamePresent(username, MySQLInteract::IS_ADMIN)){
					printf("\tInvalid username\n\tHit Enter to retry");
					getchar();
					getchar();
		
				}

				else if(!admin.login(username,password,MySQLInteract::IS_ADMIN)){
					printf("\tInvalid password\n\tHit Enter to retry");
					getchar();
					getchar();			
				}

				cout<<"\tLog in Successfull";

				// admin(); calling admin fuc
				break;


			case 2:
				if(!admin.isUserNamePresent(username, MySQLInteract::IS_TEACHER)){
					printf("\tInvalid username\n\tHit Enter to retry");
					getchar();
					getchar();
		
				}

				else if(!admin.login(username,password,MySQLInteract::IS_TEACHER)){
					printf("\tInvalid password\n\tHit Enter to retry");
					getchar();
					getchar();
				}

				cout<<"\tLog in Successfull";

				// teacher(); calling teacher fuc
				break;

			case 3:
				if(!admin.isUserNamePresent(username, MySQLInteract::IS_STUDENT)){
					printf("\tInvalid username\n\tHit Enter to retry");
					getchar();
					getchar();
		
				}

				else if(!admin.login(username,password,MySQLInteract::IS_STUDENT)){
					printf("\tInvalid password\n\tHit Enter to retry");
					getchar();
					getchar();
				}

				cout<<"\tLog in Successfull";

				// student(); calling student fuc
				break;

			default:
				printf("\tInvalid Type of User\nHit Enter to retry");
				break;

		}

		printf("\n\n\tDo you want to continue ? [Y/N] \n\tEnter your choice : ");
		cin>>choice;
		if(choice.compare("N")==0 || choice.compare("n")==0){
			printf("\n\n\tProgramme Terminated\n");
			exit(0);
		}


	}while(true);

	
}