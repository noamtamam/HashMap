#include <sstream>
#include <iostream>
#include <cmath>
#include "HashMap.hpp"
#include "Dictionary.hpp"
#define FUNC(name) std::make_pair(#name, name)
#define IS_TRUE(x) IS_TRUE_MSG(x, "")
#define IS_TRUE_MSG(x, msg) if (!(x)) { std::cout << __FUNCTION__ << " failed on line " << __LINE__ << ". Message: " << msg << std::endl; return false; }
#define RAISES_ERROR(error, function, ...) try { function ( __VA_ARGS__ ); return false; } catch (const error &Error) { std::cout << "Got Expected error: " << Error.what() << std::endl; }
#define EPSILON 1e-7
#define COMP_FLOAT(x, y, epsilon) IS_TRUE_MSG(std::fabs (x - y) < epsilon, x << " !~= " << y << " (" << epsilon << " precision)")
#define SUCCESS "\033[1;32mSUCCESS\033[0m"
#define FAILURE "\033[1;31mFAILURE\033[0m"
#define MAGNETA_S "\033[1;35m"
#define MAGNETA_E "\033[0m"
#define BLUE_START "\033[1;34m"
#define BLUE_END "\033[0m"

typedef std::vector<std::pair<std::string, std::string>> s_pair_vec;

bool test_default_cntr() {
  HashMap<int, int> map;
  IS_TRUE(map.empty())
  IS_TRUE(map.size() == 0)
  IS_TRUE(map.capacity() == 16)
  IS_TRUE(!map.contains_key (100))
  return true;
}

bool test_copy_cntr() {
  HashMap<int, std::string> map1;
  map1.insert (1, "hi");
  map1.insert (2, "bye");
  HashMap<int, std::string> map2 (map1);
  map2[2] = "ny";
  IS_TRUE(map2.at (1) == "hi" && map1.at (1) == "hi")
  IS_TRUE_MSG(map2.at (2) == "ny" && map1.at (2) == "bye", map2.at (2))
  return true;
}

bool test_special_cntr() {
  std::vector<int> keys {1  ,2  ,3  ,1  ,4  ,4  };
  std::vector<std::string> vals {"a","b","c","d","e","f"};
  HashMap<int, std::string> map1 (keys, vals);
  IS_TRUE_MSG(map1.size() == 4, "Size is: " << map1.size())
  // Implementation specific?
  IS_TRUE_MSG(map1.capacity() == 16, "Cap is: " << map1.capacity())
  IS_TRUE(map1[1] == "d")
  IS_TRUE(map1[2] == "b")
  IS_TRUE(map1[3] == "c")
  IS_TRUE(map1[4] == "f")
  HashMap<int, std::string> map2;
  map2[1] = "hi";
  IS_TRUE(map2.size() == 1)
  IS_TRUE(map2[1] == "hi")

  map2 = map1;
  map2[3] = "d";
  IS_TRUE(map1[1] == "d")
  IS_TRUE(map1[3] == "c")
  IS_TRUE(map2[3] == "d")

//  keys.pop_back();
//  typedef HashMap<int, std::string> tempt;
//  RAISES_ERROR(std::length_error, tempt, keys, vals)
  return true;
}

bool test_size_and_resize() {
  HashMap<int, int> map;
  for (int i = 1; i <= 100; i ++) {
    i % 2 == 0 ? map[i] = i : map.insert (i, i);
    IS_TRUE(map.size() == i)
  }
  IS_TRUE_MSG(map.capacity() == 256, "Got capacity of " << map.capacity())
  COMP_FLOAT(map.get_load_factor(), 100.0 / 256.0, EPSILON)
  for (int i = 1; i <= 100; i ++) {
    map.erase (i);
    IS_TRUE_MSG(map.size() == 100 - i, "Got " << map.size() << " Instead of " << 100 - i)
  }
  IS_TRUE_MSG(map.capacity() == 1, "Got capacity of " << map.capacity())
  return true;
}

bool test_basic_insert_erase() {
  HashMap<int, int> map;
  IS_TRUE(map.empty())
  IS_TRUE(map.insert (1, 10))
  IS_TRUE(map.contains_key (1))
  IS_TRUE(!map.insert (1, 11))
  IS_TRUE(map.at (1) == 10)
  IS_TRUE(map.erase (1))
  IS_TRUE(!map.contains_key (1))

  RAISES_ERROR(std::exception, map.at, 1)
  return true;
}

bool test_clear() {
  HashMap<int, int> map;
  IS_TRUE(map.empty())
  IS_TRUE(map.insert (1, 10))
  IS_TRUE(map.insert (2, 10))
  IS_TRUE(map.capacity() == 16)
  map.clear();
  IS_TRUE(map.empty())
  IS_TRUE(map.capacity() == 16)
  return true;
}

