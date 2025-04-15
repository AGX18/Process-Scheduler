
#include "process.h"
#include <iostream>
#include <vector>
#include <mutex>
#include <shared_mutex>
#include <algorithm>

class Data {
private:
    static Data* instance;
    static std::mutex instance_mutex;

    std::vector<Process> ProcessVector;
    mutable std::shared_mutex data_mutex;

    Data() = default;
    Data(const Data&) = delete;
    Data& operator=(const Data&) = delete;

public:
    // Thread-safe instance access
    static Data& getInstance() {
        std::lock_guard<std::mutex> lock(instance_mutex);
        if (instance == nullptr) {
            instance = new Data();
        }
        return *instance;
    }

    // Add single value to vector
    void storeData(Process value) {
        std::unique_lock<std::shared_mutex> lock(data_mutex);
        ProcessVector.push_back(value);
    }

    // Add multiple values at once
    void storeData(const std::vector<Process>& values) {
        std::unique_lock<std::shared_mutex> lock(data_mutex);
        ProcessVector.insert(ProcessVector.end(), values.begin(), values.end());
    }

    // Add multiple values using move semantics
    void storeData(std::vector<Process>&& values) {
        std::unique_lock<std::shared_mutex> lock(data_mutex);
        ProcessVector.insert(ProcessVector.end(),
                           std::make_move_iterator(values.begin()),
                           std::make_move_iterator(values.end()));
    }

    // Get copy of all data
    std::vector<Process> getAllData() const {
        std::shared_lock<std::shared_mutex> lock(data_mutex);
        return ProcessVector;
    }

    // Get reference to data (use with caution - ensure proper locking)
    const std::vector<Process>& getDataRef() const {
        return ProcessVector; // Caller must ensure they have a shared_lock
    }

    // // Get data that matches a predicate
    // std::vector<Process> getDataIf(std::function<bool(Process)> predicate) const {
    //     std::shared_lock<std::shared_mutex> lock(data_mutex);
    //     std::vector<Process> result;
    //     std::copy_if(ProcessVector.begin(), ProcessVector.end(),
    //                  std::back_inserter(result), predicate);
    //     return result;
    // }

    // Clear all data
    void clearData() {
        std::unique_lock<std::shared_mutex> lock(data_mutex);
        ProcessVector.clear();
    }

    // Get current size
    size_t getDataSize() const {
        std::shared_lock<std::shared_mutex> lock(data_mutex);
        return ProcessVector.size();
    }

    static void destroyInstance() {
        std::lock_guard<std::mutex> lock(instance_mutex);
        if (instance != nullptr) {
            delete instance;
            instance = nullptr;
        }
    }
};

// Initialize static members
Data* Data::instance = nullptr;
std::mutex Data::instance_mutex;
