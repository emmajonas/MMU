# Virtual Memory Unit (MMU)

Implemented a Virtual Memory Unit in C, which translates a set of logical addresses to physical addresses for a virtual address space size of 65,536 bytes.

The program reads from a file containing logical addresses, uses a TLB and page table to translate each logical address to its corresponding physical address, then outputs the value of the byte stored at each physical address. Finally, the page fault rate and TLB hits rate are output at the end of the file.

It supports either 256 or 128 frames (each frame is 256 bytes) of physical memory. If there are 128 frames available, then page replacement is required. The pages are replaced using the Least Recently Used (LRU) algorithm.
