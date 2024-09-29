module;

#include <bitset>
#include <memory>
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

  explicit Pool (int size = 0) {
    _data.resize(size);
  }

  ~Pool() override = default;

  [[nodiscard]]
  bool isEmpty() const {
    return _data.empty();
  }

  [[nodiscard]]
  int getSize() const {
    return _data.size();
  }

  void resize(int size) {
    _data.resize(size);
  }

  void clear() {
    _data.clear();
  }

  void add(const T& object) {
    _data.push_back(object);
  }

  void set(int index, const T& object) {
    _data[index] = object;
  }

  T& get(int index) {
    return static_cast<T>(_data[index]);
  }

  void remove(int index) {
    _data.erase(_data.begin() + index);
  }

  T& operator [](unsigned int index) {
    return static_cast<T>(_data[index]);
  }

private:
  std::vector<T> _data;
};