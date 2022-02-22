#include <iostream>  // Used to overload ostream for struct KeyValue
#include <map>
#include <vector>

namespace storage {
namespace sstable {

struct KeyValue {
  std::string key;
  std::string value;

  // rvalue
  KeyValue(std::string&& k, std::string&& v) : key(k), value(v) {}
  // lvalue
  KeyValue(std::string& k, std::string& v)
      : key(std::move(k)), value(std::move(v)) {}
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

// TODO: Pretty sure this shouldn't be static.
static std::ostream& operator<<(std::ostream& os, const KeyValue& kv) {
  os << kv.key << ":" << kv.value;
  return os;
}

}  // namespace sstable
}  // namespace storage
