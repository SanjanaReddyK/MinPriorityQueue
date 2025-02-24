#include <iostream>
#include <vector>

using namespace std;

class MinPriorityQueue {
private:
    vector<pair<int, int>> heap; // heap storing pairs of (key, value)
    int size; // size of the heap

    int find(int value) {
	for (int i= 0; i<size; i++) {
	    if (heap[i].second == value) {
		return i;
	    }
	}
	return -1;
    }

    void heapify_up(int index) {
        if (index == 0) {
            return;
        }
        int parent_index = (index - 1) / 2;
        if (heap[index].first < heap[parent_index].first) {
            swap(heap[index], heap[parent_index]);
            heapify_up(parent_index);
        }
    }

    void heapify_down(int index) {
        int left_child_index = 2 * index + 1;
        int right_child_index = 2 * index + 2;
        int smallest_index = index;
        if (left_child_index < size && heap[left_child_index].first < heap[smallest_index].first) {
            smallest_index = left_child_index;
        }
        if (right_child_index < size && heap[right_child_index].first < heap[smallest_index].first) {
            smallest_index = right_child_index;
        }
        if (smallest_index != index) {
            swap(heap[index], heap[smallest_index]);
            heapify_down(smallest_index);
        }
    }

public:
    MinPriorityQueue() : size(0) {}

    void insert(int key, int value) {
        heap.push_back(make_pair(key, value));
        heapify_up(size);
        size++;
    }

    int extract_min() {
        if (size == 0) {
            throw out_of_range("Heap is empty");
        }
        int min_value = heap[0].second;
        heap[0] = heap[size-1];
        size--;
        heap.pop_back();
        heapify_down(0);
        return min_value;
    }

    void decrease_key(int value, int new_key) {
	int index = find(value);
	if (index < 0 || index >= size) {
            throw invalid_argument("Value is not exist");
	}
        if (new_key > heap[index].first) {
            throw invalid_argument("New key is greater than current key");
        }
        heap[index].first = new_key;
        heapify_up(index);
    }

    bool is_empty() {
        return size == 0;
    }
};
