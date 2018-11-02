#ifndef _NER_STL_PRIORITY_QUEUE_H_
#define _NER_STL_PRIORITY_QUEUE_H_

#include "NerHeap.h"
#include "NerVector.h"
#include "Functional.h"
#include "Allocator.h"

namespace NER_STL
{
template <class T, class Sequence = vector<T>, class Compare = less<typename Sequence::value_type>>
class priority_queue
{
  public:
	typedef typename Sequence::value_type value_type;
	typedef typename Sequence::size_type size_type;
	typedef typename Sequence::reference reference;
	typedef typename Sequence::const_reference const_reference;

  private:
	Sequence seq;
	Compare comp;

  public:
	//Constructor
	priority_queue() : seq() {}
	priority_queue(const Compare &c) : seq(), comp(c) {}
	template <class InputIterator>
	priority_queue(InputIterator first, InputIterator last, const Compare &c) : seq(first, last), comp(c)
	{
		make_heap(seq.begin(), seq.end(), comp);
	}
	template <class InputIterator>
	priority_queue(InputIterator first, InputIterator last) : seq(first, last)
	{
		make_heap(seq.begin(), seq.end(), comp);
	}

	bool empty() const { return seq.empty(); }
	size_type size() const { return seq.size(); }
	reference top() { return seq.front(); }
	const_reference top() const { return seq.front(); }
	void push(const value_type &x)
	{
		seq.push_back(x);
		NER_STL::push_heap(seq.begin(), seq.end(), comp);
	}
	void pop()
	{
		NER_STL::pop_heap(seq.begin(), seq.end(), comp);
		seq.pop_back();
	}

	void swap(priority_queue &x)
	{
		seq.swap(x.seq);
	}
};
}
#endif