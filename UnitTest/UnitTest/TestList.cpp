#include "..\..\Library\UnitTest\UnitTest.h"
#include "..\..\Library\Collections\List.h"
#include "..\..\Library\Collections\Dictionary.h"
#include "..\..\Library\Collections\Operation.h"

using namespace vl;
using namespace vl::collections;

#define _ ,
#define CHECK_EMPTY_LIST(CONTAINER) TestReadonlyList(CONTAINER, 0, 0)
#define CHECK_LIST_ITEMS(CONTAINER,ITEMS)do{int __items__[]=ITEMS;TestReadonlyList(CONTAINER,__items__, sizeof(__items__)/sizeof(*__items__));}while(0)
#define CHECK_EMPTY_DICTIONARY(CONTAINER) TestReadonlyDictionary(CONTAINER, 0, 0, 0);
#define CHECK_DICTIONARY_ITEMS(CONTAINER,KEYS,VALUES)do{int __keys__[]=KEYS;int __values__[]=VALUES;TestReadonlyDictionary(CONTAINER, __keys__, __values__, sizeof(__keys__)/sizeof(*__keys__));}while(0)
#define CHECK_EMPTY_GROUP(CONTAINER) TestReadonlyGroup(CONTAINER, 0, 0, 0, 0);
#define CHECK_GROUP_ITEMS(CONTAINER,KEYS,VALUES,COUNTS)do{int __keys__[]=KEYS;int __values__[]=VALUES;int __counts__[]=COUNTS;TestReadonlyGroup(CONTAINER, __keys__, __values__, __counts__, sizeof(__keys__)/sizeof(*__keys__));}while(0)

void TestReadonlyList(const IReadonlyList<int>& list, int* items, int count)
{
	TEST_ASSERT(list.Count()==count);
	IEnumerator<int>* enumerator=list.CreateEnumerator();
	for(int i=0;i<count;i++)
	{
		TEST_ASSERT(list.Contains(items[i]));
		TEST_ASSERT(list.Get(i)==items[i]);
		TEST_ASSERT(list[i]==items[i]);
		TEST_ASSERT(list[list.IndexOf(items[i])]==items[i]);
		TEST_ASSERT(enumerator->Available()==true);
		TEST_ASSERT(enumerator->Current()==items[i]);
		TEST_ASSERT(enumerator->Index()==i);
		TEST_ASSERT(enumerator->Next()==(i!=count-1));
	}
	TEST_ASSERT(enumerator->Available()==false);
	delete enumerator;
}

template<typename T>
void CompareEnumerable(const IEnumerable<T>& dst, const IEnumerable<T>& src)
{
	IEnumerator<T>* dstEnum=dst.CreateEnumerator();
	IEnumerator<T>* srcEnum=src.CreateEnumerator();
	while(dstEnum->Available())
	{
		TEST_ASSERT(dstEnum->Available()==srcEnum->Available());
		TEST_ASSERT(dstEnum->Current()==srcEnum->Current());
		TEST_ASSERT(dstEnum->Index()==srcEnum->Index());
		TEST_ASSERT(dstEnum->Next()==srcEnum->Next());
	}
	TEST_ASSERT(dstEnum->Available()==false);
	TEST_ASSERT(srcEnum->Available()==false);
	delete dstEnum;
	delete srcEnum;
}

void TestArray(IArray<int>& arr)
{
	arr.Resize(0);
	CHECK_EMPTY_LIST(arr);

	arr.Resize(10);
	for(int i=0;i<5;i++)
	{
		arr[i]=i;
	}
	for(int i=5;i<10;i++)
	{
		arr.Set(i, i);
	}
	CHECK_LIST_ITEMS(arr, {0 _ 1 _ 2 _ 3 _ 4 _ 5 _ 6 _ 7 _ 8 _ 9});

	arr.Resize(15);
	for(int i=10;i<15;i++)
	{
		arr.Set(i, i*2);
	}
	CHECK_LIST_ITEMS(arr, {0 _ 1 _ 2 _ 3 _ 4 _ 5 _ 6 _ 7 _ 8 _ 9 _ 20 _ 22 _ 24 _ 26 _ 28});

	arr.Resize(5);
	CHECK_LIST_ITEMS(arr, {0 _ 1 _ 2 _ 3 _ 4});
}

