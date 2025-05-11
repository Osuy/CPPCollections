//#include <vector>
//
//
///*
//	arr��Ϊ��������
//	(l, mid)��(mid + 1, r)
//*/
//void merge(int* arr, int l, int mid, int r)
//{
//	//�½�һ�����飬��������arr��Ԫ��
//	std::vector<int> v;
//	for (int m = l; m <= r; m++)
//	{
//		v.push_back(arr[m]);
//	}
//
//	//i��j�ֱ�ָ�����������������㣬���ڱ���
//	int i = l, j = mid + 1;
//
//	for (int k = l; k <= r; ++k)
//	{
//		if (i > mid)//������������꣬��������������
//		{
//			arr[k] = v[j - l];
//			++j;
//		}
//		else if (j > r)//������������꣬��������������
//		{
//			arr[k] = v[i - l];
//			++i;
//		}
//		else//�������鶼û�����꣬�Ƚ�i��jָ���Ԫ�أ�˭Сȡ˭��Ȼ��i/j�Լ�
//		{
//			if (v[i - l] < v[j - l])
//			{
//				arr[k] = v[i - l];
//				++i;
//			}
//			else
//			{
//				arr[k] = v[j - l];
//				++j;
//			}
//		}
//	}
//}
//
//void merge2(int* arr, int l, int mid, int r)
//{
//	if (l >= r)return;
//	int i,j;
//	std::vector<int> v;
//	for (i = l; i <= r; i++)
//		v.push_back(arr[i]);
//
//	i = l, j = mid + 1;
//	for (int k = l; k <= r; k++)
//	{
//		if (l > mid)
//			arr[k] = v[i++ - l];
//		else if (j > r)
//			arr[k] = v[j++ - l];
//		else
//		{
//			if (v[i] > v[j])
//				arr[k] = v[j++ - l];
//			else
//				arr[k] = v[i++ - l];
//		}
//	}
//}
//
////�ݹ��
//void mergesort1(int* arr, int l, int r)
//{
//	if (l >= r)
//		return;
//	 
//	int mid = (l + r) / 2;
//	//return merge(mergesort(arr, middle), mergesort(arr + middle, len - middle));
//	mergesort1(arr, l, mid);
//	mergesort1(arr, mid + 1, r);
//	merge(arr, l, mid, r);
//}
//
///*
//	�ǵݹ�汾
//	sΪ���鳤�ȣ���2��ʼ��ÿ�η���
//	������ֳɴ�СΪs�������飬Ȼ�����merge
//	����β�˿��ܳ��ֲ���s���������Ҫ��ѭ�����⴦��
//
//*/
//void mergesort1(int* arr, int len)
//{
//	int s = 2, i;
//	while (s <= len)//��ѭ������s����ʼΪ2��ÿ�α������������Ϊlen/s��С�飬ÿ��s��Ԫ�ؽ���merge
//	{
//		i = 0;
//		while (i + s <= len)//��ѭ������i����ʼΪ0��ÿ�μ���s�����Բ���s��������
//		{
//			merge(arr, i, i + (s - 1) / 2, i + s - 1);
//			i += s;
//		}
//		
//		//���ܻ���n-s<i<n-1���������ʱ��ʣ�ಿ��Ϊ�鲢����ȴ�˳���ѭ��
//		//�ֶ��鲢
//		merge(arr, i + 1, (i + len) / 2, len - 1);
//
//		s *= 2;//s����
//	}
//
//	//�˳�ѭ���󣬵õ���������δ����ã�������������õ������飬��Ҫ�ٶ����������ֶ��鲢һ��
//	merge(arr, 0, s / 2 - 1, len - 1);
//}
//
//void mergesort2(int* arr, int len)
//{
//	int s = 2, i;
//	while (s <= len)
//	{
//		i = 0;
//		while (i + s < len)
//		{
//			merge(arr, i, i + (s - 1) / 2, i + s - 1);
//			i += s;
//		}
//		merge(arr, i, (i + len - 1) / 2, len - 1);
//		s *= 2;
//	}
//
//	merge(arr, 0, s / 2 - 1, len - 1);
//}
