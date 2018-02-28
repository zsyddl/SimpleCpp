#ifndef ScNetwork_H
#define ScNetwork_H




namespace SimpleCpp{

	class CScNetwork
	{
		CScNetwork();
		~CScNetwork();

		static int Init();
		static int Free();
		static CScNetwork m_instance;
	};


}


#endif