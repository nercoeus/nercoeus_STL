#ifndef _NER_STL_ALLOC_H_
#define _NER_STL_ALLOC_H_

#include <cstdlib>

namespace NER_STL
{
class alloc
{
  private:
	enum
	{
		_ALIGN = 8
	};
	enum
	{
		_MAX_BYTES = 128
	};
	enum
	{
		_NFREELISTS = _MAX_BYTES / _ALIGN
	};

  private:
	//freelist
	union obj {
		union obj *next_free_list;
		char client_data[1];
	};
	static obj *free_list[_NFREELISTS];

  private:
	//内存池分配情况
	static char *start_free;
	static char *end_free;
	static size_t heap_size;

  private:
	//上调至8的倍数
	static size_t ROUND_UP(size_t bytes)
	{
		return ((bytes + _ALIGN - 1) & ~(_ALIGN - 1));
	}
	//查找freelist
	static size_t FREELIST_INDEX(size_t bytes)
	{
		return ((bytes + _ALIGN - 1) / _ALIGN - 1);
	}
	//为freelist分配空间
	static void *refill(size_t n);
	//从内存池中获取可用空间
	static char *chunk_alloc(size_t size, int &nobjs);

  public:
	static void *allocate(size_t n);
	static void deallocate(void *p, size_t n);
	static void *reallocate(void *p, size_t old_sz, size_t new_sz);
};

}
#endif
