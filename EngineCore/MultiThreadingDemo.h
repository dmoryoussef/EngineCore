#include <intrin.h>
struct work_queue_entry
{
	string str;
	work_queue_entry(string s)
	{
		str = s;
	}
};

vector<work_queue_entry> Entries;

struct win32_thread_info
{
	int ThreadIndex;
	int processedJobs = 0;
	bool idle;
	HANDLE SemaphoreHandle;
};

int volatile totalEntries = 0;
int volatile currentEntry = 0;

DWORD WINAPI ThreadProc(LPVOID lpParam)
{
	win32_thread_info* thread = (win32_thread_info*)lpParam;
	for (;;)
	{
		if (currentEntry < totalEntries)
		{
			string s = Entries[currentEntry].str;
			_ReadBarrier();
			InterlockedIncrement((LONG volatile*)& currentEntry);

			//char Buffer[256];
			//wsprintf(Buffer, "Thread %u: %s \n", thread->ThreadIndex, s.c_str());
			//OutputDebugString(Buffer);

			thread->processedJobs++;
		}
		else
		{
			thread->idle = true;
			WaitForSingleObjectEx(thread->SemaphoreHandle, INFINITE, FALSE);
		}
	}
}

class Thread
{
private:
	HANDLE thread;
	win32_thread_info info;

public:
	Thread() {}

	void start(int i, HANDLE hSemaphore)
	{
		info.ThreadIndex = i;
		info.SemaphoreHandle = hSemaphore;

		DWORD ThreadID;
		thread = CreateThread(0, 0,
			ThreadProc, &info,
			0,
			&ThreadID);

	}

	win32_thread_info Info()
	{
		return info;
	}

	int id()
	{
		return info.ThreadIndex;
	}
};

class MultiThreadingDemo : public GameState
{
private:
	Thread threads[11];

public:
	MultiThreadingDemo() {}

	void addString(string s, HANDLE semaphore)
	{
		Entries.push_back(work_queue_entry(s));
		_WriteBarrier();
		_mm_sfence();
		totalEntries++;

		ReleaseSemaphore(semaphore, 1, 0);
	}

	void start(BaseNode* pData, BaseNode* pSystems, BaseNode* pGUI)
	{
		int InitialCount = 0;
		int ThreadCount = 11;
		HANDLE SemaphoreHandle = CreateSemaphoreEx(0,
			InitialCount,
			ThreadCount,
			0, 0, SEMAPHORE_ALL_ACCESS);

		//	start up threads
		for (int i = 0; i < 11; i++)
		{
			threads[i].start(i, SemaphoreHandle);
		}
		
		//	fill work queue
		for (int i = 0; i < 20000; i++)
		{
			string n = thingToString<int>(i);
			addString("Work " + n, SemaphoreHandle);
		}
	}

	void render(OutputBuffer* pEngineBuffer)
	{
		Render2D r(pEngineBuffer);
		int totalJobsProcessed = 0;
		for (int i = 0; i < 11; i++)
		{
			float posX = 5;
			float posY = 5 * i;
			float sizeX = 25;
			float sizeY = 6;
			r.DrawQuad(posX, posY + 1, sizeX + posX, sizeY + posY, { PIXEL_SOLID, FG_WHITE });
			r.DrawNum<int>(threads[i].id(), posX+5, posY + 4);
			r.DrawNum<int>(threads[i].Info().processedJobs, posX + 10, posY + 4);
			totalJobsProcessed = threads[i].Info().processedJobs + totalJobsProcessed;
			
			if (threads[i].Info().idle)
				r.DrawString("idle", posX + 18, posY + 4);
			else
				r.DrawString("running", posX + 18, posY + 4);
		}
		r.DrawNum<int>(totalJobsProcessed, 8, 0);
	}
};
