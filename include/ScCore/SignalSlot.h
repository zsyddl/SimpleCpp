#pragma once

#ifndef SignalSlot_H
#define SignalSlot_H
#ifdef OS_LINUX
#define STHREAD_API
#endif

#include <vector>
#include "Windows.h"
using namespace std;

namespace SThread{

	template<class TypeBase>
	class CSlotBase
	{
		public:
			virtual void Exec(TypeBase para) = 0;
	};

	template<class TypeImpl, class TypeBase>
	class CSlotImpl:public CSlotBase<TypeBase>
	{
		public:
			CSlotImpl(TypeImpl *pObj, void (TypeImpl::*func)(TypeBase para))
			{
				m_pObj = pObj;
				m_pFunc = func;
			}

			void Exec(TypeBase para)
			{
				(m_pObj->*m_pFunc)(para);
			}

		private:
			TypeImpl *m_pObj;
			void (TypeImpl::*m_pFunc)(TypeBase para);
	};

	template<class TypeBase>
	class Slot
	{
		public:
			template<class TypeImpl>
			Slot(TypeImpl *pObj, void (TypeImpl::*pFunc)(TypeBase para))
			{
				m_pSoltBase = new CSlotImpl<TypeImpl, TypeBase>(pObj, pFunc);
			};

			~Slot()
			{
				if (NULL != m_pSoltBase)
				{
					delete m_pSoltBase;
					m_pSoltBase = NULL;
				}
			}

			void Exec(TypeBase para)
			{
				m_pSoltBase->Exec(para);
			}

		private:
			CSlotBase<TypeBase> *m_pSoltBase;
	};

	template<class TypeBase>
	class Signal
	{
		public:
			template<class TypeImpl>
			void Bind(TypeImpl *pObj, void (TypeImpl::*pFunc)(TypeBase para))
			{
				m_SlotSet.push_back(new Slot<TypeBase>(pObj, pFunc));
			}

			~Signal()
			{
				int nSize = m_SlotSet.size();

				for (int i = 0; i < nSize; i++)
				{
					delete m_SlotSet[i];
					m_SlotSet[i] = NULL;
				}
			}

			void operator()(TypeBase para)
			{
				int nSize = m_SlotSet.size();

				for (int i = 0; i < nSize; i++)
				{
					m_SlotSet[i]->Exec(para);
				}
			}

		private:
			vector<Slot<TypeBase> *> m_SlotSet;
	};

	#define Connect(sender, signal, receiver, method) ((sender)->signal.Bind(receiver, method))
}

#endif
