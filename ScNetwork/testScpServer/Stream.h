#pragma once

class CStream
{
public:
	CStream(void);
	virtual ~CStream(void);

	virtual int Open();
	virtual int Close();
	virtual int Write();
};
