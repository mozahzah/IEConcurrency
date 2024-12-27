// SPDX-License-Identifier: GPL-2.0-only
// Copyright © Interactive Echoes. All rights reserved.
// Author: mozahzah

#include <chrono>
#include <iostream>
#include <string>
#include <thread>

#include "IEConcurrency.h"

static void Test_IESpinOnWriteObject()
{
    /*
        This test demonstrates how the Reader thread blocks the Writer thread when accessing the shared data (IESpinOnWriteObjectstd::string).
        During each read operation, the Writer thread attempts to write new data but is blocked until the Reader completes its task.
        After the first read operation, the Writer is able to update the data, and in the second read operation, the updated data is successfully retrieved.
        Notably, the read operation is both lock-free and wait-free.
    */

    IESpinOnWriteObject<std::string> Data(std::string("[OLD DATA]"));
    const int ReaderSimulatedWorkTime = 3000;
    
    std::thread Reader([&Data](int SimulatedWorkTime)
        {
            for (int i = 0; i < 2; i++)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(300)); // Add small delay between reads
                IESpinOnWriteObject<std::string>::LockedValue LockedData = Data.LockForRead();
                std::cout << "Reading Locked Data: " << LockedData.Value.c_str() << "\n";
                std::this_thread::sleep_for(std::chrono::milliseconds(SimulatedWorkTime));
                std::cout << "Unlocked Data\n";
            }

        }, ReaderSimulatedWorkTime);

    std::this_thread::sleep_for(std::chrono::milliseconds(400)); // Add small delay before writing new data.
    std::thread Writer([&Data]()
        {
            std::cout << "Waiting to Write New Data...\n";
            std::string NewData = std::string("[NEW DATA]");
            Data.Write(NewData);
            std::cout << "New Data Written: " << NewData.c_str() << "\n";
        });

    Writer.join();
    Reader.join();

    std::cout << "\nIESpinOnWriteObject Test Finished.\n";
}

static void Test_IESPSCQueue()
{
    /*
        This test demonstrates the behavior of the IESPSCQueue<int> in a producer-consumer scenario.
        A Producer thread generates data and attempts to push it into the queue, retrying if the queue is full.
        Meanwhile, a Consumer thread retrieves and processes data from the queue.
        The Producer and Consumer run concurrently, with the Producer generating items and the Consumer consuming them at different simulated work intervals.
        The test highlights the synchronization between the threads and the queue’s handling of push and pop operations.
        The test concludes once all items are processed.
    */

    IESPSCQueue<int> Queue(5);
    const int ProducerSimulatedWorkTime = 100;
    const int ConsumerSimulatedWorkTime = 300;

    std::thread Producer([&Queue](int SimulatedWorkTime)
    {
        for (int i = 0; i < 10; ++i)
        {
            while (!Queue.Push(i))
            {
                std::cout << "Queue is full, retrying to push " << i << "...\n";
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
            std::cout << "Produced: " << i << "\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(SimulatedWorkTime));
        }
        }, ProducerSimulatedWorkTime);
    
    std::thread Consumer([&Queue](int SimulatedWorkTime)
        {
        for (int i = 0; i < 10; ++i)
        {
            std::optional<decltype(Queue)::ValueType> Value = Queue.Pop();
            if (Value)
            {
                std::cout << "Consumed: " << *Value << "\n";
            }
            else
            {
                std::cout << "Queue is empty, waiting for data...\n";
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(SimulatedWorkTime));
        }
    }, ConsumerSimulatedWorkTime);

    Producer.join();
    Consumer.join();

    std::cout << "\nIESPSCQueue Test Finished.\n";
}

int main()
{
    /* Comment/Uncomment whicher test to run */

    //Test_IESpinOnWriteObject();
    //Test_IESPSCQueue();

    return 0;
}