/* PriorityList.h
 * Extended from List.h 
 * 
 * Priority list keeps the paradigms of List but adds the
 * functionality that the list is sorted according to a particular
 * function that is determined when the list is created. Lowest
 * elements are stored on the left and are then in increasing order.
 */

#ifndef _PriorityList_h_
#define _PriorityList_h_

#include "List.h"

template <class T>
class PriorityList : public List<T> {
public:
  PriorityList(float (*f)(T)) : List<T>() {
    comp = f;
  }

  void push_front(const T & new_element) { push(new_element); }
  void push_back(const T & new_element)  { push(new_element); }
  void push(const T& ele);

protected:
  float (*comp)(T);
};

template <class T>
void PriorityList<T>::push(const T& new_element) {

    auto itr = this->cbegin();
    while ((itr != this->cend()) && (comp(*itr) < comp(new_element))) {
      itr++;
    }
    
    insert(itr, new_element);
}

#endif
