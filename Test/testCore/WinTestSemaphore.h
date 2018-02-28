#pragma once
#ifndef WINTESTSEMAPHORE_H
#define WINTESTSEMAPHORE_H

namespace SimpleCpp
{
	class CMutex;
	class CThread;
	class CSemaphore;
};

using namespace SimpleCpp;

class TestSemaphore
{
	public:
		TestSemaphore();
		~TestSemaphore();

		int Open();
		int Test();
		int Close();

	private:
		static int __stdcall ProducerThread(void *pObj);
		static int __stdcall ConsumerThread(void *pObj);

		CSemaphore *m_pSemaphore;
		CThread *m_pThreadProducer;
		CThread *m_pThreadConsumer;

		bool m_bProducerExitFlag;
		bool m_bConsumerExitFlag;
		CMutex *m_pProducerMutex;
		CMutex *m_pConsumerMutex;
		int m_nCounter;
};

#endif