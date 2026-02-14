#ifndef GW_LIST_H
#define GW_LIST_H

#include <iostream>
#include <stdexcept>

template <typename T> class List;
template <typename T> class ListNode {
private:
  T data;
  ListNode<T> *next;
  ListNode(T data) : data(data), next(nullptr) {}

  friend class List<T>;

  template <typename U>
  friend std::ostream &operator<<(std::ostream &os, const List<U> &list);
};

template <typename T> class List {
private:
  ListNode<T> *head;
  ListNode<T> *tail;
  int listSize;

public:
  List();
  List(T data);
  ~List();

  List(const List<T> &other);
  List(List<T> &&other) noexcept;
  List<T> &operator=(const List<T> &other);
  List<T> &operator=(List<T> &&other) noexcept;

  void append(T data);
  void pop();

  void insert(int index, T data);

  T get(int index) const;
  void set(int index, T data);

  void print() const;

  int size() const;

  template <typename U>
  friend std::ostream &operator<<(std::ostream &os, const List<U> &list);
};

template <typename T>
List<T>::List() : head(nullptr), tail(nullptr), listSize(0) {}

template <typename T>
List<T>::List(T data) : head(nullptr), tail(nullptr), listSize(0) {
  append(data);
}

template <typename T> List<T>::~List() {
  ListNode<T> *current = head;
  while (current != nullptr) {
    ListNode<T> *next = current->next;
    delete current;
    current = next;
  }
}

template <typename T>
List<T>::List(const List<T> &other)
    : head(nullptr), tail(nullptr), listSize(0) {
  ListNode<T> *current = other.head;
  while (current != nullptr) {
    append(current->data);
    current = current->next;
  }
}

template <typename T>
List<T>::List(List<T> &&other) noexcept
    : head(other.head), tail(other.tail), listSize(other.listSize) {
  other.head = nullptr;
  other.tail = nullptr;
  other.listSize = 0;
}

template <typename T> List<T> &List<T>::operator=(const List<T> &other) {
  if (this != &other) {
    ListNode<T> *current = head;
    while (current != nullptr) {
      ListNode<T> *next = current->next;
      delete current;
      current = next;
    }

    head = nullptr;
    tail = nullptr;
    listSize = 0;

    current = other.head;
    while (current != nullptr) {
      append(current->data);
      current = current->next;
    }
  }
  return *this;
}

template <typename T> List<T> &List<T>::operator=(List<T> &&other) noexcept {
  if (this != &other) {
    ListNode<T> *current = head;
    while (current != nullptr) {
      ListNode<T> *next = current->next;
      delete current;
      current = next;
    }

    head = other.head;
    tail = other.tail;
    listSize = other.listSize;

    other.head = nullptr;
    other.tail = nullptr;
    other.listSize = 0;
  }
  return *this;
}

template <typename T> void List<T>::append(T data) {
  ListNode<T> *newNode = new ListNode<T>(data);

  if (head == nullptr) {
    head = newNode;
    tail = newNode;
  } else {
    tail->next = newNode;
    tail = newNode;
  }

  listSize++;
}

template <typename T> void List<T>::pop() {
  if (head == nullptr) {
    return;
  }

  if (head == tail) {
    delete head;
    head = nullptr;
    tail = nullptr;
    listSize = 0;
    return;
  }

  ListNode<T> *current = head;
  while (current->next != tail) {
    current = current->next;
  }

  delete tail;
  tail = current;
  tail->next = nullptr;
  listSize--;
}

template <typename T> void List<T>::insert(int index, T data) {
  if (index < 0 || index > listSize) {
    throw std::out_of_range("Index out of range");
  }

  if (index == 0) {
    ListNode<T> *newNode = new ListNode<T>(data);
    newNode->next = head;
    head = newNode;

    if (tail == nullptr) {
      tail = newNode;
    }

    listSize++;
    return;
  }

  if (index == listSize) {
    append(data);
    return;
  }

  ListNode<T> *current = head;
  for (int i = 0; i < index - 1; i++) {
    current = current->next;
  }

  ListNode<T> *newNode = new ListNode<T>(data);
  newNode->next = current->next;
  current->next = newNode;
  listSize++;
}

template <typename T> T List<T>::get(int index) const {
  if (index < 0 || index >= listSize) {
    throw std::out_of_range("Index out of range");
  }

  ListNode<T> *current = head;
  for (int i = 0; i < index; i++) {
    current = current->next;
  }

  return current->data;
}

template <typename T> void List<T>::set(int index, T data) {
  if (index < 0 || index >= listSize) {
    throw std::out_of_range("Index out of range");
  }

  ListNode<T> *current = head;
  for (int i = 0; i < index; i++) {
    current = current->next;
  }

  current->data = data;
}

template <typename T> void List<T>::print() const {
  ListNode<T> *current = head;
  while (current != nullptr) {
    std::cout << current->data << " ";
    current = current->next;
  }
  std::cout << std::endl;
}

template <typename T> int List<T>::size() const { return listSize; }

template <typename T>
std::ostream &operator<<(std::ostream &os, const List<T> &list) {
  ListNode<T> *current = list.head;
  while (current != nullptr) {
    os << current->data;
    if (current->next != nullptr) {
      os << " ";
    }
    current = current->next;
  }
  return os;
}

#endif // GW_LIST_H