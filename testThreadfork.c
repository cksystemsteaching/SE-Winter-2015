int global = 1;

int main() {
	int pid;
	pid = threadfork();
	if (pid == -1)
		exit(-1);
	else if (pid > 0){//main thread
		wait(pid);		
	} else { //thread 1
		global = global + 1;
		exit(global - 1);
	}
	
	pid = threadfork();
	if (pid == -1)
		exit(-1);
	else if (pid > 0){//main thread
		wait(pid);		
	} else { //thread 2
		global = global + 1;
		exit(global - 1);
	}
	
	pid = threadfork();
	if (pid == -1)
		exit(-1);
	else if (pid > 0){//main thread
		wait(pid);		
	} else { //thread 3
		global = global + 1;
		exit(global - 1);
	}
	
	
	exit(0);
}