bool test_bucket_ops() {
  HashMap<int, int> map;
  IS_TRUE(map.capacity() == 16)
  IS_TRUE(map.insert (10, 10))

  IS_TRUE(map.capacity() == 16)
  IS_TRUE_MSG(map.bucket_index(10) == 10, map.bucket_index (10))
  IS_TRUE(map.insert (17, 17))
  IS_TRUE(map.bucket_index(17) == 1)

  IS_TRUE(map.bucket_size(17) == 1)
  IS_TRUE(map.bucket_size(10) == 1)
  IS_TRUE(map.size() == 2)

  RAISES_ERROR(std::exception, map.bucket_size, 1)
  RAISES_ERROR(std::exception, map.bucket_index, 2)
  return true;
}

bool test_iterator() {
  std::vector<int> keys {1  ,2  ,3  ,4  };
  std::vector<std::string> vals {"a","b","c","d"};
  HashMap<int, std::string> map (keys, vals);
  IS_TRUE(map.begin() != map.end())

  int count = 0, sum = 0;
  for (const auto &it : map) {
    count += 1;
    sum += it.first;
  }
  IS_TRUE(sum == 10 && count == 4)

  auto it1 = map.begin();
  auto it2 = ++map.begin();
  IS_TRUE(it1 != it2)
  it1 ++;
  IS_TRUE(it1 == it2)

  it1 = map.begin();
  IS_TRUE((++it1) == it1)
  IS_TRUE((it1++) != it1)
  IS_TRUE_MSG(it1->second == (++(++map.begin()))->second, it1->second)

  count = sum = 0;
  for (auto it = map.cbegin(); it != map.cend(); it ++) {
    count += 1;
    sum += it->first;
  }
  IS_TRUE(sum == 10 && count == 4)

  HashMap<int, int> emtpy_map;
  IS_TRUE(emtpy_map.begin() == emtpy_map.end())
  return true;
}

bool test_compare () {
  HashMap<int, int> m1;
  HashMap<int, int> m2;

  IS_TRUE(m1 == m2 && m2 == m1)
  m1.insert (1, 1);
  IS_TRUE(m1 == m1)
  IS_TRUE(!(m1 == m2) && !(m2 == m1) && (m1 != m2) && (m2 != m1))

  m2.insert (1, 2);
  IS_TRUE(!(m1 == m2) && !(m2 == m1) && (m1 != m2) && (m2 != m1))
  m2[1] = 1;
  IS_TRUE(m1 == m2 && m2 == m1)
  m2[100] = 19;
  IS_TRUE(!(m1 == m2) && !(m2 == m1) && (m1 != m2) && (m2 != m1))

  m1.clear(); m2.clear();
  IS_TRUE(m1 == m2 && m2 == m1)
  for (int i = 0; i < 100; i ++) {
    m1.insert (i, i);
    m2.insert (100 - i, i);
  }
  IS_TRUE(!(m1 == m2) && !(m2 == m1) && (m1 != m2) && (m2 != m1))
  return true;
}

bool test_dict () {
  Dictionary d;
  s_pair_vec vals;
  vals.emplace (vals.end(), "Hey", "There");
  vals.emplace (vals.end(), "Whats", "up");
  vals.emplace (vals.end(), "Hey", "bye");
  d.update (vals.begin(), vals.end());
  IS_TRUE(d.size() == 2)
  IS_TRUE(d["Hey"] == "bye")
  IS_TRUE(d["Whats"] == "up")
  IS_TRUE(d.erase("Hey"))
  RAISES_ERROR(std::invalid_argument, d.erase, "Hey")
  IS_TRUE(d.size() == 1)
  d.update (vals.end(), vals.end());
  IS_TRUE(d.size() == 1)
  return true;
}

typedef bool (*testFunc) ();

int main () {
  std::vector<std::pair<std::string, testFunc>> _tests {
      FUNC(test_default_cntr),
      FUNC(test_copy_cntr),
      FUNC(test_special_cntr),
      FUNC(test_size_and_resize),
      FUNC(test_bucket_ops),
      FUNC(test_basic_insert_erase),
      FUNC(test_iterator),
      FUNC(test_clear),
      FUNC(test_compare),
      FUNC(test_dict),
  };
  int passed = 0;
  int failed = 0;
  for (std::pair<std::string, testFunc> &p : _tests) {
      std::cout << "\t- " << BLUE_START << p.first << BLUE_END << "\n";
      bool res = p.second();
      passed += (res ? 1 : 0);
      failed += (res ? 0 : 1);
      std::cout << BLUE_START << "\t\tTest summary - " << BLUE_END << (res ? SUCCESS : FAILURE) << std::endl ;
    }
  std::cout << MAGNETA_S << "\nSummary: " << (failed == 0 ? SUCCESS : FAILURE
  ) << MAGNETA_S << "\nPassed: " << passed << "\nFailed: " << failed << MAGNETA_E << std::endl ;
  return 0;
}