void TestCollectionWithIncreasingItems(ICollection<int>& collection)
{
	collection.Clear();
	CHECK_EMPTY_LIST(collection);
	
	collection.Add(0);
	CHECK_LIST_ITEMS(collection, {0});

	for(int i=1;i<10;i++)
	{
		collection.Add(i);
	}
	CHECK_LIST_ITEMS(collection, {0 _ 1 _ 2 _ 3 _ 4 _ 5 _ 6 _ 7 _ 8 _ 9});

	collection.RemoveAt(2);
	CHECK_LIST_ITEMS(collection, {0 _ 1 _ 3 _ 4 _ 5 _ 6 _ 7 _ 8 _ 9});

	collection.RemoveRange(3, 4);
	CHECK_LIST_ITEMS(collection, {0 _ 1 _ 3 _ 8 _ 9});

	TEST_ASSERT(collection.Remove(3)==true);
	CHECK_LIST_ITEMS(collection, {0 _ 1 _ 8 _ 9});

	TEST_ASSERT(collection.Remove(9)==true);
	CHECK_LIST_ITEMS(collection, {0 _ 1 _ 8});

	TEST_ASSERT(collection.Remove(0)==true);
	CHECK_LIST_ITEMS(collection, {1 _ 8});

	for(int i=0;i<10;i++)
	{
		if(i!=1 && i!=8)
		{
			TEST_ASSERT(collection.Remove(i)==false);
			CHECK_LIST_ITEMS(collection, {1 _ 8});
		}
	}

	collection.Clear();
	CHECK_EMPTY_LIST(collection);
}

void TestSortedCollection(ICollection<int>& collection)
{
	int items[]={7, 1, 12, 2, 8, 3, 11, 4, 9, 5, 13, 6, 10};
	collection.Clear();
	for(int i=0;i<sizeof(items)/sizeof(*items);i++)
	{
		collection.Add(items[i]);
	}
	CHECK_LIST_ITEMS(collection, {1 _ 2 _ 3 _ 4 _ 5 _ 6 _ 7 _ 8 _ 9 _ 10 _ 11 _ 12 _ 13});

	TEST_ASSERT(collection.Remove(7)==true);
	CHECK_LIST_ITEMS(collection, {1 _ 2 _ 3 _ 4 _ 5 _ 6 _ 8 _ 9 _ 10 _ 11 _ 12 _ 13});

	TEST_ASSERT(collection.Remove(1)==true);
	CHECK_LIST_ITEMS(collection, {2 _ 3 _ 4 _ 5 _ 6 _ 8 _ 9 _ 10 _ 11 _ 12 _ 13});

	TEST_ASSERT(collection.Remove(12)==true);
	CHECK_LIST_ITEMS(collection, {2 _ 3 _ 4 _ 5 _ 6 _ 8 _ 9 _ 10 _ 11 _ 13});

	TEST_ASSERT(collection.Remove(2)==true);
	CHECK_LIST_ITEMS(collection, {3 _ 4 _ 5 _ 6 _ 8 _ 9 _ 10 _ 11 _ 13});

	TEST_ASSERT(collection.Remove(8)==true);
	CHECK_LIST_ITEMS(collection, {3 _ 4 _ 5 _ 6 _ 9 _ 10 _ 11 _ 13});

	TEST_ASSERT(collection.Remove(3)==true);
	CHECK_LIST_ITEMS(collection, {4 _ 5 _ 6 _ 9 _ 10 _ 11 _ 13});

	for(int i=0;i<sizeof(items)/sizeof(*items);i++)
	{
		collection.Add(items[i]);
	}
	CHECK_LIST_ITEMS(collection, {1 _ 2 _ 3 _ 4 _ 4 _ 5 _ 5 _ 6 _ 6 _ 7 _ 8 _ 9 _ 9 _ 10 _ 10 _ 11 _ 11 _ 12 _ 13 _ 13});

	collection.Clear();
	CHECK_EMPTY_LIST(collection);
}

void TestNormalList(IList<int>& list)
{
	list.Clear();
	for(int i=0;i<10;i++)
	{
		list.Insert(i/2,i);
	}
	CHECK_LIST_ITEMS(list, {1 _ 3 _ 5 _ 7 _ 9 _ 8 _ 6 _ 4 _ 2 _ 0});

	for(int i=0;i<10;i++)
	{
		list[i]=9-i;
	}
	CHECK_LIST_ITEMS(list, {9 _ 8 _ 7 _ 6 _ 5 _ 4 _ 3 _ 2 _ 1 _ 0});

	for(int i=0;i<10;i++)
	{
		list.Set(i,i*2);
	}
	CHECK_LIST_ITEMS(list, {0 _ 2 _ 4 _ 6 _ 8 _ 10 _ 12 _ 14 _ 16 _ 18});
}

