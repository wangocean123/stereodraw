#pragma once

#if _MSC_VER>=1800
#include <mutex>
#endif

namespace gis
{
	class readwrite_lock
	{
	public:
		readwrite_lock();
		~readwrite_lock();

		void readLock();
		void readUnlock();
		bool tryReadLock();

		void writeLock();
		void writeUnlock();

	private:
#if _MSC_VER>=1800
		std::mutex read_mtx;
		std::mutex write_mtx;
#else
		CRITICAL_SECTION read_cs;
		CRITICAL_SECTION write_cs;
#endif
		int read_cnt; // 已加读锁个数  
	};
}