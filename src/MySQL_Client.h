#ifndef __MySQLClient__
	#define __MySQLClient__



	/*
	*	Class that will bw used to communicate with
	*	MySQL server
	*/

	//Basic libraries
	#include <bits/stdc++.h>
	#include <fstream>
	#include <stdexcept>
	#include <ctime>

	//Libraries of Connetion/C++
	#include "mysql_connection.h"
	#include <boost/scoped_ptr.hpp>
	#include <cppconn/driver.h>
	#include <cppconn/exception.h>
	#include <cppconn/resultset.h>
	#include <cppconn/statement.h>
	#include <cppconn/prepared_statement.h>


	using namespace std;

	class MySQLInteract{
		private:
			string HOST, USER_NAME, PASSWORD, DATABASE, TABLE, PORT;
			string USER;

		public:
			static const int IS_ADMIN; 	  //USE THIS if type of user is ADMIN
			static const int IS_TEACHER;  //USE THIS if type of user is TEACHER
			static const int IS_STUDENT;  //USE THIS if type of user is STUDENT
			sql::Driver *driver;
			sql::Connection *con;

			MySQLInteract(string host, string port, string user, string pwd, string db){
				HOST = host;
				USER_NAME = user;
				PASSWORD = pwd;
				DATABASE = db;
				PORT = port;
						
				PORT = "3306"; //always same. Change if required.

				string hostInfo = "tcp://" + HOST + ":" + PORT;
				


				//creating connection.
				cout<<"Creating connection to server......."<<endl;
				driver = get_driver_instance();
				con = driver->connect(hostInfo,USER_NAME,PASSWORD);


				if(con->isValid()){
					cout<<"Connetction to server " + HOST + " was successfully created"<<endl;
				}
				else{
					cout<<"Connetction to server " + HOST + " could not be created"<<endl;
					cout<<"Terminating programme..."<<endl<<"Terminated"<<endl;
					exit(1);
				}

				con->setSchema(DATABASE);

			}

			
			MySQLInteract(const MySQLInteract &connector, string type){
				this->HOST = connector.getHost();
				this->USER_NAME = connector.getUserName();
				this->PASSWORD = connector.getPassword();
				this->DATABASE = type;
				this->PORT = connector.getPort();

				this->con = connector.con;
				int maxAttempt = 20;
				while(!this->con->isValid() && maxAttempt-->0){
					this->con->reconnect();
				}	
				if(maxAttempt==0){
					cout<<"Connetction to server " + HOST + " could not be created"<<endl;
					cout<<"Terminating programme..."<<endl<<"Terminated"<<endl;
					exit(1);
				}

				con->setSchema(DATABASE);


			}


			bool setUser(string user_name){
				USER = user_name;
				return true;
			}

			string getUser(){
				return USER;
			}

			string getHost() const{
				return HOST;
			}

			string getPassword() const{
				return PASSWORD;
			}

			string getPort() const{
				return PORT;
			}

			string getUserName() const{
				return USER_NAME;
			}



			~MySQLInteract(){
				// cout<<"~MySQLInteract"<<endl;
				// if(con!=NULL)
				// 	delete con;
			}
	};


	const int MySQLInteract::IS_ADMIN = 0;
	const int MySQLInteract::IS_TEACHER = 1;
	const int MySQLInteract::IS_STUDENT = 2;



			/*
			*	When DATABASE == LOGIN, and when login info needs to accessed,
			*	follwing methods are utilised.
			*/



	class Admin : public MySQLInteract{
		private:	
			sql::Statement *stmt;
		  	sql::ResultSet *res;
		  	sql::PreparedStatement *pstmt;


		public:

			Admin(const MySQLInteract& connector) : MySQLInteract(connector, "LOGIN"){

				setUser("ADMIN");
			}


			/*
			*	#TODO Checks if the USERNAME is Already taken or not.
			*	@param
			*		1.USERNAME to be checked 
			*		2.TYPE of user
			*	@return
			*		true, if USERNAME already taken.
			*		false, if not taken.
			*
			*/

			bool isUserNamePresent(string user_name,int type){
				
				string table;
				switch(type){
					case 0:
						table = "ADMIN";
						break;
					case 1:
						table = "TEACHER";
						break;
					case 2:
						table = "STUDENT";
						break;
				}


				pstmt = con->prepareStatement("SELECT ID FROM " + table + " WHERE EXISTS (SELECT username FROM " + table + " WHERE username = ? )");

				pstmt->setString(1,user_name);
				res = pstmt->executeQuery();

				return res->next() != false ? true:false;
			}


			/*
			*	#TODO Checks if the lgoin credentials are correct or not.
			*	@param
			*		1.USERNAME  
			*		2.PASSWORD
			*		3.TYPE of user
			*	@return
			*		true, if the credentials are correct.
			*		false, if not.
			*
			*/

			bool login(string user_name, string password, int type){

				string table;
				switch(type){
					case 0:
						table = "ADMIN";
						break;
					case 1:
						table = "TEACHER";
						break;
					case 2:
						table = "STUDENT";
						break;
				}

				pstmt = con->prepareStatement("SELECT password FROM " + table + " WHERE username = ? ");

				pstmt->setString(1,user_name);
				res = pstmt->executeQuery();

				delete pstmt;

				while(res->next()){
					string str = res->getString("password");
					if(password.compare(str) == 0){
						delete res;
						return true;
					}
				}

				delete res;

				return false;

			}

			/*
			*	#TODO Creates new account for the user.
			*	@param
			*		1.NEW USERNAME  
			*		2.NEW PASSWORD
			*		3.TYPE of user.
			*	@return
			*		true always.
			*
			*/

			bool createNewUser(string user_name, string password, int type){

				string table;
				switch(type){
					case 0:
						table = "ADMIN";
						break;
					case 1:
						table = "TEACHER";
						break;
					case 2:
						table = "STUDENT";
						break;
				}
				// con->reconnect();

				pstmt = con->prepareStatement("INSERT INTO " + table + " (username, password) VALUES (?,?)");
				pstmt->setString(1,user_name);
				pstmt->setString(2,password);

				pstmt->executeUpdate();
				delete pstmt;


				return true;

			}

			/*
			*	#TODO Checks if the USERNAME is valid or not.
			*	@param
			*		1.USERNAME to be checked 
			*		2.TYPE of user
			*	@return
			*		true, if USERNAME is not taken already.
			*		false, if otherwise.
			*
			*/

			bool isUserNameValid(string user_name, int type){
				

				string table;
				switch(type){
					case 0:
						table = "ADMIN";
						break;
					case 1:
						table = "TEACHER";
						break;
					case 2:
						table = "STUDENT";
						break;
				}

				pstmt = con->prepareStatement("SELECT ID FROM " + table + " WHERE EXISTS (SELECT username FROM " + table + " WHERE username = ? )");

				pstmt->setString(1,user_name);
				res = pstmt->executeQuery();

				delete pstmt;

				if(res->next()){

					delete res;
					return false;
				}

				delete res;
				return true;
			}

			~Admin(){
				// cout<<"~ADMIN"<<endl;
				// delete stmt;
				// delete res;
				// delete pstmt;
			}
	};


			/*
			*	When DATABASE == STUDENT
			*/



	class Student : public MySQLInteract{
			

		private:	
			sql::Statement *stmt;
		  	sql::ResultSet *res;
		  	sql::PreparedStatement *pstmt;

		public:

			Student(string user_name, const MySQLInteract& connector) : MySQLInteract(connector, "STUDENT"){
				setUser(user_name);
			}


			/*
			*	#TODO Creates a new TABLE with the USERNAME as its name.
			*	@param
			*		1.USERNAME
			*	@return
			*		true always.
			*
			*/

			bool createStudent(string user_name){

				pstmt = con->prepareStatement("CREATE TABLE " + user_name + " (ID INT NOT NULL AUTO_INCREMENT,Question INT NOT NULL, DateTime TEXT, Language varchar(255), File LONGTEXT, Result varchar(255), Marks INT, PRIMARY KEY(ID))");
				pstmt->executeUpdate();

				delete pstmt;

				return true;

			}


			/*
			*	#TODO Stores the contents of the file along with other info into the table.
			*	@param
			*		1.QUESTION NO for which the given file corresponds to. 
			*		2.PATH to the file
			*		3.PROGRAMMING LANGUAGE in which the file is written.
			*		4.RESULT of testing, i.e AC,WA,TLE or RE.
			*		5.MARKS obtained after testing.
			*	@return
			*		true always.
			*
			*/

			bool storeFileWithResult(int quesNo, string pathToFile, string language, string result, int marks){

				pstmt = con->prepareStatement("INSERT INTO " + getUser() + " (Question,DateTime,Language,File,Result,Marks) VALUES  (?,?,?,?,?,?)");
				
				pstmt->setInt(1,quesNo);
				
				
				//Current date/time based on current system
				time_t now = time(0);
			   	//Convert now to string form
			   	string time = ctime(&now);
				time.resize(time.length() - 1); //to remove the "\n" char.
			   	pstmt->setString(2,time);
				
				
				pstmt->setString(3,language);
				
				
				//Copying the file into a string;
				ifstream ifs (pathToFile);
				std::string content( (std::istreambuf_iterator<char>(ifs) ),(std::istreambuf_iterator<char>() ) );
				pstmt->setString(4,content);

				pstmt->setString(5,result);
				pstmt->setInt(6,marks);

				pstmt->executeUpdate();

				delete pstmt;


				return true;

			}


			/*
			*	#TODO Returns the contents of the requested file.
			*	@param
			*		1.QUESTION NO for which the requested file was submitted.
			*		2.SUBMISSION NO of the file.
			*	@return
			*		If the file is present, a string containing the contents of the requested file.
			*		NULL string otherwise.
			*
			*/

			string getFile(int quesNo, int submissionNo){

				pstmt = con->prepareStatement("SELECT File FROM " + getUser() + " WHERE Question = ? AND ID = ?");

				pstmt->setInt(1,quesNo);
				pstmt->setInt(2,submissionNo);
				res = pstmt->executeQuery();

				while(res->next()){
					string toReturn = res->getString("File");
					delete res;
					delete pstmt;

					return toReturn;
				}

				delete res;
				delete pstmt;

				return NULL;

			}


			/*
			*	#TODO Returns the submission logs for a perticular question.
			*	@param
			*		1.QUESTION NO.
			*	@return
			*		If the question is present, a string containing the Submission log.
			*		NULL string otherwise.
			*
			*/


			string getSubmissionLogs(int quesNo){

				int flag = 0;

				pstmt = con->prepareStatement("SELECT CONCAT('      ',ID,' \t|    ',DateTime,'   |\t    ',Language,'   \t|\t',Result,'\t|\t',Marks) AS _subLog FROM " + getUser() + " WHERE Question = ? ORDER BY ID ASC");
				pstmt->setInt(1,quesNo);

				res = pstmt->executeQuery();


				string toReturn = "Submission ID\t|\t    Date-Time   \t|\tLanguage\t|     Result  \t|\tMarks\n";
				while(res->next()){
					toReturn = toReturn + res->getString("_subLog") + "\n";
					flag = 1;
				}

				delete pstmt;
				delete res;

				return (flag == 1)? toReturn : NULL;

			}

			/*
			*	#TODO Returns the submission logs for the user.
			*	@param
			*		void
			*	@return
			*		If the any submission is present, a string containing the Submission log.
			*		NULL string otherwise.
			*
			*/

			string getSubmissionLogs(){

				int flag = 0;

				pstmt = con->prepareStatement("SELECT CONCAT('    ',Question,'   \t|\t    ',ID,'   \t|    ',DateTime,'   |\t    ',Language,'   \t|\t',Result,'\t|\t',Marks) AS _subLog FROM " + getUser() +" ORDER BY Question ASC, ID DESC");

				res = pstmt->executeQuery();


				string toReturn = "Question No\t|\tSubmission ID\t|\t    Date-Time   \t|\tLanguage\t|     Result  \t|\tMarks\n";
				while(res->next()){
					toReturn = toReturn + res->getString("_subLog") + "\n";
					flag = 1;
				}

				delete pstmt;
				delete res;

				return (flag == 1)? toReturn : NULL;

			}

			~Student(){
				// delete stmt;
				// delete res;
				// delete pstmt;
			}
	};



			/*
			*	When DATABASE == TEACHER	
			*/



	class Teacher : public MySQLInteract {


		private:	
			sql::Statement *stmt;
		  	sql::ResultSet *res;
		  	sql::PreparedStatement *pstmt;

		public:

			static const int QUESTION_FILE;		//USE THIS if requested file contains QUESTION
			static const int TEST_IN_FILE;		//USE THIS if requested file contains INPUT TEST CASES
			static const int TEST_OUT_FILE; 	//USE THIS if requested file contains EXPECTED OUTPUTS

			Teacher(string user_name, const MySQLInteract& connector) : MySQLInteract(connector, "TEACHER"){
				setUser(user_name);
			}


			/*
			*	#TODO Creates a new TABLE with the USERNAME as its name.
			*	@param
			*		1.USERNAME
			*	@return
			*		true always.
			*
			*/

			bool createTeacher(string user_name){

				pstmt = con->prepareStatement("CREATE TABLE " + user_name + " (Question INT NOT NULL AUTO_INCREMENT,Question_File LONGTEXT,Test_In_File LONGTEXT,Test_Out_File LONGTEXT,Marks_For_Each_Test INT, PRIMARY KEY(Question))");
				pstmt->executeUpdate();

				delete pstmt;

				return true;
			}


			/*
			*	#TODO Returns the Number of questions added by the user.
			*	@param
			*		void.
			*	@return
			*		If any question is present, an int containing the number of questions.
			*		0 otherwise.
			*
			*/

			int getNoOfQuestions(){
				
				int flag = 0;

				pstmt = con->prepareStatement("SELECT MAX(Question) AS noOfQues FROM " + getUser());

				res = pstmt->executeQuery();

				int toReturn;
				while(res->next()){
					toReturn = res->getInt("noOfQues");
					flag = 1;
				}

				delete pstmt;
				delete res;

				return (flag == 1)? toReturn : 0;

			}


			/*
			*	#TODO Returns the contents of the requested file.
			*	@param
			*		1.QUESTION NO for which the requested file was submitted.
			*		2.TYPE of the file.
			*	@return
			*		If the file is present, a string containing the contents of the requested file.
			*		NULL string otherwise.
			*
			*/


			string getFile(int quesNo, int type){
				
				string column;
				switch(type){
					case 0:
						column = "Question_File";
						break;
					case 1:
						column = "Test_In_File";
						break;
					case 2:
						column = "Test_Out_File";
						break;
				}

				pstmt = con->prepareStatement("SELECT " + column + " FROM " + getUser() + " WHERE Question = ?");
				pstmt->setInt(1,quesNo);
				res = pstmt->executeQuery();

				while(res->next()){
					string toReturn = res->getString(column);
					delete res;
					delete pstmt;

					return toReturn;
				}

				delete res;
				delete pstmt;

				return NULL;

			}


			/*
			*	#TODO Stores the contents of the files along with other info into the table.
			*	@param
			*		1.PATH to the FILE CONTAINING THE QUESTION 
			*		2.PATH to the FILE CONTATING THE INPUT TEST CASES.
			*		3.PATH to the FILE COTAINING THE EXPECTED OUTPUTS for each test case.
			*		4.MARKS alloted pers test cases.
			*	@return
			*		true always.
			*
			*/

			
			bool addQuestion(string pathToQuestionFile, string pathToInputTest, string pathToOutputTest, int marksPerTest){
				
				pstmt = con->prepareStatement("INSERT INTO " + getUser() + " (Question_File, Test_In_File, Test_Out_File, Marks_For_Each_Test) VALUES  (?,?,?,?)");
				
				

				//Copying the file into a string;
				ifstream ifs1 (pathToQuestionFile);
				std::string content1( (std::istreambuf_iterator<char>(ifs1) ),(std::istreambuf_iterator<char>() ) );

				pstmt->setString(1,content1);
				
				
				//Copying the file into a string;
				ifstream ifs2 (pathToInputTest);
				std::string content2( (std::istreambuf_iterator<char>(ifs2) ),(std::istreambuf_iterator<char>() ) );

				pstmt->setString(2,content2);
				
				
				//Copying the file into a string;
				ifstream ifs3 (pathToOutputTest);
				std::string content3( (std::istreambuf_iterator<char>(ifs3) ),(std::istreambuf_iterator<char>() ) );

				pstmt->setString(3,content3);

				pstmt->setInt(4,marksPerTest);

				pstmt->executeUpdate();

				delete pstmt;


				return true;

			}

			/*
			*	#TODO Returns the Marks aloocted per test case for a perticular question.
			*	@param
			*		1.QUESTION NO.
			*	@return
			*		If the question is present, an int containing the marks per test case for that question.
			*		0 otherwise.
			*
			*/

			int getMarksPerTest(int quesNo){

				int flag = 0;

				pstmt = con->prepareStatement("SELECT Marks_For_Each_Test FROM " + getUser() + " WHERE Question = ?");
				pstmt->setInt(1,quesNo);

				res = pstmt->executeQuery();

				int toReturn;
				while(res->next()){
					toReturn = res->getInt("Marks_For_Each_Test");
					flag = 1;
				}

				delete pstmt;
				delete res;

				return (flag == 1)? toReturn : 0;

			}

			

			~Teacher(){
				
				// delete stmt;
				// delete res;
				// delete pstmt;
			}
	};



	const int Teacher::QUESTION_FILE = 0;
	const int Teacher::TEST_IN_FILE = 1;
	const int Teacher::TEST_OUT_FILE = 2;







#endif


