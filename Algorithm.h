#ifndef _NER_STL_ALGORITHM_H_
#define _NER_STL_ALGORITHM_H_

#include <string.h>
#include "Iterator.h"
#include "TypeTraits.h"

namespace NER_STL
{
template <typename T>
inline const T &max(const T &a, const T &b)
{
	return a > b ? a : b;
}

template <typename T>
inline const T &min(const T &a, const T &b)
{
	return a > b ? b : a;
}
//swap函数*************************************************************
template <typename T>
inline void swap(T &a, T &b)
{
	T temp = a;
	a = b;
	b = temp;
}

template <typename ForwardIterator, typename T>
void fill(ForwardIterator first, ForwardIterator last, const T &value)
{
	for (; first != last; ++first)
		*first = value;
}

template <typename OutputIterator, typename Size, typename T>
OutputIterator fill_n(OutputIterator first, Size n, const T &value)
{
	for (; n > 0; --n, ++first)
		*first = value;
	return first;
}
//copy函数****************************************************************
template <typename InputIterator, typename OutputIterator>
inline OutputIterator copy(InputIterator first, InputIterator last, OutputIterator dest)
{
	return _copy_dispatch<InputIterator, OutputIterator>()(first, last, dest);
}

inline char *copy(const char *first, const char *last, char *dest)
{
	memmove(dest, first, last - first);
	return dest + (last - first);
}

inline wchar_t *copy(const wchar_t *first, const wchar_t *last, wchar_t *dest)
{
	memmove(dest, first, sizeof(wchar_t) * (last - first));
	return dest + (last - first);
}

//_copy_dispath偏特化版本
template <typename InputIterator, typename OutputIterator>
struct _copy_dispatch
{
	OutputIterator operator()(InputIterator first, InputIterator last, OutputIterator dest)
	{
		return _copy(first, last, dest, iterator_category(first));
	}
};

template <typename T>
struct _copy_dispatch<T *, T *>
{
	T *operator()(T *first, T *last, T *dest)
	{
		typedef typename _type_traits<T>::has_trivial_assignment_operator t;
		return _copy_t(first, last, dest, t());
	}
};

template <typename T>
struct _copy_dispatch<const T *, T *>
{
	T *operator()(const T *first, const T *last, T *dest)
	{
		typedef typename _type_traits<T>::has_trivial_assignment_operator t;
		return _copy_t(first, last, dest, t());
	}
};

template <typename InputIterator, typename OutputIterator>
inline OutputIterator _copy(InputIterator first, InputIterator last, OutputIterator dest, input_iterator_tag)
{
	for (; first != last; ++first, ++dest) //通过比较迭代器实现，较慢
		*dest = *first;
	return dest;
}

template <typename RandomAccessIterator, typename OutputIterator>
inline OutputIterator _copy(RandomAccessIterator first, RandomAccessIterator last, OutputIterator dest, random_access_iterator_tag)
{
	return _copy_d(first, last, dest, difference_type(first));
}

//RandomAccessIterator进行复制，通过比较Distance所以速度比较快
template <typename RandomAccessIterator, typename OutputIterator, typename Distance>
inline OutputIterator _copy_d(RandomAccessIterator first, RandomAccessIterator last, OutputIterator dest, Distance *)
{
	for (Distance n = last - first; n > 0; --n, ++dest, ++first)
		*dest = *first;
	return dest;
}

//T*和const T*特化版本所用到的_copy_t
template <typename T>
inline T *_copy_t(const T *first, const T *last, T *dest, _true_type)
{
	memmove(dest, first, sizeof(T) * (last - first));
	return dest + (last - first);
}

template <typename T>
inline T *_copy_t(const T *first, const T *last, T *dest, _false_type)
{
	return _copy_d(first, last, dest, (ptrdiff_t *)0);
}

template <typename BidirectionalIterator, typename Distance>
inline BidirectionalIterator _copy_backward(BidirectionalIterator first,
											BidirectionalIterator last,
											BidirectionalIterator dest,
											bidirectional_iterator_tag,
											Distance *)
{
	while (first != last)
		*(--dest) = *(--last);
	return dest;
}

template <typename RandomAccessIterator, typename BidirectionalIterator, typename Distance>
inline BidirectionalIterator _copy_backward(RandomAccessIterator first,
											RandomAccessIterator last,
											BidirectionalIterator dest,
											random_access_iterator_tag,
											Distance *)
{
	for (Distance n = last - first; n > 0; --n)
		*(--dest) = *(--last);
	return dest;
}

template <typename BidirectionalIterator1, typename BidirectionalIterator2>
inline BidirectionalIterator2 copy_backward(BidirectionalIterator1 first, BidirectionalIterator1 last, BidirectionalIterator2 dest)
{
	return _copy_backward(first, last, dest, iterator_category(first), difference_type(first));
}

template <typename InputIterator1, typename InputIterator2>
inline bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2)
{
	while (first1 != last1)
	{
		if (*first1 != *first2)
		{
			return false;
		}
		++first1;
		++first2;
	}
	return true;
}

