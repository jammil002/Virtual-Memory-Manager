#include <iostream>
#include <fstream>
#include <string>
#include <limits>

// Functions
void loadFiles();
void initPageTable();
std::string allocatePage(int logical_address);
void writeToOutputFile(std::string output);

// Global variables and constants
const int PAGE_SIZE = 4;
const int LOGICAL_MEMORY_SIZE = 16;
const int PHYSICAL_MEMORY_SIZE = 32;
const int PAGE_TABLE_SIZE = LOGICAL_MEMORY_SIZE;
int pageTable[PAGE_TABLE_SIZE];
std::string logicalMemory[LOGICAL_MEMORY_SIZE][PAGE_SIZE];
std::string physicalMemory[PHYSICAL_MEMORY_SIZE / PAGE_SIZE][PAGE_SIZE];
unsigned long accessCounter = 0;
unsigned long lastAccess[PHYSICAL_MEMORY_SIZE / PAGE_SIZE];

void initPageTable()
{
    for (int i = 0; i < PAGE_TABLE_SIZE; i++)
    {
        pageTable[i] = -1;
    }
}

void loadFiles()
{
    std::ifstream infile("input.txt");
    std::string line;
    int page_index = 0;
    int offset = 0;
    while (getline(infile, line))
    {
        if (offset == PAGE_SIZE)
        {
            offset = 0;
            page_index++;
        }
        logicalMemory[page_index][offset] = line;
        offset++;
    }
}

void writeToOutputFile(std::string text)
{
    std::ofstream output;

    output.open("./output.txt", std::ios::out | std::ios::app);
    if (output.fail())
        throw std::ios_base::failure(std::strerror(errno));

    output.exceptions(output.exceptions() | std::ios::failbit | std::ifstream::badbit);

    output << text << std::endl;
    output.close();
}

int freeFrames()
{
    for (int i = 0; i < PHYSICAL_MEMORY_SIZE / PAGE_SIZE; i++)
    {
        bool isMemoryFree = true;
        for (int j = 0; j < PAGE_SIZE; j++)
        {
            if (physicalMemory[i][j].empty())
            {
                isMemoryFree = false;
                break;
            }
        }
        if (isMemoryFree)
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
    unsigned long minValue = std::numeric_limits<unsigned long>::max();

    for (int i = 0; i < PHYSICAL_MEMORY_SIZE / PAGE_SIZE; i++)
    {
        if (lastAccess[i] < minValue)
        {
            minValue = lastAccess[i];
            minIndex = i;
        }
    }
    return minIndex;
}

std::string allocatePage(int logical_address)
{
    int page_index = logical_address / PAGE_SIZE;
    int offset = logical_address % PAGE_SIZE;
    int frame_index = pageTable[page_index];

    std::string output = ""; // Create an empty string to store the output

    if (frame_index == -1)
    {
        frame_index = freeFrames();

        if (frame_index == -1)
        {
            frame_index = findLeastRecentlyUsedFrame();

            for (int i = 0; i < PAGE_TABLE_SIZE; i++)
            {
                if (pageTable[i] == frame_index)
                {
                    pageTable[i] = -1;
                    break;
                }
            }
        }

        pageTable[page_index] = frame_index;
        for (int i = 0; i < PAGE_SIZE; i++)
        {
            physicalMemory[frame_index][i] = logicalMemory[page_index][i];
        }

        std::string page_fault_msg = "Page fault occurred, loaded page " + std::to_string(page_index) + " into frame " + std::to_string(frame_index);
        std::cout << page_fault_msg << std::endl;
        output += page_fault_msg + "\n"; // Append the message to the output string
    }

    accessCounter++;
    lastAccess[frame_index] = accessCounter;

    std::string allocation_msg = "String: " + logicalMemory[page_index][offset] + " allocated to frame " + std::to_string(frame_index);
    std::cout << allocation_msg << std::endl;
    output += allocation_msg + "\n"; // Append the message to the output string

    return output;
}

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
