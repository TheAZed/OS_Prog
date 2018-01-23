//
// Created by A.Zed on 1/23/2018.
//

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>

void do_loop();
char* get_next_line();
char** extract_args(char* line);
int z_exec(char** args, FILE* temp_input);
int execute(char** args);
int total_custom_funcs();
int z_cd(char** args);
int z_exit(char** args);

char *custom_func_names[] = {"cd", "exit"};
int (*custom_funcs[]) (char**) = {&z_cd, &z_exit};


int main(int argc, char **argv){

    do_loop();

    return 0;
}

void do_loop(){
    char* current_line;
    char** args;
    int status;

    do{
        printf("ZZZ ");

	current_line = get_next_line();
        args = extract_args(current_line);
        status = execute(args);
    }while(status);

}

char* get_next_line(){
    char* line;
    size_t line_len = 2048;
    line = (char *) malloc(line_len * sizeof(char));
    getline(&line, &line_len, stdin);
	printf("TheLine: %s \n", line);
    return line;
}

char** extract_args(char* line){
    printf("Fuck");    
int total_args = 64;
    char **args = (char**)malloc(total_args * sizeof(char*));
    char *current_arg;
    int i = 0;

    current_arg = strtok(line, " \t\r\n\a");
    printf("arg :%s", current_arg);

    while(current_arg != NULL){

        args[i] = current_arg;
        i++;
        current_arg = strtok(NULL, " \t\r\n\a");
    }
    args[i] = NULL;
    return args;
}

