#pragma once
#include "stdafx.h"
#include "readwrite_lock.h"

namespace gis
{
	readwrite_lock::readwrite_lock() : read_cnt(0)
	{
#if _MSC_VER>=1800
#else
		InitializeCriticalSection(&read_cs);
		InitializeCriticalSection(&write_cs);
#endif
	}

	readwrite_lock::~readwrite_lock()
	{

	}

	void readwrite_lock::readLock()
	{
#if _MSC_VER>=1800
		read_mtx.lock();
		if (++read_cnt == 1)
			write_mtx.lock();

		read_mtx.unlock();
#else
		EnterCriticalSection(&read_cs);
		if (++read_cnt == 1)
			EnterCriticalSection(&write_cs);

		LeaveCriticalSection(&read_cs);
#endif
	}

	bool readwrite_lock::tryReadLock()
	{
#if _MSC_VER>=1800
		read_mtx.lock();
		if (++read_cnt == 1)
		{
			if (write_mtx.try_lock())
			{
				read_mtx.unlock();
				return true;
			}
			else
			{
				read_cnt--;
				read_mtx.unlock();
				return false;
			}
		}
		read_mtx.unlock();
		return true;

#else
		EnterCriticalSection(&read_cs);
		if (++read_cnt == 1)
		{
			if(TryEnterCriticalSection(&write_cs))
			{
				LeaveCriticalSection(&read_cs);
				return true;
			}
			else
			{
				read_cnt--;
				LeaveCriticalSection(&read_cs);
				return false;
			}
		}

		LeaveCriticalSection(&read_cs);
#endif
	}

	void readwrite_lock::readUnlock()
	{
#if _MSC_VER>=1800
		read_mtx.lock();
		if (--read_cnt == 0)
			write_mtx.unlock();

		read_mtx.unlock();
#else
		EnterCriticalSection(&read_cs);
		if (--read_cnt == 0)
			EnterCriticalSection(&write_cs);

		LeaveCriticalSection(&read_cs);
#endif
	}

	void readwrite_lock::writeLock()
	{
#if _MSC_VER>=1800
		write_mtx.lock();
#else
		EnterCriticalSection(&write_cs);
#endif
	}

	void readwrite_lock::writeUnlock()
	{
#if _MSC_VER>=1800
		write_mtx.unlock();
#else
		LeaveCriticalSection(&write_cs);
#endif
	}
}