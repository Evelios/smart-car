// List.h
//
// A class List similar to the STL container list.
// Version 1.2
//
// Included from Versions 1.0 and 1.1:
//   List(), back(), push_back(e), pop_back()
//   iterators, constant iterators
//   begin, end, cbegin, cend
//   insert, erase
//
// New in this version:
//   copy constructor, assignment operator, destructor

#ifndef _List_h_
#define _List_h_

template <class T>
class List;

template <class T>
class ListIterator;

template <class T>
class ConstListIterator;

template <class T>
class ListNode
// T is the type of element stored in the List.
{
    friend class List<T>;
    friend class ListIterator<T>;
    friend class ConstListIterator<T>;

private:
    ListNode() : element(), next(nullptr), previous(nullptr) {}
    ListNode(const T & e, ListNode * next0, ListNode * previous0);

    T element;
    ListNode<T> * next;
    ListNode<T> * previous;
};

template <class T>
ListNode<T>::ListNode(const T & e, ListNode * next0,
                   ListNode * previous0) :
        element(e), next(next0), previous(previous0) {}

template <class T>
class ListIterator
// T is the type of element stored in the List.
{
   friend class List<T>;
   friend class ConstListIterator<T>;

public:
    ListIterator() : p_current_node(nullptr) {}

    T & operator*() const { return p_current_node->element; }
    T * operator->() const { return &(p_current_node->element); }

    bool operator==(const ListIterator & rhs) const
    {
        return (p_current_node == rhs.p_current_node);
    }
    bool operator!=(const ListIterator & rhs) const
    {
        return (p_current_node != rhs.p_current_node);
    }

    ListIterator & operator++();  // prefix version (++itr)
    ListIterator & operator--();

    ListIterator operator++(int);  // postfix version (itr++)
    ListIterator operator--(int);

private:
    explicit ListIterator(ListNode<T> * p) : p_current_node(p) {}

    ListNode<T> * p_current_node;
        // points to the node that contains the element that the
        // iterator currently points to
};

template <class T>
inline ListIterator<T> & ListIterator<T>::operator++()
// prefix version (++itr)
{
    p_current_node = p_current_node->next;
    return *this;
}

template <class T>
inline ListIterator<T> & ListIterator<T>::operator--()
// prefix version (--itr)
{
    p_current_node = p_current_node->previous;
    return *this;
}

template <class T>
inline ListIterator<T> ListIterator<T>::operator++(int)
// postfix version (itr++)
{
    ListIterator<T> original_itr = *this;
    p_current_node = p_current_node->next;
    return original_itr;
}

template <class T>
inline ListIterator<T> ListIterator<T>::operator--(int)
// postfix version (itr++)
{
    ListIterator<T> original_itr = *this;
    p_current_node = p_current_node->previous;
    return original_itr;
}

template <class T>
class ConstListIterator
// T is the type of element stored in the List.
{
   friend class List<T>;

public:
    ConstListIterator() : p_current_node(nullptr) {}

    ConstListIterator(const ListIterator<T> & itr) :
        p_current_node(itr.p_current_node) {}

    const T & operator*() const { return p_current_node->element; }
    const T * operator->() const
    {
        return &(p_current_node->element);
    }

    bool operator==(const ConstListIterator & rhs) const
    {
        return (p_current_node == rhs.p_current_node);
    }
    bool operator!=(const ConstListIterator & rhs) const
    {
        return (p_current_node != rhs.p_current_node);
    }

    ConstListIterator & operator++();  // prefix version (++itr)
    ConstListIterator & operator--();

    ConstListIterator operator++(int);  // postfix version (itr++)
    ConstListIterator operator--(int);

private:
    explicit ConstListIterator(const ListNode<T> * p) :
        p_current_node(p) {}

    const ListNode<T> * p_current_node;
        // points to the node that contains the element that the
        // iterator currently points to
};

template <class T>
inline ConstListIterator<T> & ConstListIterator<T>::operator++()
// prefix version (++itr)
{
    p_current_node = p_current_node->next;
    return *this;
}

template <class T>
inline ConstListIterator<T> & ConstListIterator<T>::operator--()
// prefix version (--itr)
{
    p_current_node = p_current_node->previous;
    return *this;
}

template <class T>
inline ConstListIterator<T> ConstListIterator<T>::operator++(int)
// postfix version (itr++)
{
    ConstListIterator<T> original_itr = *this;
    p_current_node = p_current_node->next;
    return original_itr;
}

template <class T>
inline ConstListIterator<T> ConstListIterator<T>::operator--(int)
// postfix version (itr++)
{
    ConstListIterator<T> original_itr = *this;
    p_current_node = p_current_node->previous;
    return original_itr;
}

