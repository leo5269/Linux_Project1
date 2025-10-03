#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/syscall.h>
#include <stdlib.h>
#define sys_my_get_physical_addresses 450 // syscall num

void * my_get_physical_addresses(void *user_addr)
{
    unsigned long phys_addr;

    if (syscall(sys_my_get_physical_addresses, &user_addr, &phys_addr) != 0) { //invoke syscall
        perror("syscall error");
        exit(EXIT_FAILURE);  // error -> exit
    }

    return (void*)phys_addr;
}

int global_a = 123;

void hello(void) {
    printf("======================================================================================================\n");
}

int main() {
    int loc_a;
    void *parent_use, *child_use;

    printf("===========================Before Fork==================================\n");
    parent_use = my_get_physical_addresses(&global_a);
    printf("pid=%d: global variable global_a:\n", getpid());
    printf("Offset of logical address:[%p]   Physical address:[%p]\n", &global_a, parent_use);
    printf("========================================================================\n");

    if (fork()) {
        printf("vvvvvvvvvvvvvvvvvvvvvvvvvv  After Fork by parent  vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv\n");
        parent_use = my_get_physical_addresses(&global_a);
        printf("pid=%d: global variable global_a:\n", getpid());
        printf("******* Offset of logical address:[%p]   Physical address:[%p]\n", &global_a, parent_use);
        printf("vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv\n");
        wait(NULL);
    } else {
        printf("llllllllllllllllllllllllll  After Fork by child  llllllllllllllllllllllllllllllll\n");
        child_use = my_get_physical_addresses(&global_a);
        printf("******* pid=%d: global variable global_a:\n", getpid());
        printf("******* Offset of logical address:[%p]   Physical address:[%p]\n", &global_a, child_use);
        printf("llllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllll\n");
        printf("____________________________________________________________________________\n");

        /*----------------------- 觸發 CoW（Copy on Write） -----------------------------------*/
        global_a = 789;

        printf("iiiiiiiiiiiiiiiiiiiiiiiiii  Test copy on write in child  iiiiiiiiiiiiiiiiiiiiiiii\n");
        child_use = my_get_physical_addresses(&global_a);
        printf("******* pid=%d: global variable global_a:\n", getpid());
        printf("******* Offset of logical address:[%p]   Physical address:[%p]\n", &global_a, child_use);
        printf("iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii\n");
        printf("____________________________________________________________________________\n");

        sleep(1000);
    }
}
