# Automated-Lab-Program-Evaluator
Software Engineering Project

### Prerequisites
This software reqires the following pre-installed softwares and well as environment setup.

#### Pre-Installed softwares
  1. GNU g++ comiler -version 5.0 and above.
  2. GNU gcc compiler -version 5.0 and above.
  3. MySQL server -distribution 5.6 and above.
  
 #### Prerequisite environment setup.
 
 
  * The MySQL server must be preinstalled on the server side.<br/>
    ```$ sudo apt-get install mysql-server-5.7```
  * The MySQL server must be set-up to contain the following databases,<br/>
    ```$ mysql -h 127.0.0.1 -P 3306 -u root -p```
    1.  LOGIN<br/>
        ```create database LOGIN;```
    2.  TEACHER<br/>
        ```create database TEACHER;```
    3.  STUDENT<br/>
        ```create database STUDENT;```
  * The "LOGIN" database must have the following tables with their Descriptions to match that shown on the image,<br />
    ```$ use LOGIN;```
    1.  ADMIN<br />
        ```MySQL
        create table ADMIN(
            ID int not null auto_increment,
            username longtext,
            password longtext,
            primary key(ID)
        );
        ```
    ![picture alt](https://github.com/Aj163/Automated-Lab-Program-Evaluator/blob/master/Diagrams/ADMIN.png "Table ADMIN")<br />
    2.  TEACHER<br />
        ```MySQL
        create table TEACHER(
            ID int not null auto_increment,
            username longtext,
            password longtext,
            primary key(ID)
        );
        ```
    ![picture alt](https://github.com/Aj163/Automated-Lab-Program-Evaluator/blob/master/Diagrams/TEACHER.png "Table TEACHER")<br />
    3.  STUDENT<br />
        ```MySQL
        create table STUDENT(
            ID int not null auto_increment,
            username longtext,
            password longtext,
            primary key(ID)
        );
        ```
    ![picture alt](https://github.com/Aj163/Automated-Lab-Program-Evaluator/blob/master/Diagrams/STUDENT.png "Table STUDENT")
    
   
