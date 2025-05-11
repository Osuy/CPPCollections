#include <cstdio> /*三数取中+插入+聚集元素+多线程组合  &&  三数取中+插入+尾递归+多线程组合*/
#include <iostream>
#include <stdlib.h>
#include <sys/time.h>
#include <thread>

using namespace std;

const long MAX = 1000000L;                          //数组中最大数
const long long MaxNumber = 1000000L;               //排序数
const int Numthread = 4;                               //线程数
const long NumberOfSort = MaxNumber / Numthread;       //每个线程排序的个数

int array_a[MaxNumber];
int array_b[MaxNumber];                             //合并后，由b数组记录最终序列

 barrier;

void initial()   //数组初始化函数
{
	srand(time(0));
	for (int i = 0; i < MaxNumber; ++i)
		array_a[i] = rand() % (MAX);
	//if(i < MaxNumber/2)
	//    array_a[i] = 5;
	//else
	//   array_a[i] = 10;
	//array_a[i] = i+1;
}


template <class T>
void Print(T a[], int n)
{
	for (int i = 0; i < n; i++)
	{
		cout << "[" << a[i] << "]";
	}
	cout << endl;
}

template <class T>
void Swap(T& a, T& b)
{
	T asd;
	asd = a;
	a = b;
	b = asd;
}

void InsertSort(int arr[], int start, int end)
{
	int low, high, median, tmp;
	for (int i = start + 1; i <= end; i++)
	{
		low = start;
		high = i - 1;

		while (low <= high)
		{
			median = (low + high) / 2;
			if (arr[i] < arr[median])
			{
				high = median - 1;
			}
			else
			{
				low = median + 1;
			}
		}

		tmp = arr[i];

		for (int j = i - 1; j > high; j--)
		{
			arr[j + 1] = arr[j];
		}
		arr[high + 1] = tmp;
	}
}

int NumberOfThree(int arr[], int low, int high)
{
	int mid = low + ((high - low) >> 1);
	if (arr[mid] > arr[high])
	{
		Swap(arr[mid], arr[high]);
	}
	if (arr[low] > arr[high])
	{
		Swap(arr[low], arr[high]);
	}
	if (arr[mid] > arr[low])
	{
		Swap(arr[mid], arr[low]);
	}
	return arr[low];
}

template <class T>
int Partition(T a[], int p, int r)
{
	int i = p, j = r + 1;
	T x = NumberOfThree(a, p, r);
	while (true)
	{
		while (a[++i] < x && i < r);
		while (a[--j] > x);
		if (i >= j)break;
		Swap(a[i], a[j]);
	}
	a[p] = a[j];
	a[j] = x;
	return j;
}

#if 1   //有聚集元素的快排
template <class T>
void QSort(T arr[], int low, int high)
{
	int first = low;
	int last = high;
	int left = low;
	int right = high;
	int leftLen = 0;
	int rightLen = 0;

	if (high - low + 1 < 6)
	{
		InsertSort(arr, low, high);
		return;
	}

	//一次分割
	int key = NumberOfThree(arr, low, high);//使用三数取中法选择枢轴

	while (low < high)
	{
		while (high > low && arr[high] >= key)
		{
			if (arr[high] == key)//处理相等元素
			{
				Swap(arr[right], arr[high]);
				right--;
				rightLen++;
			}
			high--;
		}
		arr[low] = arr[high];
		while (high > low && arr[low] <= key)
		{
			if (arr[low] == key)
			{
				Swap(arr[left], arr[low]);
				left++;
				leftLen++;
			}
			low++;
		}
		arr[high] = arr[low];
	}
	arr[low] = key;

	//一次快排结束
	//把与枢轴key相同的元素移到枢轴最终位置周围
	int i = low - 1;
	int j = first;
	while (j < left && arr[i] != key)
	{
		Swap(arr[i], arr[j]);
		i--;
		j++;
	}
	i = low + 1;
	j = last;
	while (j > right && arr[i] != key)
	{
		Swap(arr[i], arr[j]);
		i++;
		j--;
	}
	QSort(arr, first, low - 1 - leftLen);
	QSort(arr, low + 1 + rightLen, last);
}
#endif

#if 0     //没有聚集元素的快排
template <class T>
void QSort(T arr[], int low, int high)
{
	int pivotPos;
	if (high - low + 1 < 10)
	{
		InsertSort(arr, low, high);
		return;
	}

	while (low < high)
	{
		pivotPos = Partition(arr, low, high);
		QSort(arr, low, pivotPos - 1);
		low = pivotPos + 1;
	}
}
#endif // 1

void* work(void* arg)  //线程排序函数
{
	long length = (long)arg;
	QSort(array_a, length, length + NumberOfSort - 1);
	pthread_barrier_wait(&barrier);
	pthread_exit(NULL);
}

void meger()        //最终合并函数
{
	long index[thread];
	for (int i = 0; i < thread; ++i)
	{
		index[i] = i * NumberOfSort;
	}

	for (long i = 0; i < MaxNumber; ++i)
	{
		long min_index;
		long min_num = MAX;
		for (int j = 0; j < thread; ++j)
		{
			if ((index[j] < (j + 1) * NumberOfSort) && (array_a[index[j]] < min_num))
			{
				min_index = j;
				min_num = array_a[index[j]];
			}
		}
		array_b[i] = array_a[index[min_index]];
		index[min_index]++;
	}
}


int Test(int argc, char const* argv[])
{
	initial();
	//Print(array_a,MaxNumber);
	struct timeval start, end;
	pthread_t ptid;
	gettimeofday(&start, NULL);

	pthread_barrier_init(&barrier, NULL, thread + 1);
	for (int i = 0; i < thread; ++i)
		pthread_create(&ptid, NULL, work, (void*)(i * NumberOfSort));

	pthread_barrier_wait(&barrier);
	meger();

	gettimeofday(&end, NULL);
	long long s_usec = start.tv_sec * 1000000 + start.tv_usec;
	long long e_usec = end.tv_sec * 1000000 + end.tv_usec;

	double Time = (double)(e_usec - s_usec) / 1000.0;
	printf("sort use %.4f ms\n", Time);
	//Print(array_b, MaxNumber);
	return 0;
}


