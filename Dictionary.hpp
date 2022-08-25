
#include "HashMap.hpp"
#ifndef _DICTIONARY_HPP_
#define _DICTIONARY_HPP_

 class InvalidKey: public std::invalid_argument
{
  public:
   InvalidKey(std::string msg= ERROR_AT_MSG): std::invalid_argument (msg){};
};


class Dictionary :public HashMap<std::string, std::string>
{
 public:
  Dictionary() =default;
  Dictionary(std::vector<std::string> vec1,
             std::vector<std::string> vec2) :
             HashMap<std::string, std::string>(vec1, vec2) {}

 Dictionary(HashMap<std::string, std::string> hm) : HashMap<std::string,
 std::string>(hm){}
/*****
 * gets a key in dictionary, and delete it from the dictionary
 * @param key
 * @return true if the key is inside and was deleted
 */
  bool erase(std::string key) override
  {
    if(HashMap::contains_key (key))
    {
      return HashMap::erase (key);
    }
    throw InvalidKey();
  }
  /***
   * get a forward iterator for the begin and end insert all objects inside
   * @tparam ForwardIt
   * @param begin
   * @param end
   */
  template<class ForwardIt>
  void update (ForwardIt begin, ForwardIt end)
  {
    for(auto it = begin; it!=end; it++)
    {
      HashMap::operator[] (it->first) = it->second;
    }
  }
};



#endif //_DICTIONARY_HPP_
