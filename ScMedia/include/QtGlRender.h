#pragma once
#ifndef QT_GL_RENDER_H
#define QT_GL_RENDER_H
#include "Render.h"
#include "MediaSdkDef.h"

#ifdef QT_OPENGL
#include <gl/glew.h>
#include <QGLWidget>
#include "glwindow.h"

#pragma  comment(lib, "QtGuid4.lib");
#pragma comment(lib, "QtOpenGLd4.lib");
#pragma comment(lib, "QtCored4.lib");
#pragma comment(lib, "opengl32.lib");
#pragma comment(lib, "glu32.lib");

class QWidget;
class QGLWidget;
//qt 的opengl render
class CQtGlRender :
	public CRender
{
public:
	CQtGlRender(void);
	virtual ~CQtGlRender(void);
	
	int Open(HWND pParent, int nPlayBuffer=5);
	
	virtual int DoFilter();
protected:
	CGlWindow *m_pGlWidget;

	//上一帧播放的时间点, ms
	long m_nTimeLastFrame;
	//平均每帧需要的时间, ms
	long m_nTimePerFrame;
	//时间统计
	long m_nTimeStatBegin;
	long m_nFrameCount;
};
#endif

#endif
