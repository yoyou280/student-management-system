#ifndef _TRIE_HPP
#define _TRIE_HPP

#include <functional>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>

/**
 * The trie.
 *
 * This implementation stores keys of type std::string and values of type V.
 * Each edge corresponds to one character in the range [BeginChar, EndChar], so
 * each node has a fixed branching factor equal to `(EndChar - BeginChar + 1)`.
 *
 * @tparam V The value type stored in the trie.
 * @tparam BeginChar The inclusive lower bound of supported key characters.
 * @tparam EndChar The inclusive upper bound of supported key characters.
 */
template <typename V, char BeginChar, char EndChar> class Trie final {
private:
  static_assert(BeginChar <= EndChar, "BeginChar must be <= EndChar");
  static const int BranchingFactor = EndChar - BeginChar + 1;

  friend struct Test;

  /**
   * The node in the trie.
   */
  struct Node {
    /**
     *  The key string corresponding to @ref value.
     */
    std::string key;
    /**
     *  The value corresponding to @ref key.
     */
    V value;
    /**
     *  The children of the node in the trie.
     */
    Trie<V, BeginChar, EndChar> children[BranchingFactor];

    Node(std::string key, V value): key{key} , value{value} {}

  };

  /** The root node pointer; nullptr means that the trie is empty. */
  Node *root;

  /**
   * Converts a character into a child index.
   *
   * throws std::out_of_range if the character is outside [BeginChar, EndChar].
   */
  int idx_of(char c) const {
    if (c < BeginChar || c > EndChar) {
      std::stringstream ss;
      ss << "character '" << c << "' is outside trie range [" << BeginChar
         << ", " << EndChar << "]";
      throw std::out_of_range(ss.str());
    }
    return c - BeginChar;
  }

  bool has_children(Node* node) const {
    if (node == nullptr) return false;
    for (int i = 0; i < BranchingFactor; i++){
      if (node->children[i].root != nullptr){
        return true;
      }
    }
    return false;
  }

  Node* copy_node(const Node* other_node) {
    if (other_node == nullptr){return nullptr;}

    Node* new_node = new Node(other_node->key, other_node->value);
    for (int i = 0; i < BranchingFactor; i++){
      new_node->children[i].root = copy_node(other_node->children[i].root);
    }
    return new_node;
  }

public:
  /**
   * Constructs an empty trie.
   */
  Trie() {
    root = nullptr;

  }

  /**
   * Constructs a deep copy of the trie.
   */
  Trie(const Trie &other) {
    if(other.root != nullptr){
      root = copy_node(other.root);
    }
    else{
      root = nullptr;
    }

  }

  /**
   * Assigns by deep-copying another trie.
   */
  Trie &operator=(const Trie &other) {
    if(this == &other){
      return *this;
    }

    delete root;
    if(other.root != nullptr){
      root = copy_node(other.root);
    }
    else{
      root = nullptr;
    }

    return *this;
  }

  /**
   * Destructs the trie and all its nodes.
   */
  ~Trie() {
    delete root;
  }

  /**
   * Searches for a key-value pair by key.
   *
   * @param key The key to locate.
   * @return A pointer to the value if exists, otherwise nullptr.
   * @throws std::out_of_range for out-of-range key characters.
   */
  V *search(const std::string &key) const {
    for(char c : key){
      idx_of(c); 
    }

    if(root == nullptr){return nullptr;}
    if(root->key == key){
      return &(root->value);
    }

    const Trie* checker = this;

    for(int i = 0; i < static_cast<int>(key.length()); i++){
      char next_check = key[i];
      int index = idx_of(next_check);
      checker = &(checker->root->children[index]);
      if(checker->root == nullptr){return nullptr;}
      if(checker->root->key == key){
        return &(checker->root->value);
      }
    }

    return nullptr;


  }

  /**
   * Inserts or overwrites a key-value pair by the key.
   *
   * @param key The key to insert or overwrite.
   * @param val The value to insert or overwrite.
   * @throws std::out_of_range for out-of-range key characters.
   */
  void insert(const std::string &key, const V &val) {

    V* check_exist = search(key);
    if(check_exist == nullptr){
      if(root == nullptr){
        root = new Node(key, val);
        return;
      }
      Trie* checker = this;

      for(int i = 0; i < static_cast<int>(key.length()); i++){
        char next_check = key[i];
        int index = idx_of(next_check);
        checker = &(checker->root->children[index]);
        if(checker->root == nullptr){
          checker->root = new Node(key, val);
          return;
        }
      }
      Node* temp = checker->root;
      checker->root = new Node(key, val);
      for(int i = 0; i < BranchingFactor; i++){
        Node* move_trie = temp->children[i].root;
        temp->children[i].root = nullptr;
        checker->root->children[i].root = move_trie;
      }
      insert(temp->key, temp->value);

      delete temp;
    }
    else{
      *check_exist = val;
    }


  }

  /**
   * Removes a key-value pair by the key if exists.
   *
   * @param key The key to remove.
   * @throws std::out_of_range for out-of-range key characters.
   */
  void remove(const std::string &key) {

    V* check_exist = search(key);
    if(check_exist == nullptr){return;}

    Trie* checker = this;
    if(checker->root->key != key){
      for(int i = 0; i < static_cast<int>(key.length()); i++){
        char next_check = key[i];
        int index = idx_of(next_check);
        checker = &(checker->root->children[index]);
        if(checker->root->key == key){
          break;
        }
      }
    }

    if(!has_children(checker->root)){
      delete checker->root;
      checker->root = nullptr;
    }
    else{
      Trie* temp = checker;
      while(has_children(temp->root)){
        for(int i = 0; i < BranchingFactor; i++){
          if(temp->root->children[i].root != nullptr){
            temp = &(temp->root->children[i]);
            break;
          }
        }
      }
      checker->root->key = temp->root->key;
      checker->root->value = temp->root->value;
      delete temp->root;
      temp->root = nullptr;


    }



  }

  /**
   * Looks up a key and inserts a default value if it is missing.
   *
   */
  V &operator[](const std::string &key) {
    V* check_exist = search(key);
    if(check_exist != nullptr){
      return *check_exist;
    }
    else{
      insert(key, V{});
      V* find = search(key);
      return *find;
    }


  }

  /**
   * Looks up a key or throws a std::out_of_range if it is missing.
   *
   * @throws std::out_of_range if the key is missing or if the key contains
   * out-of-range characters.
   */
  const V &operator[](const std::string &key) const {

    V* check_exist = search(key);
    if(check_exist != nullptr){
      return *check_exist;
    }
    else{
      throw std::out_of_range("Key does not exist in trie");
    }

  }

  /**
   * Traverses all key-value pairs in the trie.
   *
   * @param v The function to call for each key-value pair.
   */
  void traverse(std::function<void(const std::string &, const V &)> v) const {
    if(root == nullptr){return;}
    v(root->key, root->value);
    for(int i = 0; i < BranchingFactor; i++){
      static_cast<const Trie &>(root->children[i]).traverse(v);
    }


  }

  /**
   * Traverses all entries and allows value mutation.
   *
   * @param v The function to call for each key-value pair.
   */
  void traverse(std::function<void(const std::string &, V &)> v) {
    if(root == nullptr){return;}
    v(root->key, root->value);
    for(int i = 0; i < BranchingFactor; i++){
      root->children[i].traverse(v);
    }


  }

  /**
   * Merges another trie into this trie.
   *
   * @return Reference to `*this`.
   */
  Trie &operator+=(const Trie &rhs) {
    rhs.traverse([this](const std::string &key, const V &val){this->insert(key, val);});


    return *this;
  }

  /**
   * Merges two tries and returns the result as a new trie.
   */
  friend Trie operator+(Trie lhs, const Trie &rhs) {
    lhs += rhs;
    return lhs;

  }

  /**
   *  Compares key-value content for equality.
   */
  friend bool operator==(const Trie &lhs, const Trie &rhs) {
    bool match = true;
    int lhs_count = 0;
    int rhs_count = 0;

    lhs.traverse([&rhs, &match, &lhs_count](const std::string &key, const V &val){
      lhs_count++;
      const V* check = rhs.search(key);
      if(check == nullptr){match = false;}
      else if(*check != val){match = false;}
    });
    
    if(!match){return false;}

    rhs.traverse([&rhs_count](const std::string &, const V &){rhs_count++;});

    if(lhs_count == rhs_count){return true;}
    else{return false;}


  }

  /**
   *  Compares key-value content for inequality.
   */
  friend bool operator!=(const Trie &lhs, const Trie &rhs) {
    return !(lhs == rhs);


  }

  /**
   * Prints all key-value entries to a stream.
   *
   * Output format:
   * - One line for each key-value pair.
   * - Each line is `key => value`.
   * - Lines are produced in the same traversal order as `traverse()`.
   */
  friend std::ostream &operator<<(std::ostream &os, const Trie &trie) {
    trie.traverse([&os](const std::string &key, const V &val){
      os << key << " => " << val << std::endl;
    });
    return os;

  }
};

#endif // _TRIE_HPP
