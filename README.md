# Linux_Project1
Linux OS 課程的第一個project

leo_project1.c 撰寫 kernel 的system call,讓 Project1 的 Question1,2 去呼叫kernel 的 system call 服務
先用 copy_from_user() 把 user process 的 virtual address 傳進 kernel,在 kernel 中用將接收的 virtual address 透過 linux 的五層 page_table 轉成 physical address, 再用 copy_to_user() 傳回 user address space.

project1.c 的 code 是 Project1 的 Question 1
要觀察 fork() 後所造成的 Copy-on-write 機制，父和子 process 一開始會共享相同的實體記憶體位置，但若子 process 修改(寫入)某一個變數，kernel 就會為他的這個變數配置一個新的實體記憶體位置，再讓子 process 寫入。

project1-2.c 的 code 是 Project1 的 Question 2 
因應現在的程式都很大， 所以 OS 採取虛擬記憶體機制，執行 process 不需要一開始把整個程式的 data load進實體記憶體，當變數的大小超過 user address space 所能配置的實體記憶體，就不會配置給超出的部分實體記憶體，除非之後去對他做存取。

