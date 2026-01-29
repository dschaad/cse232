/***********************************************************************
 * Header:
 *    Test
 * Summary:
 *    Driver to test node.h
 * Author:
 *    James Helfrich, PhD. (c) 2022 by Kendall Hunt
 ************************************************************************/

#ifndef DEBUG
#define DEBUG   
#endif
//#undef DEBUG  // Remove this comment to disable unit tests

#include "testSpy.h"        // for the spy unit tests
#include "testNode.h"       // for the unit tests
int Spy::counters[] = {};

/**********************************************************************
 * MAIN
 * This is just a simple menu to launch a collection of tests
 ***********************************************************************/
int main()
{
#ifdef DEBUG
   // unit tests
   TestSpy().run();
   TestNode().run();
#endif // DEBUG
  
   return 0;
}

