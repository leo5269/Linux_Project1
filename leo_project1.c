#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/mm.h>
#include <linux/sched.h>
#include <linux/uaccess.h>

// define my_get_physical_addresses
SYSCALL_DEFINE2(my_get_physical_addresses,  void __user *, va, unsigned long __user *, pa)
{
    struct mm_struct *mm = current -> mm;
    pgd_t *pgd;
    p4d_t *p4d;
    pud_t *pud;
    pmd_t *pmd;
    pte_t *pte;
    unsigned long phys_addr = 0;
    struct page *page;
    unsigned long kernel_addr;  // save user address

    printk(KERN_ALERT "my_get_physical_addresses system call invoked\n");

    // use copy_from_user to get user address
    if (copy_from_user(&kernel_addr, va, sizeof(kernel_addr)))
       return -EFAULT;

    // query page
    pgd = pgd_offset(mm, kernel_addr);
    if (pgd_none(*pgd) || pgd_bad(*pgd))
        goto out;

    p4d = p4d_offset(pgd, kernel_addr);
    if (p4d_none(*p4d) || p4d_bad(*p4d))
        goto out;

    pud = pud_offset(p4d, kernel_addr);
    if (pud_none(*pud) || pud_bad(*pud))
        goto out;

    pmd = pmd_offset(pud, kernel_addr);
    if (pmd_none(*pmd) || pmd_bad(*pmd))
        goto out;

    pte = pte_offset_map(pmd, kernel_addr);
    if (!pte || pte_none(*pte) || !pte_present(*pte)){
        if(pte)           pte_unmap(pte);
        goto out; // Page not present, phys_addr remains 0
    }
    printk(KERN_ALERT "PGD: %lx\n", pgd_val(*pgd));
    printk(KERN_ALERT "P4D: %lx\n", p4d_val(*p4d));
    printk(KERN_ALERT "PUD: %lx\n", pud_val(*pud));
    printk(KERN_ALERT "PMD: %lx\n", pmd_val(*pmd));
    printk(KERN_ALERT "PTE: %lx\n", pte_val(*pte));

    // get page
    page = pte_page(*pte);
    phys_addr = ((unsigned long)page_to_pfn(page) << PAGE_SHIFT) + (kernel_addr & ~PAGE_MASK); // Get physical address with offset
    pte_unmap(pte);

out:
    if(copy_to_user(pa, &phys_addr, sizeof(phys_addr)))
        return -EFAULT;

    return 0;
}