template <typename InputIterator1, typename InputIterator2, typename BinaryPredicate>
inline bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, BinaryPredicate binary_pred)
{
	while (first1 != last1)
	{
		if (!binary_pred(*first1, *first2))
		{
			return false;
		}
		++first1;
		++first2;
	}
	return true;
}

template <typename InputIterator1, typename InputIterator2>
bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1,
							 InputIterator2 first2, InputIterator2 last2)
{
	while (first1 != last1 && first2 != last2)
	{
		if (*first1 < *first2)
		{
			return true;
		}
		if (*first1 > *first2)
		{
			return false;
		}
	}
	return first1 == last1 && first2 != last2;
}

template <typename InputIterator1, typename InputIterator2, typename Compare>
bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1,
							 InputIterator2 first2, InputIterator2 last2,
							 Compare comp)
{
	while (first1 != last1 && first2 != last2)
	{
		if (comp(*first1, *first2))
		{
			return true;
		}
		if (comp(*first2, *first1))
		{
			return false;
		}
	}
	return first1 == last1 && first2 != last2;
}

template <typename InputIterator, typename T>
T accumulate(InputIterator first, InputIterator last, T init)
{
	while (first != last)
	{
		init = init + *first;
		++first;
	}
	return init;
}

template <typename InputIterator, typename T, typename BinaryOperation>
T accumulate(InputIterator first, InputIterator last, T init, BinaryOperation op)
{
	while (first != last)
	{
		init = op(init, *first);
		++first;
	}
	return init;
}

template <typename InputIterator, typename T>
InputIterator find(InputIterator first, InputIterator last, const T &value)
{
	while (first != last && *first != value)
	{
		++first;
	}
	return first;
}

template <typename ForwardIterator, typename T>
inline ForwardIterator lower_bound(ForwardIterator first, ForwardIterator last, const T &value)
{
	return _lower_bound(first, last, value, difference_type(first), iterator_category(first));
}

template <typename ForwardIterator, typename T, typename Distance>
inline ForwardIterator _lower_bound(ForwardIterator first, ForwardIterator last, const T &value, Distance *, forward_iterator_tag)
{
	Distance len = distance(first, last);
	Distance half;
	ForwardIterator mid;
	while (len > 0)
	{
		half = len >> 1;
		mid = first;
		advance(mid, half);
		if (*mid < value)
		{
			first = mid;
			++first;
			len = len - half - 1;
		}
		else
			len = half;
	}
	return first;
}

template <typename ForwardIterator, typename T, typename Distance>
inline ForwardIterator _lower_bound(ForwardIterator first, ForwardIterator last, const T &value, Distance *, random_access_iterator_tag)
{
	Distance len = distance(first, last);
	Distance half;
	ForwardIterator mid;
	while (len > 0)
	{
		half = len >> 1;
		mid = first + half;
		if (*mid < value)
		{
			first = mid + 1;
			len = len - half - 1;
		}
		else
			len = half;
	}
	return first;
}

} // namespace NER_STL

#endif