int z_exec(char** args, FILE* temp_input){
    pid_t pid, w_pid;
    int status, found_redirection = 0, filedes[2], fileref[2];
    int token_count = sizeof(args) / sizeof(char*);
    int redirect_index, redirect_type = -1;

    pipe(filedes);
    pipe(fileref);

    for(redirect_index = 0; redirect_index < token_count; redirect_index++){
        if(strcmp(args[redirect_index], ">")){
            redirect_type = 0;
            break;
        }
        else if(strcmp(args[redirect_index], "|")){
            redirect_type = 1;
            break;
        }else if(strcmp(args[redirect_index], "<")){
            redirect_type = 2;
            break;
        }
    }

    if(redirect_type == 0){
        pid = fork();
        if(pid == 0){
            while ((dup2(filedes[1], STDOUT_FILENO) == -1) && (errno == EINTR));
            if(temp_input != NULL){
                while (1) {
                    char buffer[2048];
                    fscanf(temp_input,"%s", buffer);
                    ssize_t count = write(fileref[1], buffer, sizeof(buffer));
                    if (count == -1) {
                        if (errno == EINTR) {
                            continue;
                        } else {
                            perror("read");
                            exit(1);
                        }
                    } else if (count == 0) {
                        break;
                    }
                }
            }
            while ((dup2(fileref[0], STDIN_FILENO) == -1) && (errno == EINTR)){}
            close(fileref[1]);
            close(filedes[1]);
            close(filedes[0]);
            int i = 0;
            char** temp_args = (char**)malloc(sizeof(args));
	    for(i = 0; i < redirect_index; i++){
                temp_args[i] = args[i];
            }
            temp_args[i] = NULL;
            execvp(args[0], temp_args);
            exit(EXIT_FAILURE);
        } else if(pid < 0){
            perror("Failure in forking!");
        } else {
            char buffer[2048];
            FILE *target = fopen(args[redirect_index + 1], "w");
            while (1) {
                ssize_t count = read(filedes[0], buffer, sizeof(buffer));
                if (count == -1) {
                    if (errno == EINTR) {
                        continue;
                    } else {
                        perror("read");
                        exit(1);
                    }
                } else if (count == 0) {
                    break;
                } else {
                    fprintf(target, "%s", buffer);
                }
            }
            fclose(target);
            close(filedes[0]);
            do {
                w_pid = waitpid(pid, &status, WUNTRACED);
            } while (!WIFEXITED(status) && !WIFSIGNALED(status));
        }
    }else if(redirect_type == 1){
        pid = fork();
        if(pid == 0){
            while ((dup2(filedes[1], STDOUT_FILENO) == -1) && (errno == EINTR));
            if(temp_input != NULL){
                while (1) {
                    char buffer[2048];
                    fscanf(temp_input,"%s", buffer);
                    ssize_t count = write(fileref[1], buffer, sizeof(buffer));
                    if (count == -1) {
                        if (errno == EINTR) {
                            continue;
                        } else {
                            perror("read");
                            exit(1);
                        }
                    } else if (count == 0) {
                        break;
                    }
                }
            }
            while ((dup2(fileref[0], STDIN_FILENO) == -1) && (errno == EINTR)){}
            close(fileref[1]);
            close(filedes[1]);
            close(filedes[0]);
            int i = 0;
            char** temp_args = (char**)malloc(sizeof(args));
            for(i = 0; i < redirect_index; i++){
                temp_args[i] = args[i];
            }
            temp_args[i] = NULL;
            execvp(args[0], temp_args);
            exit(EXIT_FAILURE);
        } else if(pid < 0){
            perror("Failure in forking!");
        } else {
            char buffer[2048];
            char* name = "temp_file_for_p_";
            char buff[20];
            sprintf(buff, "%d", (int)(pid));
            strcat(name, buff);
            FILE *target = fopen(name, "w+");
            while (1) {
                ssize_t count = read(filedes[0], buffer, sizeof(buffer));
                if (count == -1) {
                    if (errno == EINTR) {
                        continue;
                    } else {
                        perror("read");
                        exit(1);
                    }
                } else if (count == 0) {
                    break;
                } else {
                    fprintf(target, "%s", buffer);
                }
            }
            close(filedes[0]);
            do {
                w_pid = waitpid(pid, &status, WUNTRACED);
            } while (!WIFEXITED(status) && !WIFSIGNALED(status));
            char** other_args = (char**)malloc(sizeof(args));
            int i = 0;
	    for(i = redirect_index + 1; i < sizeof(args) / sizeof(char*); i++){
                other_args[i - redirect_index - 1] = args[i];
            }
            rewind(target);
            z_exec(other_args, target);
            fclose(target);
            remove(name);
        }
    }else if(redirect_type == 2){
        FILE* input_file = fopen(args[redirect_index + 1], "r");
        char** new_args = (char**)malloc(sizeof(args));
        int i = 0;
        for(i = 0; i < redirect_index; i++){
            new_args[i] = args[i];
        }
        new_args[i] = NULL;
        z_exec(new_args, input_file);
        fclose(input_file);
    }else{
        pid = fork();
        if(pid == 0){
//            while ((dup2(filedes[1], STDOUT_FILENO) == -1) && (errno == EINTR));
            if(temp_input != NULL){
                while (1) {
                    char buffer[2048];
                    fscanf(temp_input,"%s", buffer);
                    ssize_t count = write(fileref[1], buffer, sizeof(buffer));
                    if (count == -1) {
                        if (errno == EINTR) {
                            continue;
                        } else {
                            perror("read");
                            exit(1);
                        }
                    } else if (count == 0) {
                        break;
                    }
                }
                while ((dup2(fileref[0], STDIN_FILENO) == -1) && (errno == EINTR)){}
            }
            close(fileref[1]);
//            close(filedes[1]);
//            close(filedes[0]);
            int i = 0;
            char** temp_args = (char**)malloc(sizeof(args));
            for(i = 0; i < redirect_index; i++){
                temp_args[i] = args[i];
            }
            temp_args[i] = NULL;
            execvp(args[0], temp_args);
            exit(EXIT_FAILURE);
        } else if(pid < 0){
            perror("Failure in forking!");
        } else {
//            char buffer[2048];
//            FILE *target = fopen(args[redirect_index + 1], "w");
//            while (1) {
//                ssize_t count = read(filedes[0], buffer, sizeof(buffer));
//                if (count == -1) {
//                    if (errno == EINTR) {
//                        continue;
//                    } else {
//                        perror("read");
//                        exit(1);
//                    }
//                } else if (count == 0) {
//                    break;
//                } else {
//                    fprintf(target, buffer);
//                }
//            }
//            fclose(target);
            close(filedes[0]);
            do {
                w_pid = waitpid(pid, &status, WUNTRACED);
            } while (!WIFEXITED(status) && !WIFSIGNALED(status));
        }
    }

//    pid = fork();
//    if(pid == 0){
//        execvp(args[0], args);
//        exit(EXIT_FAILURE);
//    } else if(pid < 0){
//        perror("Failure in forking!");
//    } else {
//        do {
//            w_pid = waitpid(pid, &status, WUNTRACED);
//        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
//    }

    return 1;
}


int total_custom_funcs(){
    return sizeof(custom_func_names) / sizeof(char*);
}

int z_cd(char **args){
    if(args[1] == NULL){
        fprintf(stderr, "Argument Expected for \"cd\" command \n");
    }else{
        chdir(args[1]);
    }
    return 1;
}

int z_exit(char **args){
    return 0;
}

int execute(char ** args){
    int i;

    if (args[0] == NULL) {
        // An empty command was entered.
        return 1;
    }

    for (i = 0; i < total_custom_funcs(); i++) {
        if (strcmp(args[0], custom_func_names[i]) == 0) {
            return (*custom_funcs[i])(args);
        }
    }

    return z_exec(args, NULL);
}


