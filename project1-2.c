#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <stdlib.h>

#define sys_my_get_physical_addresses 450  // invoke syscall

void * my_get_physical_addresses(void *user_addr)
{
    unsigned long phys_addr;

    if (syscall(sys_my_get_physical_addresses, &user_addr, &phys_addr) == -1) {
        perror("syscall error");
        exit(EXIT_FAILURE);
    }

    return (void*)phys_addr;
}

int a[2000000];  // global array

int main()
{
    int loc_a;
    void *phy_add;

    // get a[0] physical address
    phy_add = my_get_physical_addresses(&a[0]);
    printf("global element a[0]:\n");
    printf("Offset of logical address: [%p]   Physical address: [%p]\n", &a[0], phy_add);
    printf("========================================================================\n");

    // get a[1999999] physical address
    phy_add = my_get_physical_addresses(&a[1999999]);
    printf("global element a[1999999]:\n");
    if(phy_add == 0)
            printf("Offset of logical address: [%p]   Physical address: 0\n", &a[1999999]);
    else
            printf("Offset of logical address: [%p]   Physical address: [%p]\n", &a[1999999], phy_add);
    printf("========================================================================\n");

    return 0;
}
