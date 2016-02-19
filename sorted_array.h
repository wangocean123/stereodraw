#pragma once

#include <vector>

template<class T>
class sorted_array : public std::vector<T>
{
public:
	sorted_array()
	{

	}
	~sorted_array()
	{

	}

	void add_sort(const T& a)
	{
		int insert_index = -1;
		if (find_sort(a, &insert_index))
		{
			insert(begin() + insert_index, a);
		}
		else
		{
			insert(begin() + insert_index, a);
		}
	}

	void add_sort_no_repeated(const T& a)
	{
		int insert_index = -1;
		if (find_sort(a, &insert_index))
		{			
		}
		else
		{
			insert(begin() + insert_index, a);
		}
	}

	void remove_sort(const T& a)
	{
		int insert_index = -1;
		if (find_sort(a, &insert_index))
		{
			std::vector<T>& arr = *this;
			for (int i = insert_index; i < size(); )
			{
				if (arr[i] == a)
				{
					erase(begin() + i);
				}
			}
			for (int i = insert_index-1; i >=0;i--)
			{
				if (arr[i] == a)
				{
					erase(begin() + i);
				}
			}
		}
	}

	void remove_sort_no_repeated(const T& a)
	{
		int insert_index = -1;
		if (find_sort(a, &insert_index))
		{
			erase(begin() + insert_index);
		}
	}

	bool find_sort(const T& a, int *insert_index)
	{
		std::vector<T>& arr = *this;
		int size = arr.size();
		int index = size;
		bool bret = false;

		//二分法查找
		if (a <= arr[size - 1] && a >= arr[0])
		{
			int start = 0, end = size - 1;
			int mid = (start + end) / 2;

			while (start < end && a != arr[mid])
			{
				if (a < arr[mid])
				{
					end = mid - 1;
				}
				else if (a > arr[mid])
				{
					start = mid + 1;
				}

				mid = (start + end) / 2;
			}

			index = mid;

			if (a == arr[mid])
				return true;
		}

		return bret;
	}
};