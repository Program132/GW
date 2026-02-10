#include "TestsList.h"
#include "../src/utils/Assert.h"
#include "../src/utils/List.h"

void testIntList() {
  List<int> list = List<int>(5);
  GW_ASSERT(list.get(0) == 5);

  list = List<int>(0);
  list.append(1);

  GW_ASSERT(list.get(0) == 0);

  List<int> list2 = List<int>(1);
  list2.append(2);
  list2.append(3);
  GW_ASSERT(list2.get(1) == 2);
  GW_ASSERT(list2.get(list2.size() - 1) == 3);

  list.append(1);
  list.append(1);
  list.append(1);
  list.append(1);
  list.append(1);
  list.append(1);
  list.append(1);
  GW_ASSERT(list.get(list.size() - 1) == 1);

  list2.append(1554564544);
  GW_ASSERT(list2.get(list2.size() - 1) == 1554564544);

  list.pop();
  GW_ASSERT(list.get(list.size() - 1) == 1);

  list.insert(0, 10);
  GW_ASSERT(list.get(0) == 10);

  list.insert(5, 10);
  GW_ASSERT(list.get(5) == 10);

  list.set(0, 20);
  GW_ASSERT(list.get(0) == 20);

  list.set(5, 20);
  GW_ASSERT(list.get(5) == 20);
}

void testStringList() {
  List<std::string> list = List<std::string>("Hello World");
  GW_ASSERT(list.get(0) == "Hello World");
  list.append("Hi");
  GW_ASSERT(list.get(list.size() - 1) == "Hi");

  list.append("a");
  GW_ASSERT(list.get(list.size() - 1) == "a");
  GW_ASSERT(list.get(2) == "a");
  list.append("b");
  GW_ASSERT(list.get(list.size() - 1) == "b");

  GW_ASSERT(list.size() == 4);

  list.insert(3, "Bye bye");
  GW_ASSERT(list.get(3) == "Bye bye");
  GW_ASSERT(list.size() == 5);
  GW_ASSERT(list.get(list.size() - 1) == "b");

  list.pop();
  GW_ASSERT(list.size() == 4);
  GW_ASSERT(list.get(list.size() - 1) == "Bye bye");
}
