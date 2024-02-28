#include <vector>
#include <string>
#include <fstream>
#include <functional>

template <class T>
class Database {
    std::string filename;

public:
    Database(std::string filename) {
        this->filename = filename;
    }

    std::vector<T> load_all() {
        std::vector<T> objects;
        std::ifstream file(filename);
        T obj;
        while (file >> obj) {
            objects.push_back(obj);
        }
        return objects;
    }

    void insert(T& obj) {
        std::ofstream file(filename, std::ios::app);
        file << obj << '\n';
    }

    void remove(T& target) {
        std::vector<T> objects = load_all();
        std::ofstream file(filename, std::ios::trunc);

        bool deleted = false;
        for (T& obj : objects) {
            if (!deleted && obj == target) {
                deleted = true;
            } else {
                file << obj << '\n';
            }
        }
    }

    void update(T& target, T& new_obj) {
        std::vector<T> objects = load_all();
        std::ofstream file(filename, std::ios::trunc);

        bool updated = false;
        for (const T& obj : objects) {
            if (!updated && obj == target) {
                file << new_obj << '\n';
                updated = true;
            } else {
                file << obj << '\n';
            }
        }
    }

    std::vector<T> search(std::function<bool(T&)> compare) {
        std::vector<T> objects = load_all();
        std::vector<T> results;
        for (T& obj : objects) {
            if (compare(obj)) {
                results.push_back(obj);
            }
        }
        return results;
    }
};