void TestReadonlyDictionary(const IReadonlyDictionary<int, int>& dictionary, int* keys, int* values, int count)
{
	TEST_ASSERT(dictionary.Count()==count);
	TestReadonlyList(dictionary.Keys(), keys, count);
	TestReadonlyList(dictionary.Values(), values, count);
	for(int i=0;i<count;i++)
	{
		TEST_ASSERT(dictionary.Get(keys[i])==values[i]);
	}

	IEnumerator<Pair<int, int>>* enumerator=dictionary.CreateEnumerator();
	for(int i=0;i<count;i++)
	{
		Pair<int, int> pair(keys[i], values[i]);
		TEST_ASSERT(enumerator->Available()==true);
		TEST_ASSERT(enumerator->Current()==pair);
		TEST_ASSERT(enumerator->Index()==i);
		TEST_ASSERT(enumerator->Next()==(i!=count-1));
	}
	TEST_ASSERT(enumerator->Available()==false);
	delete enumerator;
}

void TestSortedDictionary(IDictionary<int, int>& dictionary)
{
	dictionary.Clear();
	CHECK_EMPTY_DICTIONARY(dictionary);

	dictionary.Add(1, 1);
	dictionary.Add(2, 4);
	dictionary.Add(3, 9);
	dictionary.Add(4, 16);
	CHECK_DICTIONARY_ITEMS(dictionary, {1 _ 2 _ 3 _ 4}, {1 _ 4 _ 9 _ 16});

	dictionary.Set(1, -1);
	dictionary.Set(2, -4);
	dictionary.Set(3, -9);
	dictionary.Set(4, -16);
	dictionary.Set(5, -25);
	CHECK_DICTIONARY_ITEMS(dictionary, {1 _ 2 _ 3 _ 4 _ 5}, {-1 _ -4 _ -9 _ -16 _ -25});

	dictionary.Remove(4);
	dictionary.Remove(5);
	dictionary.Remove(6);
	CHECK_DICTIONARY_ITEMS(dictionary, {1 _ 2 _ 3}, {-1 _ -4 _ -9});

	dictionary.Clear();
	CHECK_EMPTY_DICTIONARY(dictionary);
}

void TestReadonlyGroup(const IReadonlyGroup<int, int>& group, int* keys, int* values, int* counts, int count)
{
	TEST_ASSERT(group.Count()==count);
	TestReadonlyList(group.Keys(), keys, count);
	int offset=0;
	for(int i=0;i<count;i++)
	{
		TEST_ASSERT(group.Contains(keys[i])==true);
		TestReadonlyList(group.Get(keys[i]), values+offset, counts[i]);
		TestReadonlyList(group[keys[i]], values+offset, counts[i]);
		TestReadonlyList(group.GetByIndex(i), values+offset, counts[i]);
		for(int j=0;j<counts[i];j++)
		{
			TEST_ASSERT(group.Contains(keys[i], values[offset+j]));
		}
		offset+=counts[i];
	}

	IEnumerator<Pair<int, int>>* enumerator=group.CreateEnumerator();
	int keyIndex=0;
	int valueIndex=0;
	int index=0;
	while(keyIndex<count)
	{
		Pair<int, int> pair(keys[keyIndex], values[index]);
		TEST_ASSERT(enumerator->Available()==true);
		TEST_ASSERT(enumerator->Current()==pair);
		TEST_ASSERT(enumerator->Index()==index);
		TEST_ASSERT(enumerator->Next()==!(keyIndex==count-1 && valueIndex==counts[keyIndex]-1));

		valueIndex++;
		index++;
		if(valueIndex==counts[keyIndex])
		{
			keyIndex++;
			valueIndex=0;
		}
	}
	TEST_ASSERT(enumerator->Available()==false);
	delete enumerator;
}

