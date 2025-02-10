#include <iostream>

// �� ���� ���� ��ȯ�ϴ� �Լ�.
void Swap(int& a, int& b)
{
	int temp = a;
	a = b;
	b = temp;
}

// ���� ���� �Լ�.
void BubbleSort(int* array, int length)
{
	// ����.
	for (int ix = 0; ix < length - 1; ++ix)
	{
		for (int jx = 0; jx < length - 1 - ix; ++jx)
		{
			// �ΰ��� ��.
			if (array[jx] > array[jx + 1])
			{
				// ��ȯ.
				Swap(array[jx], array[jx + 1]);
			}
		}
	}
}

// ��� �Լ�.
void PrintArray(int* array, int length)
{
	for (int ix = 0; ix < length; ++ix)
	{
		std::cout << array[ix] << " ";
	}

	std::cout << "\n";
}

int main()
{
	// �迭.
	int array[] = { 5, 3, 2, 8, 10, 15, 20, 23, 4, 1, 7, 8, 11, 13, 21 };

	// �迭 ����.
	int length = sizeof(array) / sizeof(int);

	// ���.
	std::cout << "���� ��: ";
	PrintArray(array, length);

	// ����.
	BubbleSort(array, length);
	
	std::cout << "���� ��: ";
	PrintArray(array, length);

	std::cin.get();
}