template <class T>
class List
// T is the type of element stored in the List.
{
public:
    typedef ListIterator<T> iterator;
    typedef ConstListIterator<T> const_iterator;

    List();
    List(const List<T> & ls);
    ~List();

    T & back() { return p_head_node->previous->element; }
    const T & back() const
    {
        return p_head_node->previous->element;
    }

    T & front() { return p_head_node->next->element; }
    const T & front() const {
      return p_head_node->next->element;
    }

    void push_front(const T & new_element);
    void pop_front();
    void push_back(const T & new_element);
    void pop_back();

    iterator begin() { return iterator(p_head_node->next); }
    iterator end() { return iterator(p_head_node); }

    const_iterator begin() const
    {
        return const_iterator(p_head_node->next);
    }
    const_iterator end() const
    {
        return const_iterator(p_head_node);
    }

    const_iterator cbegin() const
    {
        return const_iterator(p_head_node->next);
    }
    const_iterator cend() const
    {
        return const_iterator(p_head_node);
    }

    iterator rbegin() { return iterator(p_head_node); }
    iterator rend() { return iterator(p_head_node->previous); }

    const_iterator rbegin() const
    {
        return const_iterator(p_head_node);
    }
    const_iterator rend() const
    {
        return const_iterator(p_head_node->previous);
    }

    const_iterator crbegin() const
    {
        return const_iterator(p_head_node);
    }
    const_iterator crend() const
    {
        return const_iterator(p_head_node->previous);
    }

    iterator insert(const_iterator itr, const T & new_element);
    iterator erase(const_iterator itr );
    iterator erase(const T & element);
    bool contains(const T & element);
    void clear();

    int size() const;

    List<T> & operator=( const List<T> & rhs );

protected:
    ListNode<T> * p_head_node;
};

template <class T>
inline List<T>::List()
{
    p_head_node = new ListNode<T>;
    p_head_node->next = p_head_node->previous = p_head_node;
}

template <class T>
inline void List<T>::push_front(const T & new_element)
{
    // set a pointer to the first node
    ListNode<T> * p_first_node = p_head_node->next;

    // create new node and set its contents
    ListNode<T> * p_new_node =
        new ListNode<T>(new_element, p_first_node, p_head_node);

    // finish linking new node to list
    p_head_node->next = p_new_node;
    p_first_node->previous = p_new_node;
}

template <class T>
inline void List<T>::pop_front()
{
    // set pointers to first node and node that will become first
    ListNode<T> * p_first_node = p_head_node->next;
    ListNode<T> * p_new_first_node = p_first_node->next;

    // modify the list to skip the last node
    p_head_node->next = p_new_first_node;
    p_new_first_node->previous = p_head_node;

    // deallocate the last node
    p_first_node->next = p_first_node->previous = nullptr;
    delete p_first_node;
}

template <class T>
inline void List<T>::push_back(const T & new_element)
{
    // set a pointer to the last node
    ListNode<T> * p_last_node = p_head_node->previous;

    // create new node and set its contents
    ListNode<T> * p_new_node =
        new ListNode<T>(new_element, p_head_node, p_last_node);

    // finish linking new node to list
    p_last_node->next = p_new_node;
    p_head_node->previous = p_new_node;
}

template <class T>
inline void List<T>::pop_back()
{
    // set pointers to last node and node that will become last
    ListNode<T> * p_last_node = p_head_node->previous;
    ListNode<T> * p_new_last_node = p_last_node->previous;

    // modify the list to skip the last node
    p_new_last_node->next = p_head_node;
    p_head_node->previous = p_new_last_node;

    // deallocate the last node
    p_last_node->next = p_last_node->previous = nullptr;
    delete p_last_node;
}

template <class T>
inline typename List<T>::iterator List<T>::insert(
        const_iterator const_itr, const T & new_element)
{
    // set pointer to the node that should follow the new one
    ListNode<T> * p_next_node =
        (ListNode<T> *)(const_itr.p_current_node);

    // set pointer to the node that should precede the new one
    ListNode<T> * p_previous_node = p_next_node->previous;

    // create new node and set its contents
    ListNode<T> * p_new_node =
        new ListNode<T>(new_element, p_next_node, p_previous_node);

    // set next and previous nodes to point to the new node
    p_previous_node->next = p_new_node;
    p_next_node->previous = p_new_node;

    return iterator(p_new_node);
}

template <class T>
inline typename List<T>::iterator List<T>::erase(
        const_iterator const_itr)
{
    // set pointer to the node to be deleted
    ListNode<T> * p_target_node =
        (ListNode<T> *)(const_itr.p_current_node);

    // set pointers to the nodes that precede and follow the target
    // node
    ListNode<T> * p_previous_node = p_target_node->previous;
    ListNode<T> * p_next_node = p_target_node->next;

    // modify the list to skip the target node
    p_previous_node->next = p_next_node;
    p_next_node->previous = p_previous_node;

    // deallocate the target node
    p_target_node->next = p_target_node->previous = nullptr;
    delete p_target_node;

    return iterator(p_next_node);
}

template <class T>
inline typename List<T>::iterator List<T>::erase(const T& element)
{
    auto itr = cbegin();
    while (itr != cend()) {
      if (*itr == element) {
        return erase(itr);
      }
      itr++;
    }
    return end();
}

template <class T>
bool List<T>::contains(const T & ele) {
  auto itr = begin();
  while (itr != end()) {
    if ((*itr) == ele) {
      return true;
    }
    itr++;
  }
  return false;
}

template <class T>
void List<T>::clear() {
  while (begin() != end()) pop_back();
}

template <class T>
int List<T>::size() const {
  int len = 0;
  
  auto itr = cbegin();
  while(itr != cend()) {
    itr++;
    len++;  
  }
  return len;
}

template <class T>
List<T>::List(const List<T> & ls) : List<T>()
{
    for (const T & e : ls) push_back(e);
}

template <class T>
List<T>::~List()
{
    // erase all elements
    clear();

    // deallocate dummy head node
    delete p_head_node;
}

template <class T>
inline List<T> & List<T>::operator=(const List<T> & rhs)
{
    List<T> copy_of_rhs = rhs;
    auto temp = p_head_node;
    p_head_node = copy_of_rhs.p_head_node;
    copy_of_rhs.p_head_node = temp;
    return *this;
    // old contents of receiver is deallocated when copy_of_rhs
    // is destroyed
}

#endif
