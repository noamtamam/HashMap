#ifndef _HASHMAP_HPP_
#define _HASHMAP_HPP_
#define INITIAL_SIZE 16
#define UPPER_FACTOR 0.75
#define LOWER_FACTOR 0.25
#define ERROR_AT_MSG "key was not found"
#define INVALID_VEC_ERROR "vectors and not the same size"
#define INCREASE_BASE 2
#define DECREASE_BASE 0.5
#define MIN_CAPACITY 1
#include <vector>
#include <string>
#include <stdexcept>

template <typename KeyT, typename  ValueT>
class HashMap
{
  /**** Types ***/
  typedef std::pair<KeyT, ValueT> cell;
  typedef std::vector<std::pair<KeyT, ValueT>> vec;

 private:
  vec* _data = nullptr;
  unsigned int _capacity;
  unsigned int _size;


/***
 * *** const iterator class***
 */
  class ConstIterator
  {
    const HashMap* _hash_map;
    unsigned int _outer_idx;
    unsigned int _inner_idx;
   public:

    typedef cell value_type;
    typedef const cell &reference;
    typedef const cell *pointer;
    typedef int difference_type; // irrelevant here, as we have no difference
    // - but still required
    typedef std::forward_iterator_tag iterator_category;

    ConstIterator(const HashMap* hash_map, unsigned int out_idx,
                  unsigned int inner_idx):
    _hash_map
    (hash_map),_outer_idx(out_idx), _inner_idx(inner_idx) {}


    ConstIterator& operator++()
    {
      if(_inner_idx  +1< (_hash_map->_data[_outer_idx]).size())
      {
        _inner_idx++;
      }
      else
      {
        _outer_idx++;
        unsigned int cur_outer_idx = _outer_idx;
        while (_outer_idx < _hash_map->_capacity && (_hash_map->_data
                                                    +_outer_idx)->empty())
        {
          _outer_idx++;
        }
        _inner_idx = 0;
        if(_outer_idx == _hash_map->_capacity)
        {
          _outer_idx = cur_outer_idx-1;
          _inner_idx = _hash_map->_data[_outer_idx].size();
        }

      }
      return *this;
    }

    ConstIterator operator++(int)
    {
      ConstIterator it(*this);
      this->operator++();
      return it;
    }

    bool operator==(const ConstIterator &rhs) const
    {
      return  ((_hash_map == rhs._hash_map)
               && (_inner_idx == rhs._inner_idx)
               && (_outer_idx == rhs._outer_idx));
    }


    bool operator!=(ConstIterator rhs) const
    {
      return !operator== (rhs);
    }

    reference operator*() const
    {
      return _hash_map->_data[_outer_idx][_inner_idx];
    }

    pointer operator-> () const
    {
      return &(operator*());
    }

  };

 public:
  using const_iterator = ConstIterator;
    /***
     * returns an iterator of the first item in hashmap
     * @return const oterator
     */
    const_iterator cbegin() const
    {
      if(this->empty())
      {
        return ConstIterator(this, 0,0);
      }
      unsigned int i = 0;
      while(_data[i].empty() && i<_capacity)
      {
        i++;
      }
      return ConstIterator(this, i,0);
    }

    const_iterator begin()const
    {
      return cbegin();
    }
    /***
     * returns the last item in hash map
     * @return const iterator
     */
    const_iterator cend() const
    {
      if(empty())
      {
        return ConstIterator(this, 0,0);
      }
      unsigned int i = _capacity-1;
      while (_data[i].empty() && i >=0)
      {
        i--;
      }
      unsigned int cell_idx = _data[i].size();
      return ConstIterator(this, i, cell_idx);
    }
    const_iterator end() const
    {
      return cend();
    }


/***
 ***** function hash-map***
 */
  HashMap()
  {
    _data = new std::vector<std::pair<KeyT, ValueT>>[INITIAL_SIZE];
    _size = 0;
    _capacity = INITIAL_SIZE;
  }
  /***
   * gets 2 vectors and inserts the vectors values by key and value from
   * each vector
   * @param vec1
   * @param vec2
   */
  HashMap(std::vector<KeyT> vec1, std::vector<ValueT> vec2)
  {
    _data = new std::vector<std::pair<KeyT, ValueT>>[INITIAL_SIZE];
    _size = 0;
    _capacity = INITIAL_SIZE;
    if (vec1.size () != vec2.size ())
    {
      delete[] _data;
      throw std::runtime_error (INVALID_VEC_ERROR);
    }
    for (unsigned int i = 0; i < vec2.size (); i++)
    {
      operator[] (vec1[i]) = vec2[i];
    }
  }

