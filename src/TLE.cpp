#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

using namespace std;

#define MAXTIME 5000000

void handler(int sig){
	wait(NULL);
	cout<<"Process ended"<<endl;
}

int main(){
	pid_t pid = fork();
	if(pid==0){
		execl("/home/nitk/Desktop/simply/test","test",NULL);
		// exit(EXIT_FAILURE);
	}

	else{
		int flag = 0;
		signal(SIGCHLD,handler);
		usleep(MAXTIME);
		if(flag==0){
			if(kill(pid,0)==0){
				cout<<"Killing process"<<endl;
				kill(pid,SIGKILL);
			}
			else{
				cout<<"Process already has ended"<<endl;
			}
		}
	}
}