void TestSortedGroup(IGroup<int, int>& group)
{
	group.Clear();
	CHECK_EMPTY_GROUP(group);

	for(int i=0;i<20;i++)
	{
		group.Add(i/5, i);
	}
	CHECK_GROUP_ITEMS(group, {0 _ 1 _ 2 _ 3}, {0 _ 1 _ 2 _ 3 _ 4 _ 5 _ 6 _ 7 _ 8 _ 9 _ 10 _ 11 _ 12 _ 13 _ 14 _ 15 _ 16 _ 17 _ 18 _ 19}, {5 _ 5 _ 5 _ 5});

	group.Remove(1);
	CHECK_GROUP_ITEMS(group, {0 _ 2 _ 3}, {0 _ 1 _ 2 _ 3 _ 4 _ 10 _ 11 _ 12 _ 13 _ 14 _ 15 _ 16 _ 17 _ 18 _ 19}, {5 _ 5 _ 5});

	for(int i=13;i<18;i++)
	{
		group.Remove(i/5, i);
	}
	CHECK_GROUP_ITEMS(group, {0 _ 2 _ 3}, {0 _ 1 _ 2 _ 3 _ 4 _ 10 _ 11 _ 12 _ 18 _ 19}, {5 _ 3 _ 2});

	group.Clear();
	CHECK_EMPTY_GROUP(group);
}

TEST_CASE(TestArray)
{
	Array<int> arr;
	TestArray(arr.Wrap());
}

TEST_CASE(TestSortedList)
{
	SortedList<int> list;
	TestCollectionWithIncreasingItems(list.Wrap());
	TestSortedCollection(list.Wrap());
}

TEST_CASE(TestList)
{
	List<int> list;
	TestCollectionWithIncreasingItems(list.Wrap());
	TestNormalList(list.Wrap());
}

TEST_CASE(TestDictionary)
{
	Dictionary<int, int> dictionary;
	TestSortedDictionary(dictionary.Wrap());
}

TEST_CASE(TestGroup)
{
	Group<int, int> group;
	TestSortedGroup(group.Wrap());
}

TEST_CASE(TestListCopy)
{
	Array<int> arr;
	List<int> list;
	SortedList<int> sortedList;

	arr.Resize(5);
	for(int i=0;i<5;i++)
	{
		arr[i]=i;
	}
	CopyFrom(list.Wrap(), arr.Wrap());
	CopyFrom(sortedList.Wrap(), arr.Wrap());
	CHECK_LIST_ITEMS(list.Wrap(), {0 _ 1 _ 2 _ 3 _ 4});
	CHECK_LIST_ITEMS(sortedList.Wrap(), {0 _ 1 _ 2 _ 3 _ 4});

	list.Clear();
	for(int i=10;i<20;i++)
	{
		list.Add(i);
	}
	CopyFrom(arr.Wrap(), list.Wrap());
	CopyFrom(sortedList.Wrap(), list.Wrap());
	CHECK_LIST_ITEMS(arr.Wrap(), {10 _ 11 _ 12 _ 13 _ 14 _ 15 _ 16 _ 17 _ 18 _ 19});
	CHECK_LIST_ITEMS(sortedList.Wrap(), {10 _ 11 _ 12 _ 13 _ 14 _ 15 _ 16 _ 17 _ 18 _ 19});

	sortedList.Clear();
	for(int i=6;i<9;i++)
	{
		sortedList.Add(i);
	}
	CopyFrom(arr.Wrap(), sortedList.Wrap());
	CopyFrom(list.Wrap(), sortedList.Wrap());
	CHECK_LIST_ITEMS(arr.Wrap(), {6 _ 7 _ 8});
	CHECK_LIST_ITEMS(list.Wrap(), {6 _ 7 _ 8});

	arr.Resize(5);
	for(int i=0;i<5;i++)
	{
		arr[i]=i;
	}
	CopyFrom(list.Wrap(), (const IEnumerable<int>&)arr.Wrap());
	CopyFrom(sortedList.Wrap(), (const IEnumerable<int>&)arr.Wrap());
	CHECK_LIST_ITEMS(list.Wrap(), {0 _ 1 _ 2 _ 3 _ 4});
	CHECK_LIST_ITEMS(sortedList.Wrap(), {0 _ 1 _ 2 _ 3 _ 4});

	list.Clear();
	for(int i=10;i<20;i++)
	{
		list.Add(i);
	}
	CopyFrom(arr.Wrap(), (const IEnumerable<int>&)list.Wrap());
	CopyFrom(sortedList.Wrap(), (const IEnumerable<int>&)list.Wrap());
	CHECK_LIST_ITEMS(arr.Wrap(), {10 _ 11 _ 12 _ 13 _ 14 _ 15 _ 16 _ 17 _ 18 _ 19});
	CHECK_LIST_ITEMS(sortedList.Wrap(), {10 _ 11 _ 12 _ 13 _ 14 _ 15 _ 16 _ 17 _ 18 _ 19});

	sortedList.Clear();
	for(int i=6;i<9;i++)
	{
		sortedList.Add(i);
	}
	CopyFrom(arr.Wrap(), (const IEnumerable<int>&)sortedList.Wrap());
	CopyFrom(list.Wrap(), (const IEnumerable<int>&)sortedList.Wrap());
	CHECK_LIST_ITEMS(arr.Wrap(), {6 _ 7 _ 8});
	CHECK_LIST_ITEMS(list.Wrap(), {6 _ 7 _ 8});
}

