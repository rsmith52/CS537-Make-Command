#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "proc_creation_prog_exe.h"

void ExecuteBuild(Spec_Representation * build_target) {

	for (int i = 0; i < build_target->num_commands; i++) {
		pid_t pid;
		int stat;

		int num_args = 0;
		for (int j = 0; j < LINE_BUFF_SIZE; j++) {
			if (build_target->commands[i][j][0] == '\0') {
				// Empty Arg
				break;
			} else {
				num_args++;
			}
		}
		char ** args = malloc(sizeof(char *) * (num_args + 1));
		for (int j = 0; j < num_args; j++) {
			args[j] = build_target->commands[i][j];
			if (j == (num_args - 1)) {
				args[j + 1] = NULL;
			}
		}
		
        	if ((pid = fork()) == 0) {
                	// Child Process
			for (int j = 0; j < num_args; j++) {
				printf("%s", args[j]);
				if (j != (num_args - 1)) {
					printf(" ");
				}
			}
			printf("\n");	
			execvp(args[0], args);
			exit(0);
        	} else {
                	// Parent Process - fork_return holds child process PID
			waitpid(pid, &stat, 0);
        	}
	}
		


	return;
}
