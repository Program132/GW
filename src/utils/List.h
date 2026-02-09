#ifndef GW_LIST_H
#define GW_LIST_H

#include <iostream>
#include <stdexcept>

template <typename T> class List {
private:
  T data;
  List<T> *next = nullptr;
  bool hasFirstValue = false;

public:
  List() : data(T{}), next(nullptr) {}
  List(T data);
  ~List();

  void append(T data);
  void pop();

  void insert(int index, T data);

  T get(int index);
  void set(int index, T data);

  void print();

  int size();

  template <typename U>
  friend std::ostream &operator<<(std::ostream &os, const List<U> &list);
};

template <typename T>
List<T>::List(T data) : data(data), next(nullptr), hasFirstValue(true) {}

template <typename T> List<T>::~List() { delete next; }

template <typename T> void List<T>::append(T data) {
  if (!hasFirstValue) {
    this->data = data;
    hasFirstValue = true;
  } else if (next) {
    next->append(data);
  } else {
    next = new List<T>(data);
  }
}

template <typename T> void List<T>::pop() {
  if (next) {
    if (next->next == nullptr) {
      delete next;
      next = nullptr;
    } else {
      next->pop();
    }
  }
}

template <typename T> void List<T>::insert(int index, T data) {
  if (index == 0) {
    List<T> *temp = next;
    next = new List<T>(this->data);
    next->next = temp;
    this->data = data;
  } else if (next) {
    next->insert(index - 1, data);
  } else {
    throw std::out_of_range("Index out of range");
  }
}

template <typename T> T List<T>::get(int index) {
  if (index == 0) {
    return data;
  } else if (next) {
    return next->get(index - 1);
  }
  throw std::out_of_range("Index out of range");
}

template <typename T> void List<T>::set(int index, T data) {
  if (index == 0) {
    this->data = data;
  } else if (next) {
    next->set(index - 1, data);
  }
}

template <typename T> void List<T>::print() {
  std::cout << data << " ";
  if (next) {
    next->print();
  } else {
    std::cout << std::endl;
  }
}

template <typename T> int List<T>::size() {
  if (next) {
    return 1 + next->size();
  }
  return 1;
}

template <typename T>
std::ostream &operator<<(std::ostream &os, const List<T> &list) {
  os << list.data;
  if (list.next) {
    os << " " << *(list.next);
  }
  return os;
}

#endif // GW_LIST_H