TEST_CASE(TestDictionaryCopy)
{
	Array<Pair<int, int>> arr;
	List<Pair<int, int>> list;
	SortedList<Pair<int, int>> sortedList;
	Dictionary<int, int> dictionary;
	Group<int, int> group;

	arr.Resize(10);
	for(int i=0;i<10;i++)
	{
		arr[i]=(Pair<int, int>(i/3, i));
	}
	CopyFrom(dictionary.Wrap(), arr.Wrap());
	CopyFrom(group.Wrap(), arr.Wrap());
	CHECK_DICTIONARY_ITEMS(dictionary.Wrap(), {0 _ 1 _ 2 _ 3}, {2 _ 5 _ 8 _ 9});
	CHECK_GROUP_ITEMS(group.Wrap(), {0 _ 1 _ 2 _ 3}, {0 _ 1 _ 2 _ 3 _ 4 _ 5 _ 6 _ 7 _ 8 _ 9}, {3 _ 3 _ 3 _ 1});

	for(int i=0;i<10;i++)
	{
		arr[i]=(Pair<int, int>(i/4, i));
	}
	CopyFrom(dictionary.Wrap(), (const IEnumerable<Pair<int, int>>&)arr.Wrap());
	CopyFrom(group.Wrap(), (const IEnumerable<Pair<int, int>>&)arr.Wrap());
	CHECK_DICTIONARY_ITEMS(dictionary.Wrap(), {0 _ 1 _ 2}, {3 _ 7 _ 9});
	CHECK_GROUP_ITEMS(group.Wrap(), {0 _ 1 _ 2}, {0 _ 1 _ 2 _ 3 _ 4 _ 5 _ 6 _ 7 _ 8 _ 9}, {4 _ 4 _ 2});

	CopyFrom(arr.Wrap(), dictionary.Wrap());
	CopyFrom(list.Wrap(), dictionary.Wrap());
	CopyFrom(sortedList.Wrap(), dictionary.Wrap());
	CompareEnumerable(arr.Wrap(), dictionary.Wrap());
	CompareEnumerable(list.Wrap(), dictionary.Wrap());
	CompareEnumerable(sortedList.Wrap(), dictionary.Wrap());

	CopyFrom(arr.Wrap(), group.Wrap());
	CopyFrom(list.Wrap(), group.Wrap());
	CopyFrom(sortedList.Wrap(), group.Wrap());
	CompareEnumerable(arr.Wrap(), group.Wrap());
	CompareEnumerable(list.Wrap(), group.Wrap());
	CompareEnumerable(sortedList.Wrap(), group.Wrap());
}

int Square(int a)
{
	return a*a;
}

int Double(int a)
{
	return a*2;
}

TEST_CASE(TestSelectOperation)
{
	List<int> src;
	List<int> dst;
	for(int i=1;i<=10;i++)
	{
		src.Add(i);
	}
	CopyFrom(dst.Wrap(), src.Wrap()>>Select(Square)>>Select(Double));
	CHECK_LIST_ITEMS(dst.Wrap(), {2 _ 8 _ 18 _ 32 _ 50 _ 72 _ 98 _ 128 _ 162 _ 200});
	CompareEnumerable(dst.Wrap(), src.Wrap()>>Select(Square)>>Select(Double));
}

bool Odd(int a)
{
	return a%2==1;
}

TEST_CASE(TestWhereOperation)
{
	List<int> src;
	List<int> dst;
	for(int i=1;i<=10;i++)
	{
		src.Add(i);
	}
	CopyFrom(dst.Wrap(), src.Wrap()>>Where(Odd)>>Select(Square));
	CHECK_LIST_ITEMS(dst.Wrap(), {1 _ 9 _ 25 _ 49 _ 81});
	CopyFrom(dst.Wrap(), src.Wrap()>>Where(Odd));
	CHECK_LIST_ITEMS(dst.Wrap(), {1 _ 3 _ 5 _ 7 _ 9});
	CompareEnumerable(dst.Wrap(), src.Wrap()>>Where(Odd));
}

