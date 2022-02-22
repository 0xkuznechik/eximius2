#include <iostream>
#include <string>

#include "utils/sort/qsort.h"

using utils::sort::MakeQuickSort;
using utils::sort::QuickSort;

struct KeyValue {
  std::string key;
  std::string value;

  // rvalue
  KeyValue(std::string&& k, std::string&& v) : key(k), value(v) {}
  // lvalue
  KeyValue(std::string& k, std::string& v)
      : key(std::move(k)), value(std::move(k)) {}
  // copy constructor
  KeyValue(const KeyValue& kv) : key(kv.key), value(kv.value) {}
  // move constructor
  KeyValue(KeyValue&& kv)
      : key(std::move(kv.key)), value(std::move(kv.value)) {}

  // Copy assignment operator
  KeyValue& operator=(const KeyValue& kv) {
    key = kv.key;
    value = kv.value;
    return *this;
  }
  // Move assignment operator
  KeyValue& operator=(KeyValue&& kv) {
    key = std::move(kv.key);
    value = std::move(kv.value);
    return *this;
  }

  bool operator<(const KeyValue& other) {
    if (key == other.key) {
      return value < other.value;
    }
    return key < other.key;
  }

  bool operator==(const KeyValue& other) {
    return key == other.key && value == other.value;
  }
};

std::ostream& operator<<(std::ostream& os, const KeyValue& kv) {
  os << kv.key << ":" << kv.value;
  return os;
}

int main(int argc, char** argv) {
  std::vector<int> v = {5, 3, 4, 7, 1, 8};
  auto q = MakeQuickSort(v);  // factory function, with type deduction
  q.Print();
  q.Sort();
  q.Print();

  std::vector<std::string> s = {"albert", "chang", "is", "cool"};
  auto p = MakeQuickSort(s);
  p.Print();
  p.Sort();
  p.Print();

  KeyValue kv1{"x", "3"};
  KeyValue kv2{"a", "1"};
  KeyValue kv3{"b", "2"};
  KeyValue kv4{"x", "4"};

  std::vector<KeyValue> keyvalues = {kv1, kv2, kv3, kv4};
  auto u = MakeQuickSort(keyvalues);
  u.Print();
  u.Sort();
  u.Print();

  return 0;
}
