module;

#include <bitset>
#include <memory>
#include <unordered_map>
#include <vector>

export module pool;


/*
 *
 */

export class IPool {
public:
  virtual ~IPool() = default;
};


/*
 * A pool is just a vector of objects of type T
 */
export template <typename T>
class Pool : public IPool {
public:
  explicit Pool(int capacity = 100) {
    _size = 0;
    _data.resize(capacity);
  }

  ~Pool() override = default;

  [[nodiscard]]
  bool isEmpty() const {
    return _size == 0;
  }

  [[nodiscard]]
  int getSize() const {
    return _size;
  }

  void resize(int size) {
    _data.resize(size);
  }

  void clear() {
    _data.clear();
    _size = 0;
  }

  void add(const T& object) {
    _data.push_back(object);
  }

  void set(int entityId, const T& object) {
    if (_entityIdToIndex.contains(entityId)) {
      // Entity already exists, replace the object
      int index = _entityIdToIndex[entityId];
      _data[index] = object;
    } else {
      // Entity does not exist, add it
      int index = _size;
      _entityIdToIndex.emplace(entityId, index);
      _indexToEntityId.emplace(index, entityId);

      if (index >= _data.size()) {
        _data.resize(_size * 2);
      }
      _data[index] = object;
      _size++;
    }
  }

  T& get(int index) {
    return static_cast<T&>(_data[index]);
  }

  void remove(int index) {
    _data.erase(_data.begin() + index);
  }

  T& operator [](unsigned int index) {
    return _data[index];
  }

private:
  std::vector<T> _data;
  int _size;
  // Helper maps to keep track of entity ids
  std::unordered_map<int, int> _entityIdToIndex;
  std::unordered_map<int, int> _indexToEntityId;
};