int Add(int a, int b)
{
	return a+b;
}

bool And(bool a, bool b)
{
	return a && b;
}

bool Or(bool a, bool b)
{
	return a || b;
}

TEST_CASE(TestAggregateOperation)
{
	List<int> src;
	for(int i=1;i<=10;i++)
	{
		src.Add(i);
	}
	TEST_ASSERT(src.Wrap()>>Aggregate(Add)==55);
	TEST_ASSERT(src.Wrap()>>All(Odd)==false);
	TEST_ASSERT(src.Wrap()>>Any(Odd)==true);
	TEST_ASSERT(src.Wrap()>>Max()==10);
	TEST_ASSERT(src.Wrap()>>Min()==1);
}

TEST_CASE(TestConcatOperation)
{
	List<int> first;
	List<int> second;
	List<int> result;
	for(int i=0;i<5;i++)
	{
		first.Add(i);
	}
	for(int i=5;i<10;i++)
	{
		second.Add(i);
	}
	CopyFrom(result.Wrap(), first.Wrap()>>Concat(second.Wrap()));
	CHECK_LIST_ITEMS(result.Wrap(), {0 _ 1 _ 2 _ 3 _ 4 _ 5 _ 6 _ 7 _ 8 _ 9});
	CompareEnumerable(result.Wrap(), first.Wrap()>>Concat(second.Wrap()));
}

TEST_CASE(TestSequenceOperation)
{
	List<int> src;
	List<int> dst;
	for(int i=0;i<10;i++)
	{
		src.Add(i);
	}

	CopyFrom(dst.Wrap(), src.Wrap()>>Take(5));
	CHECK_LIST_ITEMS(dst.Wrap(), {0 _ 1 _ 2 _ 3 _ 4});
	CompareEnumerable(dst.Wrap(), src.Wrap()>>Take(5));
	CopyFrom(dst.Wrap(), src.Wrap()>>Take(15));
	CHECK_LIST_ITEMS(dst.Wrap(), {0 _ 1 _ 2 _ 3 _ 4 _ 5 _ 6 _ 7 _ 8 _ 9});
	CompareEnumerable(dst.Wrap(), src.Wrap()>>Take(15));

	CopyFrom(dst.Wrap(), src.Wrap()>>Skip(5));
	CHECK_LIST_ITEMS(dst.Wrap(), {5 _ 6 _ 7 _ 8 _ 9});
	CompareEnumerable(dst.Wrap(), src.Wrap()>>Skip(5));
	CopyFrom(dst.Wrap(), src.Wrap()>>Skip(15));
	CHECK_EMPTY_LIST(dst.Wrap());
	CompareEnumerable(dst.Wrap(), src.Wrap()>>Skip(15));

	src.Clear();
	for(int i=0;i<3;i++)
	{
		src.Add(i);
	}
	CopyFrom(dst.Wrap(), src.Wrap()>>Repeat(0));
	CHECK_EMPTY_LIST(dst.Wrap());
	CompareEnumerable(dst.Wrap(), src.Wrap()>>Repeat(0));
	CopyFrom(dst.Wrap(), src.Wrap()>>Repeat(1));
	CHECK_LIST_ITEMS(dst.Wrap(), {0 _ 1 _ 2});
	CompareEnumerable(dst.Wrap(), src.Wrap()>>Repeat(1));
	CopyFrom(dst.Wrap(), src.Wrap()>>Repeat(2));
	CHECK_LIST_ITEMS(dst.Wrap(), {0 _ 1 _ 2 _ 0 _ 1 _ 2});
	CompareEnumerable(dst.Wrap(), src.Wrap()>>Repeat(2));

	src.Clear();
	CopyFrom(dst.Wrap(), src.Wrap()>>Repeat(0));
	CHECK_EMPTY_LIST(dst.Wrap());
	CompareEnumerable(dst.Wrap(), src.Wrap()>>Repeat(0));
	CopyFrom(dst.Wrap(), src.Wrap()>>Repeat(1));
	CHECK_EMPTY_LIST(dst.Wrap());
	CompareEnumerable(dst.Wrap(), src.Wrap()>>Repeat(1));
	CopyFrom(dst.Wrap(), src.Wrap()>>Repeat(2));
	CHECK_EMPTY_LIST(dst.Wrap());
	CompareEnumerable(dst.Wrap(), src.Wrap()>>Repeat(2));
}

