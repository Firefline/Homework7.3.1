// Homework7.3.1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <vector>
#include <random>
#include <thread>
#include <future>

void get_vector(std::vector<int> &vector_, int size_)
{
    std::random_device rd;
    std::mt19937 gen{ rd() };
    std::uniform_int_distribution <> dis(0, 1000);

    int value = dis(gen);

    for (int i = 0; i < size_; ++i)
    {
        vector_.push_back(value);
        value = dis(gen);
    }
}

void get_min(std::vector<int>& vector_, std::promise<int> prom_)
{
    int min = 0;
    int temp = 0;
    for (int i = 0; i < vector_.size(); ++i)
    {
        min = i;

        for (int j = i + 1; j < vector_.size(); ++j)
        {
            if (vector_[j] < vector_[min])
            {
                min = j;
            }
        }

        if (i != min)
        {
            temp = std::move(vector_[i]);
            vector_[i] = std::move(vector_[min]);
            vector_[min] = std::move(temp);
        }
    }
    prom_.set_value(true);
}

int main(int argc, char** argv)
{
    setlocale(LC_ALL, "Russian");

    std::vector<int> vector_test;

    get_vector(vector_test, 30);

    std::promise<int> prom;
    std::future<int> fut = prom.get_future();

    int min = 0;
    int temp = 0;

    auto res = std::async(std::launch::async, [&vector_test, &prom]()
        {
            get_min(vector_test, std::move(prom));
        }
    );
    fut.wait();
    fut.get();

    for (int i = 0; i < vector_test.size(); i++)
    {
        std::cout << vector_test[i] << " ";
    }

    return 0;
}