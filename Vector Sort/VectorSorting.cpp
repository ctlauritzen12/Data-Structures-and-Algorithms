//============================================================================
// Name        : VectorSorting.cpp
// Author      : Chris Lauritzen
// Version     : 1.0
// Description : Vector Sorting Algorithms
//============================================================================

#include <algorithm>
#include <iostream>
#include <time.h>

#include "CSVparser.hpp"

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

// forward declarations
double strToDouble(string str, char ch);

// define a structure to hold bid information
struct Bid {
    string bidId; // unique identifier
    string title;
    string fund;
    double amount;
    Bid() {
        amount = 0.0;
    }
};

//============================================================================
// Static methods used for testing
//============================================================================

/**
 * Display the bid information to the console (std::out)
 *
 * @param bid struct containing the bid info
 */
void displayBid(Bid bid) {
    cout << bid.bidId << ": " << bid.title << " | " << bid.amount << " | "
            << bid.fund << endl;
    return;
}

/**
 * Prompt user for bid information using console (std::in)
 *
 * @return Bid struct containing the bid info
 */
Bid getBid() {
    Bid bid;

    cout << "Enter Id: ";
    cin.ignore();
    getline(cin, bid.bidId);

    cout << "Enter title: ";
    getline(cin, bid.title);

    cout << "Enter fund: ";
    cin >> bid.fund;

    cout << "Enter amount: ";
    cin.ignore();
    string strAmount;
    getline(cin, strAmount);
    bid.amount = strToDouble(strAmount, '$');

    return bid;
}

vector<Bid> loadBids(string csvPath) {
    cout << "Loading CSV file " << csvPath << endl;

    // Define a vector data structure to hold a collection of bids.
    vector<Bid> bids;

    // initialize the CSV Parser using the given path
    csv::Parser file = csv::Parser(csvPath);

    try {
        // loop to read rows of a CSV file
        for (int i = 0; i < file.rowCount(); i++) {

            // Create a data structure and add to the collection of bids
            Bid bid;
            bid.bidId = file[i][1];
            bid.title = file[i][0];
            bid.fund = file[i][8];
            bid.amount = strToDouble(file[i][4], '$');

            //cout << "Item: " << bid.title << ", Fund: " << bid.fund << ", Amount: " << bid.amount << endl;

            // push this bid to the end
            bids.push_back(bid);
        }
    } catch (csv::Error &e) {
        std::cerr << e.what() << std::endl;
    }
    return bids;
}

int partition(vector<Bid>& bids, int begin, int end) {
    
    int low = begin;
    int high = end; //set low and high equal to begin and end
    int pivot = low + ((high - low) / 2); // pick the middle element as pivot point
    bool done = false;

    while (!done) {// while not done 
        // keep incrementing low index while bids[low] < bids[pivot]
        while (bids[low].title < bids[pivot].title) {
            low += 1;
        }
        // keep decrementing high index while bids[pivot] < bids[high]
        while (bids[pivot].title < bids[high].title) {
            high -= 1;
        }
        /* If there are zero or one elements remaining,
            all bids are partitioned. Return high */
        if (low >= high) {
            done = true;
        }
        else {
            // else swap the low and high bids (built in vector method)   
            swap(bids[low], bids[high]);
            // move low and high closer ++low, --high
            low += 1;
            high -= 1;
        }
    }
    return high;
}

void quickSort(vector<Bid>& bids, int begin, int end) {
    
    int mid = 0;//set mid equal to 0
    int low = begin;
    int high = end;

    /* Base case: If there are 1 or zero bids to sort,
     partition is already sorted otherwise if begin is greater
     than or equal to end then return*/

    if (low >= high) {
        return;
    }

    /* Partition bids into low and high such that
     midpoint is location of last element in low */

    mid = partition(bids, low, high);

    // recursively sort low partition (begin to mid)

    quickSort(bids, low, mid);

    // recursively sort high partition (mid+1 to end)

    quickSort(bids, mid + 1, high);

}

// FIXME (1a): Implement the selection sort logic over bid.title

/**
 * Perform a selection sort on bid title
 * Average performance: O(n^2))
 * Worst case performance O(n^2))
 *
 * @param bid address of the vector<Bid>
 *            instance to be sorted
 */
void selectionSort(vector<Bid>& bids) {
    //define min as int (index of the current minimum bid)

    // check size of bids vector
    // set size_t platform-neutral result equal to bid.size()

    // pos is the position within bids that divides sorted/unsorted
    // for size_t pos = 0 and less than size -1 
        // set min = pos
        // loop over remaining elements to the right of position
            // if this element's title is less than minimum title
                // this element becomes the minimum
        // swap the current minimum with smaller one found
            // swap is a built in vector method
    int j = 0;
    int min = 0; //(index of the current minimum bid)
    string temp; //holds value for swapping

    int size_t = bids.size(); //variable for the size of the bids vector

    int pos = 0; //index position within bids that divides sorted/unsorted
    for (pos = 0; pos < size_t - 1; ++pos) {// for size_t pos = 0 and less than size -1 
        min = pos;// set min = pos
        for (j = pos + 1; j < size_t - 1; ++j) {// loop over remaining elements to the right of position
            if (bids[j].title < bids[min].title) {// if this element's title is less than minimum title
                min = j; // this element becomes the minimum
            }
            temp = bids[pos].title;
            bids[pos].title = bids[min].title;
            bids[min].title = temp;
            // swap the current minimum with smaller one found
        }
    }
}

/**
 * Simple C function to convert a string to a double
 * after stripping out unwanted char
 *
 * credit: http://stackoverflow.com/a/24875936
 *
 * @param ch The character to strip out
 */
double strToDouble(string str, char ch) {
    str.erase(remove(str.begin(), str.end(), ch), str.end());
    return atof(str.c_str());
}

/**
 * The one and only main() method
 */
int main(int argc, char* argv[]) {

    // process command line arguments
    string csvPath;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        break;
    default:
        csvPath = "eBid_Monthly_Sales.csv";
    }

    // Define a vector to hold all the bids
    vector<Bid> bids;

    // Define a timer variable
    clock_t ticks;

    int choice = 0;
    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Load Bids" << endl;
        cout << "  2. Display All Bids" << endl;
        cout << "  3. Selection Sort All Bids" << endl;
        cout << "  4. Quick Sort All Bids" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {

        case 1:
            // Initialize a timer variable before loading bids
            ticks = clock();

            // Complete the method call to load the bids
            bids = loadBids(csvPath);

            cout << bids.size() << " bids read" << endl;

            // Calculate elapsed time and display result
            ticks = clock() - ticks; // current clock ticks minus starting clock ticks
            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;

            break;

        case 2:
            // Loop and display the bids read
            for (int i = 0; i < bids.size(); ++i) {
                displayBid(bids[i]);
            }
            cout << endl;

            break;

        // FIXME (1b): Invoke the selection sort and report timing results

        // FIXME (2b): Invoke the quick sort and report timing results

        }
    }

    cout << "Good bye." << endl;

    return 0;
}
