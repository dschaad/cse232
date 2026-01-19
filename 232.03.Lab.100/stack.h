/***********************************************************************
 * Module:
 *    Stack
 * Summary:
 *    Our custom implementation of std::stack
 *
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *
 *    This will contain the class definition of:
 *       stack             : similar to std::stack
 * Author
 *    David Schaad, Tori Tremelling
 ************************************************************************/

#pragma once

#include <cassert>  // because I am paranoid
//#include "vector.h"
#include <vector>

class TestStack; // forward declaration for unit tests

namespace custom
{

/**************************************************
 * STACK
 * First-in-Last-out data structure
 *************************************************/
template<class T>
class stack
{
   friend class ::TestStack; // give unit tests access to the privates
public:
  
   // 
   // Construct
   // 

   stack()                            { container.empty(); }
   stack(const stack <T> &  rhs)      { }
   stack(stack <T>&& rhs)             { }
   stack(const std::vector<T> &  rhs) { }
   stack(      std::vector<T> && rhs) { }
   ~stack()                           { container.empty(); }

   //
   // Assign
   //

   stack <T> & operator = (const stack <T> & rhs)
   {
      return *this;
   }
   stack <T>& operator = (stack <T> && rhs)
   {
      return *this;
   }
   void swap(stack <T>& rhs)
   {
      std::swap(container, rhs.container);
   }

   // 
   // Access
   //

         T& top()       { return container[size() - 1]; }
   const T& top() const { return container[size() - 1]; }

   // 
   // Insert
   // 

   void push(const T&  t) {  }
   void push(      T&& t) {  }

   //
   // Remove
   //

   void pop() 
   { 
      if (!empty())
         --container.numElements;
   }

   //
   // Status
   //
   size_t  size () const { return container.size();  }
   bool empty   () const { return container.size() == 0; }
   
private:
   
  std::vector<T> container;  // underlying container
};



} // custom namespace