  /**
   * copy constructor for non const hashmap
   * @param other
   */
  HashMap(HashMap& other)
  {
    _data = new std::vector<std::pair<KeyT, ValueT>>[INITIAL_SIZE];
    _size = 0;
    _capacity = INITIAL_SIZE;
    for(cell cur_cell:other)
    {
      this->insert (cur_cell.first, cur_cell.second);
    }
  }

  /***
   * copy constructor for const hashmap
   * @param other
   */
  HashMap(const HashMap& other)
  {
    _data = new std::vector<std::pair<KeyT, ValueT>>[INITIAL_SIZE];
    _size = 0;
    _capacity = INITIAL_SIZE;
    for(cell cur_cell:other)
    {
      this->insert (cur_cell.first, cur_cell.second);
    }
  }

/***
 * return the number of items in the hash map
 * @return int
 */
  unsigned int size() const
  {
    return _size;
  }
/***
 * return the number of items that can be instore in hashmap
 * @return int
 */
  unsigned int capacity() const
  {
    return _capacity;
  }

  bool empty() const
  {
    return _size == 0;
  }

/***
 * contains if a kkey is inside a hashmap
 * @param key
 * @return true if inside else false
 */
  bool contains_key(KeyT key) const
  {
    unsigned int idx = get_hash_idx (key);
    for(auto& cell:_data[idx])
    {
      if(cell.first == key)
      {
        return true;
      }
    }
    return false;
  }
/*****
 * returns the value of the key if exisit in hashmap
 * else throw exception
 * @param key
 * @return the value by reference open for edits
 */
  ValueT& at(KeyT key)
  {
    if(!contains_key (key))
    {
      throw std::runtime_error(ERROR_AT_MSG);
    }
    unsigned int idx = get_hash_idx (key);

    for( auto& cell:_data[idx])
    {
      if(cell.first == key)
      {
        return cell.second;
      }
    }
    return _data[idx][0].second;
  }
/****
 * return the value in the hashmap paired to the key
 * without premmision to edit
 * @param key
 * @return value by value
 */
  ValueT at(KeyT key) const
  {
    if(!contains_key (key))
    {
      throw std::runtime_error(ERROR_AT_MSG);
    }
    unsigned int idx = get_hash_idx (key);
    for(const auto cell:_data[idx])
    {
      if(cell.first == key)
      {
        return cell.second;
      }
    }
    return ValueT();
  }

/***
 * gets aa key and erase it from hash map
 * @param key
 * @return true if exists and deleted false if wasnt exist
 */
  virtual bool erase(KeyT key)
  {
    unsigned int idx = get_hash_idx (key);
    for(auto it = _data[idx].begin(); it!=_data[idx].end(); ++it)
    {
      if(it->first == key)
      {
        _data[idx].erase(it);
        _size--;
        while((get_load_factor()<LOWER_FACTOR) & (_capacity != MIN_CAPACITY))
        {
          double_decrease_size();
        }
        return true;
      }
    }
    return false;
  }
  /***
   * gets the factor load of the hash map the ratio of size and capacity
   * @return
   */
  double get_load_factor()const
  {
    return ((double) _size) / ((double) _capacity);
  }
/***
 * gets a key and value amd insert if to the hash map
 * @param key
 * @param value
 * @return true if wast exisit else false
 */
  bool insert(KeyT key, ValueT value)
  {
    // check if is already inside
    if(this->contains_key (key))
    {
        return false;
    }
    // insert the pair
    std::pair<KeyT, ValueT> cur_pair(key, value);
    unsigned int indx = get_hash_idx (key);
    _data[indx].push_back(cur_pair);
    _size ++;
    // resize the hash map if needed
    if (get_load_factor() > UPPER_FACTOR)
    {
      double_size();
    }
    return true;
  }
  /**
   * gets a key and return his value in hash map if was exisit,
   * if not  it creates and return the reference to the value
   * with the option to edit it
   * @param key
   * @return value by reference
   */
  ValueT& operator[](KeyT key)
  {
    if(contains_key (key))
    {
      return at(key);
    }
    this->insert (key, ValueT());
    return at (key);

  }
  /***
   * gets a key and return his value in hash map if was exisit,
   * if not it creates and return it
   * without the option to edit it
   * @param key
   * @return
   */
  ValueT operator[](KeyT key)const
  {
    if(contains_key (key))
    {
      return at(key);
    }
    return ValueT();
  }
  /***
   * gets 2 hash map amd checks if they are indendical by items
   * @param other
   * @return true if equal else false
   */
  bool operator==(HashMap<KeyT,ValueT>& other) const
  {
    if (_size != other._size)
    {
      return false;
    }

    for (const cell &cur_cell: (*this))
    {
      if (!(this->contains_key (cur_cell.first)))
      {
        return false;
      }
      if (this->at (cur_cell.first) != cur_cell.second)
      {
        return false;
      }
    }

    for (const cell& cur_cell: (other))
    {
      if (!(this->contains_key (cur_cell.first)))
      {
        return false;
      }
      if (this->at (cur_cell.first) != cur_cell.second)
      {
        return false;
      }
    }
      return true;
    }
  /***
   * gets 2 hash map amd checks if they are not indendical by items
   * @param other
   * @return fasle if equal else false
   */
    bool operator!=(HashMap<KeyT,ValueT>& other) const
    {
      return !operator== (other);
    }
    unsigned int bucket_size(KeyT key) const
    {
      if(!contains_key (key))
      {
        throw std::runtime_error(ERROR_AT_MSG);
      }
      unsigned int idx = get_hash_idx (key);
      return _data[idx].size();
    }
/***
 * gets a key and return which index of bck is it in the hashmap
 * throw exception in doesnt exisit
 * @param key
 * @return the index
 */
  unsigned int bucket_index(KeyT key)
    {
      if(!contains_key (key))
      {
        throw std::runtime_error(ERROR_AT_MSG);
      }
      return get_hash_idx (key);
    }
    /***
     * deletes all items in hashmap
     */
    HashMap& clear()
    {
      for(unsigned int i=0; i<_capacity; i++)
      {
        _data[i].clear();
      }
      _size = 0;
      return *this;
    }