TEST_CASE(TestDistinctOperation)
{
	List<int> first;
	List<int> second;
	List<int> result;
	for(int i=0;i<8;i++)
	{
		first.Add(i);
	}
	for(int i=2;i<10;i++)
	{
		second.Add(i);
	}
	CopyFrom(result.Wrap(), first.Wrap()>>Concat(second.Wrap())>>Distinct());
	CHECK_LIST_ITEMS(result.Wrap(), {0 _ 1 _ 2 _ 3 _ 4 _ 5 _ 6 _ 7 _ 8 _ 9});
	CompareEnumerable(result.Wrap(), first.Wrap()>>Concat(second.Wrap())>>Distinct());
	CopyFrom(result.Wrap(), first.Wrap()>>Concat(second.Wrap())>>Distinct()>>Reverse());
	CHECK_LIST_ITEMS(result.Wrap(), {9 _ 8 _ 7 _ 6 _ 5 _ 4 _ 3 _ 2 _ 1 _ 0});
	CompareEnumerable(result.Wrap(), first.Wrap()>>Concat(second.Wrap())>>Distinct()>>Reverse());
}

TEST_CASE(TestSetOperation)
{
	List<int> first;
	List<int> second;
	List<int> result;
	for(int i=0;i<8;i++)
	{
		first.Add(i);
	}
	for(int i=2;i<10;i++)
	{
		second.Add(i);
	}
	CopyFrom(result.Wrap(), first.Wrap()>>Intersect(second.Wrap()));
	CHECK_LIST_ITEMS(result.Wrap(), {2 _ 3 _ 4 _ 5 _ 6 _ 7});
	CompareEnumerable(result.Wrap(), first.Wrap()>>Intersect(second.Wrap()));
	CopyFrom(result.Wrap(), first.Wrap()>>Except(second.Wrap()));
	CHECK_LIST_ITEMS(result.Wrap(), {0 _ 1});
	CompareEnumerable(result.Wrap(), first.Wrap()>>Except(second.Wrap()));
	CopyFrom(result.Wrap(), first.Wrap()>>Union(second.Wrap()));
	CHECK_LIST_ITEMS(result.Wrap(), {0 _ 1 _ 2 _ 3 _ 4 _ 5 _ 6 _ 7 _ 8 _ 9});
	CompareEnumerable(result.Wrap(), first.Wrap()>>Union(second.Wrap()));
}

TEST_CASE(TestPairOperation)
{
	List<int> src;
	Group<bool, int> dst;
	List<Pair<bool, int>> pair;
	for(int i=1;i<=10;i++)
	{
		src.Add(i);
	}
	CopyFrom(dst.Wrap(), src.Wrap()>>Select(Odd)>>Pairwise(src.Wrap()>>Select(Square)));
	TEST_ASSERT(dst.Count()==2);
	TEST_ASSERT(dst.Keys()[0]==false);
	TEST_ASSERT(dst.Keys()[1]==true);
	CHECK_LIST_ITEMS(dst.Get(true), {1 _ 9 _ 25 _ 49 _ 81});
	CHECK_LIST_ITEMS(dst.Get(false), {4 _ 16 _ 36 _ 64 _ 100});
	CopyFrom(pair.Wrap(), src.Wrap()>>Select(Odd)>>Pairwise(src.Wrap()>>Select(Square)));
	CompareEnumerable(pair.Wrap(), src.Wrap()>>Select(Odd)>>Pairwise(src.Wrap()>>Select(Square)));
}

bool dividable(int a, int b)
{
	return b%a==0;
}

Func<bool(int)> dividableConverter(int a)
{
	return Curry(dividable)(a);
}

TEST_CASE(TestFunctionCollection)
{
	int divider[]={2,3,5};
	Func<bool(int)> selector=
		Array<int>(divider, 3).Wrap()
		>>Select(dividableConverter)
		>>Aggregate(Combiner<bool(int)>(And));

	List<int> src;
	List<int> dst;
	for(int i=1;i<=100;i++)
	{
		src.Add(i);
	}
	CopyFrom(dst.Wrap(), src.Wrap()>>Where(selector));
	CHECK_LIST_ITEMS(dst.Wrap(), {30 _ 60 _ 90});
}

void TestABCDE(const IReadonlyList<wchar_t>& list, int repeat)
{
	TEST_ASSERT(list.Count()==5*repeat);
	for(int i=0;i<5*repeat;i++)
	{
		TEST_ASSERT(list[i]==L'a'+i%5);
	}
}

