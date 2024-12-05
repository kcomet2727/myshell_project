#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_CMD_LEN 1024

void execute_external_program(char *program, char **args) {
    pid_t pid = fork();

    if (pid == 0) { // 자식 프로세스
        if (execvp(program, args) == -1) {
            perror("exec error");
        }
        exit(1);
    } else if (pid > 0) { // 부모 프로세스
        wait(NULL); // 자식 프로세스 종료 대기
    } else {
        perror("fork error");
    }
}

int main() {
    char command[MAX_CMD_LEN];

    while (1) {
        printf("myshell> ");
        if (fgets(command, MAX_CMD_LEN, stdin) == NULL) {
            break;
        }

        command[strcspn(command, "\n")] = '\0'; // 개행 문자 제거

        // "exit" 명령 처리
        if (strncmp(command, "exit", 4) == 0) {
            printf("myshell 종료\n");
            break;
        }

        // 명령어를 공백으로 분리
        char *args[MAX_CMD_LEN / 2 + 1];
        char *token = strtok(command, " ");
        int i = 0;

        while (token != NULL) {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL;

        // 외부 명령어 실행
        execute_external_program(args[0], args);
    }

    return 0;
}
