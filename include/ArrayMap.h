#pragma once
#include <array>
#include <cstddef>
#include <optional>
#include <stack>
#include <stdexcept>
#include <vector>

template <typename T, std::size_t Size> class ArrayMap {
  private:
    std::array<std::optional<T>, Size> data;
    std::stack<int> freeIndices;

  public:
    ArrayMap() {
        data.fill(std::nullopt);
        // Initialize all indices as available
        for (int i = Size - 1; i >= 0; --i) {
            freeIndices.push(i);
        }
    }

    int insert(T element) {
        if (freeIndices.empty()) {
            return -1;
        }
        int id = freeIndices.top();
        freeIndices.pop();
        data[id] = element;
        return id;
    }

    T get(int id) const {
        if (id < 0 || id >= Size || !data[id].has_value()) {
            throw std::runtime_error("Invalid id");
        }
        return *data[id];
    }

    bool remove(int id) {
        if (id < 0 || id >= Size || !data[id].has_value())
            return false;
        data[id].reset();
        // freeIndices.push(id);
        return true;
    }

    bool exists(int id) const {
        return id >= 0 && id < Size && data[id].has_value();
    }

    void clear() {
        data.fill(std::nullopt);

        // Reset freeIndices
        while (!freeIndices.empty()) {
            freeIndices.pop();
        }
        for (int i = Size - 1; i >= 0; --i) {
            freeIndices.push(i);
        }
    }

    std::vector<std::pair<int, T &>> getAllValidEntries() {
        // ex: [(id, element)]
        std::vector<std::pair<int, T &>> entries;
        for (int i = 0; i < Size; ++i) {
            if (data[i].has_value()) {
                entries.emplace_back(i, *data[i]);
            }
        }
        return entries;
    }
};