TEST_CASE(TestCopyFromString)
{
	WString string;
	List<wchar_t> list;
	Array<wchar_t> arr;

	CopyFrom(list.Wrap(), WString(L"abcde"), true);
	TestABCDE(list.Wrap(), 1);
	CopyFrom(list.Wrap(), WString(L"abcde"), false);
	TestABCDE(list.Wrap(), 1);
	CopyFrom(list.Wrap(), WString(L"abcde"), true);
	TestABCDE(list.Wrap(), 2);
	CopyFrom(list.Wrap(), WString(L"abcde"), false);
	TestABCDE(list.Wrap(), 1);

	CopyFrom(arr.Wrap(), WString(L"abcde"), true);
	TestABCDE(arr.Wrap(), 1);
	CopyFrom(arr.Wrap(), WString(L"abcde"), false);
	TestABCDE(arr.Wrap(), 1);
	CopyFrom(arr.Wrap(), WString(L"abcde"), true);
	TestABCDE(arr.Wrap(), 2);
	CopyFrom(arr.Wrap(), WString(L"abcde"), false);
	TestABCDE(arr.Wrap(), 1);

	string=L"";
	CopyFrom(string, list.Wrap(), true);
	TEST_ASSERT(string==L"abcde");
	CopyFrom(string, list.Wrap(), false);
	TEST_ASSERT(string==L"abcde");
	CopyFrom(string, list.Wrap(), true);
	TEST_ASSERT(string==L"abcdeabcde");
	CopyFrom(string, list.Wrap(), false);
	TEST_ASSERT(string==L"abcde");

	string=L"";
	CopyFrom(string, arr.Wrap(), true);
	TEST_ASSERT(string==L"abcde");
	CopyFrom(string, arr.Wrap(), false);
	TEST_ASSERT(string==L"abcde");
	CopyFrom(string, arr.Wrap(), true);
	TEST_ASSERT(string==L"abcdeabcde");
	CopyFrom(string, arr.Wrap(), false);
	TEST_ASSERT(string==L"abcde");

	string=L"";
	CopyFrom(string,(IEnumerable<wchar_t>&)list.Wrap(), true);
	TEST_ASSERT(string==L"abcde");
	CopyFrom(string, (IEnumerable<wchar_t>&)list.Wrap(), false);
	TEST_ASSERT(string==L"abcde");
	CopyFrom(string, (IEnumerable<wchar_t>&)list.Wrap(), true);
	TEST_ASSERT(string==L"abcdeabcde");
	CopyFrom(string, (IEnumerable<wchar_t>&)list.Wrap(), false);
	TEST_ASSERT(string==L"abcde");

	string=L"";
	CopyFrom(string, (IEnumerable<wchar_t>&)arr.Wrap(), true);
	TEST_ASSERT(string==L"abcde");
	CopyFrom(string, (IEnumerable<wchar_t>&)arr.Wrap(), false);
	TEST_ASSERT(string==L"abcde");
	CopyFrom(string, (IEnumerable<wchar_t>&)arr.Wrap(), true);
	TEST_ASSERT(string==L"abcdeabcde");
	CopyFrom(string, (IEnumerable<wchar_t>&)arr.Wrap(), false);
	TEST_ASSERT(string==L"abcde");
}

TEST_CASE(TestForEach)
{
	List<int> a;
	List<int> b;
	List<int> c;
	for(int i=0;i<3;i++)
	{
		a.Add(i);
		b.Add(i);
	}

	FOREACH(int, i, a.Wrap())
		FOREACH(int, j, b.Wrap())
		{
			c.Add(i+j);
		}
	CHECK_LIST_ITEMS(c.Wrap(), {0 _ 1 _ 2 _ 1 _ 2 _ 3 _ 2 _ 3 _ 4});
}

TEST_CASE(TestForEachWithIndexer)
{
	List<int> a;
	for(int i=0;i<10;i++)
	{
		a.Add(i*i);
	}
	Dictionary<int, int> b;

	FOREACH_INDEXER(int, num, i, a.Wrap())
	{
		b.Add(i, num);
	}
	CHECK_DICTIONARY_ITEMS(b.Wrap(), {0 _ 1 _ 2 _ 3 _ 4 _ 5 _ 6 _ 7 _ 8 _ 9}, {0 _ 1 _ 4 _ 9 _ 16 _ 25 _ 36 _ 49 _ 64 _ 81});
}