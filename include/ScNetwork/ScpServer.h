#pragma once

class CScpServer
{
public:
	CScpServer(void);
	virtual ~CScpServer(void);

	virtual int	 Open();
	virtual void Close();
	
protected:
	
};