    HashMap<KeyT, ValueT>& operator=(HashMap<KeyT, ValueT>& other)
    {
      if(*this == other)
      {
        return *this;
      }
      clear();
      if(_capacity != other._capacity)
      {
        delete _data;
        _data = new std::vector<std::pair<KeyT, ValueT>>;
        _capacity = other._capacity;
      }
//      _size = other._size;
      for(cell cur_cell:other)
      {
        this->insert (cur_cell.first, cur_cell.second);
      }
      return *this;
    }
    virtual ~HashMap()
    {
    clear();
    delete[] _data;
    }
 private:
  /****
 * gets the index of a key in hash map by his key
 * @param key
 * @return int
 */
  unsigned int get_hash_idx(KeyT key) const
  {
    unsigned int hash_value = std::hash<KeyT> {} (key);
    return hash_value &(_capacity -1);
  }

  /***
 * resize and rehash the hashmap double the size of capacity
 */
  void double_size()
  {
    // make new vector
    unsigned int new_capacity = _capacity*INCREASE_BASE;
    vec* temp = new std::vector<std::pair<KeyT, ValueT>>[new_capacity];
    for(const cell& cur_cell:(*this))
    {
      unsigned int indx = std::hash<KeyT> {} (cur_cell.first) &(
          (new_capacity)-1);
      temp[indx].push_back(cur_cell);
    }
    //delete the old data
    delete [] _data;
    _data = temp;
    _capacity =new_capacity;
  }

  /**
   * resize and rehash the hash map- divided the size by 2
   */
  void double_decrease_size()
  {

    // make new vector
    unsigned int new_capacity = _capacity*DECREASE_BASE;
    vec* temp = new std::vector<std::pair<KeyT,
        ValueT>>[new_capacity];
    for(const cell& cur_cell:(*this))
    {
      unsigned int indx = std::hash<KeyT> {} (cur_cell.first) &(
          (new_capacity)-1);
      temp[indx].push_back(cur_cell);
    }
    //delete the old one
    delete [] _data;
    _data = temp;
    _capacity = new_capacity;
  }

};
#endif //_HASHMAP_HPP_

