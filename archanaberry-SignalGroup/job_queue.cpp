#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <unordered_map>
#include <functional>
#include <memory>
#include <map>
#include <vector>
#include <future>
#include <functional>
#include <memory>


// Define a Job type that holds a std::packaged_task and a std::future
template<typename ResultType>
struct Job {
    std::packaged_task<ResultType()> task;
    std::future<ResultType> result;

    Job(std::packaged_task<ResultType()> &&t) : task(std::move(t)), result(task.get_future()) {}
};

// Define a JobQueue class to manage queues of jobs
class JobQueue {
private:
    QueuesMap queues_;
    std::mutex mutex_;

    void executeQueue(Queue& queue) {
        while (true) {
            Awaitable job;
            {
                std::unique_lock<std::mutex> lock(mutex_);
                if (queue.empty()) {
                    break;
                }
                job = queue.front();
                queue.pop();
            }
            job(); // Execute the job
        }
    std::map<std::string, std::vector<std::shared_ptr<Job<void>>>> queues;
    const size_t gcLimit = 10000;

    // Function to execute jobs in a queue
    void asyncQueueExecutor(std::string bucket) {
        auto &queue = queues[bucket];
        size_t offset = 0;
        while (!queue.empty()) {
            size_t limit = std::min(queue.size(), gcLimit);
            for (size_t i = offset; i < limit; ++i) {
                auto &job = queue[i];
                job->task(); // Execute the task
            }
            // Perform lazy GC of queue for faster iteration
            if (limit >= gcLimit) {
                queue.erase(queue.begin(), queue.begin() + limit);
                offset = 0;
            } else {
                offset = limit;
            }
            queue.erase(queue.begin(), queue.begin() + offset); // Cleanup finished jobs
        }
        // Remove the queue from the map when it's empty
        queues.erase(bucket);
    }

public:
    using QueuesMap = std::unordered_map<std::string, Queue>;

    void enqueue(const std::string& bucket, Awaitable awaitable) {
        std::unique_lock<std::mutex> lock(mutex_);
        auto& queue = queues_[bucket];
        bool wasEmpty = queue.empty();
        queue.push(awaitable);

        if (wasEmpty) {
            std::thread executor(&JobQueue::executeQueue, this, std::ref(queue));
            executor.detach();
    // Function to add a job to a queue
    std::future<void> enqueueJob(std::string bucket, std::function<void()> func) {
        // Wrap the function in a packaged_task
        std::packaged_task<void()> task(func);
        // Create a new job
        auto job = std::make_shared<Job<void>>(std::move(task));
        // Add the job to the queue
        queues[bucket].push_back(job);
        // If this is the first job in the queue, start the executor
        if (queues[bucket].size() == 1) {
            std::async(std::launch::async, &JobQueue::asyncQueueExecutor, this, bucket);
        }
        // Return the future associated with the job
        return job->result;
    }
};

int main() {
    // Example usage of JobQueue
    JobQueue jobQueue;
    auto futureResult = jobQueue.enqueueJob("device1", []() {
        std::cout << "Job running" << std::endl;
    });
    futureResult.wait(); // Wait for the job to complete
    return 0;
}
