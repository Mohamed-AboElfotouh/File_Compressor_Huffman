#include <chrono>
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
// Baseline Time = 4150 ms
class PerformanceNormalizer
{
private:
    double normalization_factor;

public:
    void measureBaseline()
    {

        int size = 10000;
        vector<int> arr(size);
        for (int i = 0; i < size; i++)
        {
            arr[i] = size - i;
        }

        auto start = chrono::high_resolution_clock::now();

        // Bubble sort (reference algorithm)
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size - i - 1; j++)
            {
                if (arr[j] > arr[j + 1])
                {
                    swap(arr[j], arr[j + 1]);
                }
            }
        }

        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();

        cout << "Baseline time: " << duration << " ms" << endl;
    }


    void setupNormalization(double baseline_time)
    {
        int size = 10000;
        vector<int> arr(size);
        for (int i = 0; i < size; i++)
        {
            arr[i] = size - i;
        }

        auto start = chrono::high_resolution_clock::now();

        // Bubble sort (identical to baseline)
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size - i - 1; j++)
            {
                if (arr[j] > arr[j + 1])
                {
                    swap(arr[j], arr[j + 1]);
                }
            }
        }

        auto end = chrono::high_resolution_clock::now();
        auto device_time = chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

        cout << "Test device time: " << device_time << " ms" << endl;

        normalization_factor = baseline_time / device_time;

        cout << "Normalization factor: " << normalization_factor << endl;
    }
};

int main()
{
    PerformanceNormalizer normalizer;

    normalizer.setupNormalization(4150); //Each member should run this file on their local device to get their own normalization factor.

    return 0;
}
