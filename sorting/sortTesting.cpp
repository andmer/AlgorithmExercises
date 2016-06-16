#include <chrono>
#include <vector>
#include "heapsort.hpp"
#include "introsort.hpp"
#include "quicksort.hpp"
#include "timeUtility.hpp"

using namespace std;

void CreateRandomData(vector<int> *numbers, int dataLength, pair<int,int> dataRange);

int main() {
	for (int dataLength : { 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000 }) {
		for (pair<int,int> dataRange : { 	pair<int,int>{1,1},
																			pair<int,int>{1,10}, 
																			pair<int,int>{1,100}, 
																			pair<int,int>{1,1000}, 
																			pair<int,int>{1,1000000} }) {
			vector<int> numbers;
			for (int i=0; i<dataLength; ++i) {
				numbers.emplace_back(i);
			}
			// CreateRandomData(&numbers, dataLength, dataRange);
			typedef chrono::nanoseconds DurationType;
			
			vector<int> stdNumbers(numbers);
			DurationType stdSortDuration;
			{
				Timer<DurationType> timer(&stdSortDuration);
				sort(stdNumbers.begin(), stdNumbers.end());
				//Timer's destruction will set duration
			}
			
			DurationType heapsortDuration;
			{
				vector<int> heapsortTempNumbers(numbers);
				{
					Timer<DurationType> timer(&heapsortDuration);
					Heapsort::Sort(heapsortTempNumbers.begin(), heapsortTempNumbers.end());
					//Timer's destruction will set duration
				}
				if (stdNumbers != heapsortTempNumbers) {
					throw runtime_error("heapsort failed!");
				}
			}
			
			DurationType quicksortDuration;
			{
				vector<int> quicksortTempNumbers(numbers);
				{
					Timer<DurationType> timer(&quicksortDuration);
					Quicksort::Sort(quicksortTempNumbers.begin(), quicksortTempNumbers.end());
					//Timer's destruction will set duration
				}
				if (stdNumbers != quicksortTempNumbers) {
					throw runtime_error("quicksort failed!");
				}
			}
			
			DurationType introsortDuration;
			{
				vector<int> introsortTempNumbers(numbers);
				{
					Timer<DurationType> timer(&introsortDuration);
					Introsort::Sort(introsortTempNumbers.begin(), introsortTempNumbers.end());
					//Timer's destruction will set duration
				}
				if (stdNumbers != introsortTempNumbers) {
					throw runtime_error("introsort failed!");
				}
			}

			printf("List length:%d, range: [%d,%d]\n",dataLength, dataRange.first, dataRange.second);
			printf("    std::sort time: %.8lf seconds\n", chrono::duration_cast<chrono::duration<double>>(stdSortDuration).count());
			printf("    Heapsort time: %.8lf seconds\n", chrono::duration_cast<chrono::duration<double>>(heapsortDuration).count());
			printf("    Quicksort time: %.8lf seconds\n", chrono::duration_cast<chrono::duration<double>>(quicksortDuration).count());
			printf("    Introsort time: %.8lf seconds\n", chrono::duration_cast<chrono::duration<double>>(introsortDuration).count());
			cout << endl;
		}
	}

	return 0;
}

void CreateRandomData(vector<int> *numbers, int dataLength, pair<int,int> dataRange) {
	random_device rd;
	vector<unsigned int> seeds;
	for (unsigned int i=0; i<mt19937::state_size; ++i) {
		seeds.emplace_back(rd());
	}
	seed_seq s(seeds.begin(),seeds.end());
	mt19937 eng(s);
	uniform_int_distribution<int> dist(dataRange.first,dataRange.second);
	while (dataLength > 0) {
		numbers->emplace_back(dist(eng));
		--dataLength;
	}
}