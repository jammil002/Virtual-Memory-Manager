### Virtual Memory Manager
<hr />

#### Run File
##### To run this file download the executable from the release menu ->
##### Navigate to the executable on your machine via the command line and type ./vmm
#### Description

This C++ program executes memory allocation for virtual memory addresses in a logical memory using a page table while simulating a paging model.

PAGE_SIZE, LOGICAL_MEMORY_SIZE, and PHYSICAL_MEMORY_SIZE are three constants that are first defined by the program. LOGICAL_MEMORY_SIZE is the total size of the logical memory in bytes, PHYSICAL_MEMORY_SIZE is the total size of the physical memory in bytes, and PAGE_SIZE is the size of each page in bytes. Later on in the program, these values are used to initialize arrays and data structures.

The strings read from the input file are stored in a two-dimensional array called logicalMemory by the program. Each column in the array corresponds to a byte within a page, and each row to a page. Strings are read from the files using the loadFiles() function and stores them in the logicalMemory array.

#### Screenshots
<img width="543" alt="Screenshot 2023-03-26 at 2 57 12 PM" src="https://user-images.githubusercontent.com/68131997/227807469-508ab4ac-ef8a-4428-961b-854290a57316.png">
