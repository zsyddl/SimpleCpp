#ifndef GLWINDOW_H
#define GLWINDOW_H

#include "MediaSdkDef.h"
#ifdef QT_OPENGL
#include <gl/glew.h>
#include <QGLWidget>


class QMutex;
class CGlWindow : public QGLWidget
{
	//Q_OBJECT

public:
	CGlWindow(QWidget *parent);
	~CGlWindow();

	int Create(int nW, int nH);
	void Refresh();
	void SetPicture(uchar *pData, int nW, int nH);
	void *GetSurface(){return (void*)m_glnTexture;}
protected:
	void initializeGL();
	void initializeYuv2Rgb();
	void paintGL();
	void resizeGL(int width, int height);
	void updateTexture();

	void converYv122Rgb(char *pYv21, int nW, int nH, char *pRgb);

	bool m_bNeedReCreateTexture;
	GLuint m_glnTexture;
	QImage *m_pImageBuffer;
	int		m_nTimerDrawGl;
	int		m_nW;
	int		m_nH;
	QMutex *m_pMutex;
	// yuv 2 rgb
	GLhandleARB FSHandle;
	GLhandleARB PHandle;
	unsigned char *Ytex, *Utex, *Vtex;

	GLuint m_glnTexY;
	GLuint m_glnTexU;
	GLuint m_glnTexV;

	GLuint m_gluVertShader;
	GLuint m_gluFragShader;
	GLuint m_gluProgram;

protected slots:
	;;
	void timerEvent(QTimerEvent *);
private:
	
};
#endif

#endif // GLWINDOW_H
