#include "login.cc"

int main()
{
	MySQLInteract connector("127.0.0.1", "3306", "root", "root", "LOGIN");
	login(connector);
}