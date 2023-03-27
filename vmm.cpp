#include <iostream>
#include <fstream>
#include <string>
#include <limits>

// Functions
void loadFiles();
void initPageTable();
int findLeastRecentlyUsedFrame();
std::string allocatePage(int logicalAddress);
void writeToOutputFile(std::string output);

// Global variables and constants
const int PAGE_SIZE = 4;                                                 // The page size is 4
const int LOGICAL_MEMORY_SIZE = 16;                                      // The logical memory size is 16
const int PHYSICAL_MEMORY_SIZE = 32;                                     // The physical memory size is 32
const int PAGE_TABLE_SIZE = LOGICAL_MEMORY_SIZE;                         // The page table size is the same as the logical memory size
int pageTable[PAGE_TABLE_SIZE];                                          // The page table is an array of integers
std::string logicalMemory[LOGICAL_MEMORY_SIZE][PAGE_SIZE];               // The logical memory is an array of strings
std::string physicalMemory[PHYSICAL_MEMORY_SIZE / PAGE_SIZE][PAGE_SIZE]; // The physical memory is an array of strings
unsigned long accessCounter = 0;                                         // This variable is used to keep track of the last access time of the frames
unsigned long lastAccess[PHYSICAL_MEMORY_SIZE / PAGE_SIZE];              // This array is used to store the last access time of the frames

// The page table is initialized by this function by setting all entries to 1.
void initPageTable()
{
    for (int i = 0; i < PAGE_TABLE_SIZE; i++)
    {
        pageTable[i] = -1;
    }
}

// This function reads the input file input.txt and loads the data into the logical memory.
void loadFiles()
{
    std::ifstream infile("input.txt");
    std::string line;
    int pageIndex = 0;
    int offset = 0;
    while (getline(infile, line))
    {
        if (offset == PAGE_SIZE)
        {
            offset = 0;
            pageIndex++;
        }
        logicalMemory[pageIndex][offset] = line; // Store the line in the logical memory
        offset++;
    }
}

// The given text is appended by this function to the output file output.txt.
void writeToOutputFile(std::string text)
{
    std::ofstream output;

    output.open("output.txt", std::ios::out | std::ios::app);
    if (output.fail())
        throw std::ios_base::failure(std::strerror(errno));

    output.exceptions(output.exceptions() | std::ios::failbit | std::ifstream::badbit);

    output << text << std::endl;
    output.close();
}

// This function searches the physical memory for any available free frames and then returns the index of the first frame that is available. It returns -1 if there are no free frames.
int freeFrames()
{
    for (int i = 0; i < PHYSICAL_MEMORY_SIZE / PAGE_SIZE; i++)
    {
        bool isMemoryFree = true;
        for (int j = 0; j < PAGE_SIZE; j++)
        {
            if (physicalMemory[i][j].empty())
            {
                isMemoryFree = false; // If the frame is not empty, then it is not free
                break;
            }
        }
        if (isMemoryFree) // If the frame is free, then return the index of the frame
        {
            return i;
        }
    }
    return -1;
}

// Add a function to find the least recently used frame
int findLeastRecentlyUsedFrame()
{
    int minIndex = 0;
    unsigned long minValue = std::numeric_limits<unsigned long>::max(); // Set the minimum value to the maximum value of an unsigned long

    for (int i = 0; i < PHYSICAL_MEMORY_SIZE / PAGE_SIZE; i++)
    {
        if (lastAccess[i] < minValue) // If the last access time of the frame is less than the minimum value, then update the minimum value and the index of the frame
        {
            minValue = lastAccess[i];
            minIndex = i;
        }
    }
    return minIndex;
}

// This function allocates a page to the physical memory and returns the string that is allocated to the frame.
std::string allocatePage(int logicalAddress)
{
    int pageIndex = logicalAddress / PAGE_SIZE;
    int offset = logicalAddress % PAGE_SIZE;
    int frameIndex = pageTable[pageIndex];

    std::string output = ""; // Create an empty string to store the output

    if (frameIndex == -1)
    {
        frameIndex = freeFrames();

        if (frameIndex == -1) // If there are no free frames, then find the least recently used frame
        {
            frameIndex = findLeastRecentlyUsedFrame(); // Find the least recently used frame

            for (int i = 0; i < PAGE_TABLE_SIZE; i++) // Remove the page from the page table
            {
                if (pageTable[i] == frameIndex) // If the page is found in the page table, then remove it
                {
                    pageTable[i] = -1;
                    break;
                }
            }
        }

        pageTable[pageIndex] = frameIndex;
        for (int i = 0; i < PAGE_SIZE; i++)
        {
            physicalMemory[frameIndex][i] = logicalMemory[pageIndex][i]; // Copy the page from the logical memory to the physical memory
        }

        std::string page_fault_msg = "Page fault occurred, loaded page " + std::to_string(pageIndex) + " into frame " + std::to_string(frameIndex);
        std::cout << page_fault_msg << std::endl;
        output += page_fault_msg + "\n"; // Append the message to the output string
    }

    accessCounter++;                        // Increment the access counter
    lastAccess[frameIndex] = accessCounter; // Update the last access time of the frame

    std::string outputMessage = "String: " + logicalMemory[pageIndex][offset] + " allocated to frame " + std::to_string(frameIndex);
    std::cout << outputMessage << std::endl;
    output += outputMessage + "\n"; // Append the message to the output string

    return output;
}

// Main function
int main()
{
    loadFiles();
    initPageTable();
    std::string output = "";
    output += allocatePage(2);
    output += allocatePage(7);
    output += allocatePage(12);
    output += allocatePage(1);
    writeToOutputFile(output);
    return 0;
}
