#include <iostream>
#include <fstream>
#include <string>

using namespace std;

void loadFiles();
void initPageTable();
void allocatePage(int logical_address);
void writeToFile();

const int PAGE_SIZE = 4;
const int LOGICAL_MEMORY_SIZE = 16;                                 // 4 pages of 4 bytes each
const int PHYSICAL_MEMORY_SIZE = 32;                                // 8 pages of 4 bytes each
const int PAGE_TABLE_SIZE = LOGICAL_MEMORY_SIZE;                    // Sets the size of the page table to the number of pages in logical memory.
int pageTable[PAGE_TABLE_SIZE];                                     // Sets the size of the page table to the number of pages in logical memory.
string logicalMemory[LOGICAL_MEMORY_SIZE][PAGE_SIZE];               // Creates an array of 4 pages of 4 bytes each.
string physicalMemory[PHYSICAL_MEMORY_SIZE / PAGE_SIZE][PAGE_SIZE]; // Divides the memory size by the page size to create an array for phyiscal memory.

// Load file containing strings.
void loadFiles()
{
    ifstream infile("input.txt");
    string line;
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

// Initialize page table using the constants set above.
void initPageTable()
{
    for (int i = 0; i < PAGE_TABLE_SIZE; i++)
    {
        pageTable[i] = -1;
    }
}

int freeFrames()
{
    for (int i = 0; i < PHYSICAL_MEMORY_SIZE / PAGE_SIZE; i++)
    {
        bool is_free = true;
        for (int j = 0; j < PAGE_SIZE; j++)
        {
            if (physicalMemory[i][j].empty())
            {
                is_free = false;
                break;
            }
        }
        if (is_free)
        {
            return i;
        }
    }
    return -1; // Return -1 if no free frames.
}

void allocatePage(int logical_address)
{
    int page_index = logical_address / PAGE_SIZE;
    int offset = logical_address % PAGE_SIZE;
    int frame_index = pageTable[page_index];
    if (frame_index == -1)
    {
        frame_index = freeFrames();
        if (frame_index == -1)
        {
            cout << "Error: Physical memory full!" << endl;
            return;
        }
        pageTable[page_index] = frame_index;
        for (int i = 0; i < PAGE_SIZE; i++)
        {
            physicalMemory[frame_index][i] = logicalMemory[page_index][i];
        }
        cout << "Page fault occurred, loaded page " << page_index << " into frame " << frame_index << endl;
    }
    cout << "String: " << logicalMemory[page_index][offset] << " allocated to frame " << frame_index << endl;
}

void writeToFile()
{
    ofstream outfile("output.txt");
    for (int i = 0; i < PHYSICAL_MEMORY_SIZE / PAGE_SIZE; i++)
    {
        for (int j = 0; j < PAGE_SIZE; j++)
        {
            if (!physicalMemory[i][j].empty())
            {
                int logical_address = i * PAGE_SIZE + j;
                int frame_index = pageTable[logical_address / PAGE_SIZE];
                outfile << "String: " << physicalMemory[i][j] << " in frame " << frame_index << " allocated using ";
                if (frame_index == logical_address / PAGE_SIZE)
                {
                    outfile << "optimal paging." << endl;
                }
                else
                {
                    outfile << "FIFO paging." << endl;
                }
            }
        }
    }
    outfile.close();
}

int main()
{
    loadFiles();
    initPageTable();
    allocatePage(2);
    allocatePage(7);
    allocatePage(12);
    allocatePage(1);
    writeToFile();
    return 0;
}