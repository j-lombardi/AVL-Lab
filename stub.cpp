/*
Author: Julian Lombardi
Class: CSI-281-04
Assignment: PA7
Date Assigned: 11/17/17
Due Date: 12/1/17 11:00am
Description:
    Creates a self balancing tree that handles insertion and deletion while retaining AVL properties
Certification of Authenticity:
    I certify that this is entirely my own work, except where I have given
    fully-documented references to the work of others. I understand the
    definition and consequences of plagiarism and acknowledge that the assessor
    of this assignment may, for the purpose of assessing this assignment:
        - Reproduce this assignment and provide a copy to another member of
        academic staff; and/or
        - Communicate a copy of this assignment to a plagiarism checking
        service (which may then retain a copy of this assignment on its
        database for the purpose of future plagiarism checking)
*/

#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include "avl.h"
#include "queue.h"
#include "timerSystem.h"

using namespace std;

const int MAX_NUM = 100000, MIN_NUM = 0;

int main()
{
    
    cout.setf(ios::fixed | ios::showpoint);
    cout.precision(9);

    AVL<int> tree;
    bool     found;
    TimerSystem timer;

    srand(1);

    timer.startClock();
    for (int i = 0; i < 100; i++) {
        int randNum = rand() % (MAX_NUM - MIN_NUM + 1) + MIN_NUM;
        tree.insert(randNum);
    }

    cout << timer.getTime() << " seconds.\n";
    
    timer.startClock();
    tree.dfs(rand() % (MAX_NUM - MIN_NUM + 1) + MIN_NUM);
    cout << timer.getTime() << " seconds.\n";


    timer.startClock();
    tree.dfs(rand() % (MAX_NUM - MIN_NUM + 1) + MIN_NUM);
    cout << timer.getTime() << " seconds.\n";

    timer.startClock();
    tree.dfs(rand() % (MAX_NUM - MIN_NUM + 1) + MIN_NUM);
    cout << timer.getTime() << " seconds.\n";
    

    

    return 0;
}