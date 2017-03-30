#include "stdafx.h"


void TestHostMemAllocator()
{
	struct TestUnit {
		void *pPointer;
		int size;
		int beginLifeFrame;
		int endLifeFrame;
	};

	const int count = 5000;
	const int frames = 1000;
	static TestUnit units[count];

	FILE *pFile = fopen("TestHostMemAllocator.log", "wb");

	for (int indexSize = 4; indexSize < 64 * 1024; indexSize += 4) {
		DWORD dwBegin, dwEnd;
		DWORD dwTime0, dwTime1;
		srand((DWORD)time(NULL));


		for (int index = 0; index < count; index++) {
			units[index].pPointer = NULL;
			units[index].size = (rand() % 8 - 4) + indexSize;
			units[index].size = max(units[index].size, 1);
			units[index].beginLifeFrame = (rand() % (frames / 2));
			units[index].endLifeFrame = units[index].beginLifeFrame + (rand() % (frames / 2));
		}


		dwBegin = tick();
		for (int indexFrame = 0; indexFrame < frames; indexFrame++) {
			for (int index = 0; index < count; index++) {
				if (units[index].beginLifeFrame == indexFrame) {
					units[index].pPointer = malloc(units[index].size);
				}
				if (units[index].endLifeFrame == indexFrame) {
					free(units[index].pPointer);
					units[index].pPointer = NULL;
				}
			}
		}
		dwEnd = tick();
		dwTime0 = (dwEnd - dwBegin) / 1000;


		dwBegin = tick();
		BOOL bCheck = FALSE;
		for (int indexFrame = 0; indexFrame < frames; indexFrame++) {
			for (int index = 0; index < count; index++) {
				if (units[index].beginLifeFrame == indexFrame) {
					units[index].pPointer = SAFE_ALLOC(units[index].size, CrossEngine::MEMTYPE_HEAP);
				}

				if (units[index].endLifeFrame == indexFrame) {
					SAFE_FREE(units[index].pPointer);
					units[index].pPointer = NULL;
				}
			}
		}
		dwEnd = tick();
		dwTime1 = (dwEnd - dwBegin) / 1000;

		fprintf(pFile, "%d\t%d\t%d\n", indexSize, dwTime0, dwTime1);
	}

	fclose(pFile);
}
