#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "proc_creation_prog_exe.h"

void ExecuteBuild(Spec_Representation * build_target) {

	for (int i = 0; i < build_target->num_commands; i++) {
		int status;
		int fork_return;
		
		int num_args = 0;
		for (int j = 0; j < LINE_BUFF_SIZE; j++) {
			if (build_target->commands[i][j][0] == '\0') {
				// Empty Arg
				break;
			} else {
				num_args++;
			}
		}
		char ** args = malloc(sizeof(char *) * num_args);
		for (int j = 0; j < num_args; j++) {
			args[j] = build_target->commands[i][j];
		}
		
		fork_return = fork();
        	if (fork_return == -1) {
                	// Failed
                	fprintf(stderr, "Fork Failed\n");
        	} else if (fork_return == 0) {
                	// Child Process
			execvp(build_target->commands[i][0], args);
			exit(0);
        	} else {
                	// Parent Process - fork_return holds child process PID
			while (wait(&status) != fork_return) {
				// Wait for Child to Finish
			}
        	}
	}
		


	return;
}
