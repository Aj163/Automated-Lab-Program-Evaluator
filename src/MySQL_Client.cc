//sudo g++ -Wall -I/usr/include/cppconn MySQL_Client.h -L/usr/lib -lmysqlcppconn -std=c++11
//sudo apt-get install libmysqlcppconn-dev

bool MySQLInteract::setUser(string user_name){
	USER = user_name;
	return true;
}

string MySQLInteract::getUser() const{
	return USER;
}

string MySQLInteract::getHost() const{
	return HOST;
}

string MySQLInteract::getPassword() const{
	return PASSWORD;
}

string MySQLInteract::getPort() const{
	return PORT;
}

string MySQLInteract::getUserName() const{
	return USER_NAME;
}



MySQLInteract::~MySQLInteract(){
	// cout<<"~MySQLInteract"<<endl;
	// if(con!=NULL)
	// 	delete con;
}






// When DATABASE == LOGIN, and when login info needs to accessed, follwing methods are utilised.
 

bool Admin::isUserNamePresent(string user_name,int type){
	
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



bool Admin::login(string user_name, string password, int type){

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


bool Admin::createNewUser(string user_name, string password, int type){

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


bool Admin::isUserNameValid(string user_name, int type){
	

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

Admin::~Admin(){
	// cout<<"~ADMIN"<<endl;
	// delete stmt;
	// delete res;
	// delete pstmt;
}



// When DATABASE == STUDENT


bool Student::createStudent(string user_name){

	pstmt = con->prepareStatement("CREATE TABLE " + user_name + " (ID INT NOT NULL AUTO_INCREMENT,Question INT NOT NULL, DateTime TEXT, Language varchar(255), File LONGTEXT, Result varchar(255), Marks INT, PRIMARY KEY(ID))");
	pstmt->executeUpdate();

	delete pstmt;

	return true;

}



bool Student::storeFileWithResult(int quesNo, string pathToFile, string language, string result, int marks){

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


string Student::getFile(int quesNo, int submissionNo){

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



string Student::getSubmissionLogs(int quesNo){

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


string Student::getSubmissionLogs(){

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

Student::~Student(){
	// delete stmt;
	// delete res;
	// delete pstmt;
}



// When DATABASE == TEACHER	

bool Teacher::createTeacher(string user_name){

	pstmt = con->prepareStatement("CREATE TABLE " + user_name + " (Question INT NOT NULL AUTO_INCREMENT,Question_File LONGTEXT,Test_In_File LONGTEXT,Test_Out_File LONGTEXT,Marks_For_Each_Test INT, PRIMARY KEY(Question))");
	pstmt->executeUpdate();

	delete pstmt;

	return true;
}



int Teacher::getNoOfQuestions(){
	
	// int flag = 0;

	// pstmt = con->prepareStatement("SELECT MAX(Question) AS noOfQues FROM " + getUser());

	// res = pstmt->executeQuery();

	// int toReturn;
	// while(res->next()){
	// 	toReturn = res->getInt("noOfQues");
	// 	flag = 1;
	// }

	pstmt = con->prepareStatement("SELECT Question FROM " + getUser());
	res = pstmt->executeQuery();
	int cnt = 0;
	while(res->next()){
		cnt++;
	}
	delete pstmt;
	delete res;

	return cnt;

}



string Teacher::getFile(int quesNo, int type){

	pstmt = con->prepareStatement("SELECT Question FROM " + getUser());
	res = pstmt->executeQuery();
	int cnt = 0,realQuesNo = -1;
	while(cnt<quesNo && res->next()){
		realQuesNo = res->getInt("Question");
		cnt++;
	}

	if(quesNo != cnt)
		return NULL;
	
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
	pstmt->setInt(1,realQuesNo);
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



bool Teacher::addQuestion(string pathToQuestionFile, string pathToInputTest, string pathToOutputTest, int marksPerTest){
	
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


int Teacher::getMarksPerTest(int quesNo){

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

bool Teacher::questionExists(int quesNo){
	pstmt = con->prepareStatement("SELECT Question FROM " + getUser());
	res = pstmt->executeQuery();

	while(res->next()){
		if(quesNo == res->getInt("Question")){
			return true;
		}
	}
	return false;
}

bool Teacher::deleteQuestion(int quesNo){

	pstmt = con->prepareStatement("DELETE FROM " + getUser() + " WHERE Question = ?");
	pstmt->setInt(1,quesNo);

	pstmt->executeUpdate();
	return true;
}
	


Teacher::~Teacher(){
	
	// delete stmt;
	// delete res;
	// delete pstmt;
}
