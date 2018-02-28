
#include <StdAfx.h>
#include "glwindow.h"

#ifdef QT_OPENGL
#include <QtOpenGL>

#include <math.h>
#include <QMutex>
#ifdef WIN32
#include <gl/glu.h>
#include <gl/wglew.h>
#endif

#ifndef debug
#define debug printf
#endif

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif

#define USE_GPU_YUV

CGlWindow::CGlWindow(QWidget *parent)
	: QGLWidget(parent)
	, m_pImageBuffer(0)
	, m_nW(0)
	, m_nH(0)
	, FSHandle(0)
	, PHandle(0)
	, m_glnTexture(-1)
	, m_bNeedReCreateTexture(false)
	, m_glnTexY(0)
	, m_glnTexV(0)
	, m_glnTexU(0)
{
	
	m_nTimerDrawGl = startTimer(40);
	m_pMutex = new QMutex();


}

CGlWindow::~CGlWindow()
{
	killTimer(m_nTimerDrawGl);
	if (m_pImageBuffer)
	{
		delete m_pImageBuffer;
		m_pImageBuffer = NULL;
	}
	QWidget *pParent = parentWidget();
	if (pParent)
	{

	}
	if (m_pMutex)
	{
		delete m_pMutex;
	}
}
int CGlWindow::Create(int nW, int nH)
{
	m_nW = nW;
	m_nH = nH;
	if (m_glnTexY > 0)
	{
		glDeleteTextures(1, &m_glnTexY);
	}
	if (m_glnTexV > 0)
	{
		glDeleteTextures(1, &m_glnTexV);
	}
	if (m_glnTexU > 0)
	{
		glDeleteTextures(1, &m_glnTexU);
	}
	if (m_glnTexture > 0)
	{
		glDeleteTextures(1, &m_glnTexture);
		m_glnTexture = -1;
	}

	glGenTextures(1, &m_glnTexY);
	glGenTextures(1, &m_glnTexU);
	glGenTextures(1, &m_glnTexV);
	glGenTextures(1, &m_glnTexture);

	GLint last_texture_ID;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture_ID);
	glBindTexture(GL_TEXTURE_2D, m_glnTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
	//	GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);
	// 	gluBuild2DMipmaps(GL_TEXTURE_2D, 4, m_pImageBuffer->width(), m_pImageBuffer->height(),
	// 		GL_RGBA, GL_UNSIGNED_BYTE, pData);
	// 
	// 	glTexImage2D(GL_TEXTURE_2D, 0, 4, nW, nH, 0,
	// 		GL_RGBA, GL_UNSIGNED_BYTE, pData);
#ifdef _WIN32
	glTexImage2D(GL_TEXTURE_2D, 0, 4, nW, nH, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, 0);
#else
	//glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, nW, nH, GL_RGBA, GL_UNSIGNED_BYTE, pData );
	glTexImage2D(GL_TEXTURE_2D, 0, 4, nW, nH, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, 0);
#endif
	glBindTexture(GL_TEXTURE_2D, m_glnTexY);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, nW, nH, 0,
		GL_LUMINANCE, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, m_glnTexU);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, nW/2, nH/2, 0,
		GL_LUMINANCE, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, m_glnTexV);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, nW/2, nH/2, 0,
		GL_LUMINANCE, GL_UNSIGNED_BYTE, 0);


	glBindTexture(GL_TEXTURE_2D, last_texture_ID);

	return 0;
}
void CGlWindow::initializeGL()
{
	QGLWidget::initializeGL();
	
	const QGLContext * pGlContext = QGLWidget::context ();
	if (pGlContext)
	{
		GLenum err = glewInit();

		if (GLEW_OK != err)
		{
			//  Problem: glewInit failed, something is seriously wrong.  
			debug("Error: %s/n", glewGetErrorString(err));
		}

		if (GLEW_ARB_vertex_program)
		{
			//通过了ARB_vertex_program extension 的检查.  
			//glGenProgramsARB(...);
			debug("GLEW_ARB_vertex_program passed\n");
		}
		if (GLEW_VERSION_1_3)
		{
			//   OpenGL 1.3 是被支持的  
			debug("GLEW_VERSION_1_3 passed\n");
		}
		if (glewIsSupported("GL_VERSION_1_4 GL_ARB_point_sprite"))
		{
			//  两个都通过了检查  
			debug("GL_VERSION_1_4 GL_ARB_point_sprite passed\n");
		}
		if (glewGetExtension("GL_ARB_fragment_program"))
		{
			//   检查ARB_fragment_program 是被支持的.  
			debug("ARB_fragment_program passed\n");
		}
	}

	QColor  qtPurple;
	qglClearColor(qtPurple.dark());

	/*glGenFramebuffersEXT();*/
// 
// 	logo = new QtLogo(this, 64);
// 	logo->setColor(qtGreen.dark());

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glShadeModel(GL_SMOOTH);
	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_TEXTURE_2D); 
	//static GLfloat lightPosition[4] = { 0.5, 5.0, 7.0, 1.0 };
	//glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	m_pImageBuffer = new QImage(QString("earth-1.JPG"));
	
	m_nH = m_pImageBuffer->height();
	m_nW = m_pImageBuffer->width();
	//该方法 太慢
	//m_glnTexture = bindTexture(*m_pImageBuffer, GL_TEXTURE_2D);
/*
	glGenTextures(1, &m_glnTexture);
	
	GLint last_texture_ID;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture_ID);
	glBindTexture(GL_TEXTURE_2D, m_glnTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glTexImage2D(GL_TEXTURE_2D, 0,  4, m_pImageBuffer->width(), m_pImageBuffer->height(), 0,
		GL_RGBA , GL_UNSIGNED_BYTE,  m_pImageBuffer->bits());

	glBindTexture(GL_TEXTURE_2D, last_texture_ID);
*/

	Create(m_nW, m_nH);

	initializeYuv2Rgb( );
}

void CGlWindow::paintGL()
{
	m_pMutex->lock();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -10.0);
	glRotatef(0 / 16.0, 1.0, 0.0, 0.0);
	glRotatef(0 / 16.0, 0.0, 1.0, 0.0);
	glRotatef(0 / 16.0, 0.0, 0.0, 1.0);

	
	updateTexture();

	static const GLfloat squareVertices[] = {
		-1.0f, -1.0f,
		1.0f, -1.0f,
		-1.0f,  1.0f,
		1.0f,  1.0f,
	};

	static const GLfloat coordVertices[] = {
		0.0f, 1.0f,
		1.0f, 1.0f,
		0.0f,  0.0f,
		1.0f,  0.0f,
	};
#ifdef USE_GPU_YUV
	glUseProgram(m_gluProgram);

	// Update uniform value
	//glUniform1f(uniforms[UNIFORM_TRANSLATE], 0.0f);
	GLuint textureUniformY = glGetUniformLocation(m_gluProgram, "SamplerY");
	GLuint textureUniformU = glGetUniformLocation(m_gluProgram, "SamplerU");
	GLuint textureUniformV = glGetUniformLocation(m_gluProgram, "SamplerV");
	// Update attribute values
#define  ATTRIB_VERTEX 0
#define  ATTRIB_TEXTURE 1
	//loc = glGetAttribLocation(p,"height");
// 
	glVertexAttribPointer(ATTRIB_VERTEX, 2, GL_FLOAT, 0, 0, squareVertices);
	glEnableVertexAttribArray(ATTRIB_VERTEX);

	glVertexAttribPointer(ATTRIB_TEXTURE, 2, GL_FLOAT, 0, 0, coordVertices);
	glEnableVertexAttribArray(ATTRIB_TEXTURE);
// 	 

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_glnTexY);
	glUniform1i(textureUniformY, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_glnTexU);
	glUniform1i(textureUniformU, 1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_glnTexV);
	glUniform1i(textureUniformV, 2);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
#else 
	glUseProgram(0);
	glBindTexture(GL_TEXTURE_2D, m_glnTexture);

	glBegin(GL_POLYGON);
	//glColor3d(255, 0, 0);
	glTexCoord2f(0.0f, 0.0f);
	//glVertex2f(-0.5f, 0.0f);
	glVertex2f(-0.5f, -0.5f);

	glTexCoord2f(1.0f, 0.0f);
	//glVertex2f(0.5f, 0.0f);
	glVertex2f(0.5f, -0.5f);

	glTexCoord2f(1.0f, 1.0f);
	//glVertex2f(0.5f, 0.5f);
	glVertex2f(0.5f, 0.5f);

	glTexCoord2f(0.0f, 1.0f);
	//glVertex2f(0.0f, 0.5f);
	glVertex2f(-0.5f, 0.5f);

	//glDrawPixels(m_nW, m_nH, GL_RGBA, GL_UNSIGNED_BYTE, m_pImageBuffer->bits());
	glEnd();
	
#endif
	

	m_pMutex->unlock();
	//glDrawPixels(m_nW, m_nH,
	//	GL_RGBA, GL_UNSIGNED_BYTE, m_pImageBuffer->bits());
}
//! [7]

//! [8]
void CGlWindow::resizeGL(int width, int height)
{
	int side = qMin(width, height);
	//glViewport((width - side) / 2, (height - side) / 2, side, side);
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
#ifdef QT_OPENGL_ES_1
	glOrthof(-0.5, +0.5, -0.5, +0.5, 4.0, 15.0);
#else
	glOrtho(-0.5, +0.5, -0.5, +0.5, 4.0, 15.0);
#endif
	glMatrixMode(GL_MODELVIEW);
}
void CGlWindow::timerEvent(QTimerEvent *pEvent)
{
	if (pEvent->timerId() == m_nTimerDrawGl)
	{
/*		static int nFramCount = 0;
//		m_glnTexture = bindTexture(*m_pImageBuffer, GL_TEXTURE_2D);
		nFramCount++;
		if (nFramCount%10 < 5 )
		{
			SetPicture(m_pImageBuffer->bits(), m_pImageBuffer->width()/2, m_pImageBuffer->height()/2);
		}
		else
		{
			SetPicture(m_pImageBuffer->bits(), m_pImageBuffer->width(), m_pImageBuffer->height());
		}
		*/
		//m_pMutex->lock();

		//m_pMutex->unlock();
		updateGL();
	}
	//
}
void CGlWindow::Refresh()
{
	//updateGL();
}
/*
linux 这个操作很消耗 cpu 10% Q8400, 1080p
*/
void CGlWindow::SetPicture(uchar *pData, int nW, int nH)
{
	 
	m_pMutex->lock();
	if (!m_pImageBuffer)
	{
		m_pImageBuffer = new QImage(QSize(nW, nH), QImage::Format_RGB32 );
		//return ;
		m_nH = nH;
		m_nW = nW;
	}
	if (nW != m_nW
		|| nH != m_nH)
	{
		delete m_pImageBuffer;
		m_pImageBuffer = new QImage(QSize(nW, nH), QImage::Format_RGB32 );
		
		m_bNeedReCreateTexture = true;

		//
 		m_nH = nH;
 		m_nW = nW;
	}



	//converYv122Rgb((char*)pData, nW, nH, (char*)m_pImageBuffer->bits());
/*	unsigned char *pLinePos = m_pImageBuffer->bits();
	unsigned char *pLineSrc = pData+m_nW*m_nH;
	for (int y=0; y<m_nH; y++)
	{
		unsigned char *pPix = pLinePos ;//+ (x<<2);
		unsigned char *pSrc = pLineSrc ;//+ (x);
		for (int x=0; x<m_nW; x++)
		{

			pPix[0] = pSrc[0];
			pPix[1] = pSrc[0];
			pPix[2] = pSrc[0];
			pPix[3] = pSrc[0];
			pSrc++;
			pPix += 4;
		}
		pLinePos += (m_nW<<2);
		pLineSrc -= (m_nW);
	}*/
	memcpy(m_pImageBuffer->bits(), pData, nW*nH*3/2);
	//m_glnTexture = bindTexture(*m_pImageBuffer, GL_TEXTURE_2D);
	m_pMutex->unlock();

}

void CGlWindow::updateTexture()
{
	if (m_bNeedReCreateTexture)
	{//重新创建 Texture. 
		Create(m_nW, m_nH);
		m_bNeedReCreateTexture = false ;
	}

	GLint last_texture_ID;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture_ID);
	glBindTexture(GL_TEXTURE_2D, m_glnTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
	//	GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);
	// 	gluBuild2DMipmaps(GL_TEXTURE_2D, 4, m_pImageBuffer->width(), m_pImageBuffer->height(),
	// 		GL_RGBA, GL_UNSIGNED_BYTE, pData);
	// 
	// 	glTexImage2D(GL_TEXTURE_2D, 0, 4, nW, nH, 0,
	// 		GL_RGBA, GL_UNSIGNED_BYTE, pData);
#ifdef _WIN32
	//glTexImage2D(GL_TEXTURE_2D, 0, 4, m_nW, m_nH, 0,
	//		GL_RGBA /*GL_LUMINANCE*/, GL_UNSIGNED_BYTE, pData);
	//glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_nW, m_nH, GL_RGBA, GL_UNSIGNED_BYTE, m_pImageBuffer->bits() );
	//converYv122Rgb((char*)m_pImageBuffer->bits(), m_nW, m_nH, 0);
#ifdef USE_GPU_YUV
	converYv122Rgb((char*)m_pImageBuffer->bits(), m_nW, m_nH, 0);
#else
	//glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_nW, m_nH, GL_RGBA, GL_UNSIGNED_BYTE, m_pImageBuffer->bits() );
	glTexImage2D(GL_TEXTURE_2D, 0, 4, m_nW, m_nH, 0,
			GL_RGBA /*GL_LUMINANCE*/, GL_UNSIGNED_BYTE, m_pImageBuffer->bits());
#endif 
#else
#ifdef USE_GPU_YUV
	converYv122Rgb((char*)m_pImageBuffer->bits(), m_nW, m_nH, 0);
#else
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_nW, m_nH, GL_RGBA, GL_UNSIGNED_BYTE, m_pImageBuffer->bits() );
#endif 
#endif

	glBindTexture(GL_TEXTURE_2D, last_texture_ID);

}

void CGlWindow::converYv122Rgb(char *pYv21, int nW, int nH, char *pRgb)
{

	Ytex=(unsigned char*)pYv21;//(unsigned char*)malloc(nW*nH);
	Utex=(unsigned char*)pYv21 + nW*nH;//(unsigned char*)malloc(nW*nH/4);
	Vtex=(unsigned char*)pYv21 + nW*nH*5/4;//( char*)malloc(nW*nH/4);
	// 
	//  	GLint last_texture_ID;
	//  	glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture_ID);
	// 	glBindTexture(GL_TEXTURE_2D, m_glnTexture);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_glnTexY);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, m_nW, m_nH, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, Ytex);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_nW, m_nH, GL_LUMINANCE, GL_UNSIGNED_BYTE, Ytex );
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_glnTexU);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, m_nW/2, m_nH/2, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, Utex);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_nW/2, m_nH/2, GL_LUMINANCE, GL_UNSIGNED_BYTE, Utex );
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_glnTexV);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, m_nW/2, m_nH/2, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, Vtex);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_nW/2, m_nH/2, GL_LUMINANCE, GL_UNSIGNED_BYTE, Vtex );
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}
void CGlWindow::initializeYuv2Rgb(/*int nW, int nH*/)
{

	m_gluVertShader = glCreateShader(GL_VERTEX_SHADER);
	m_gluFragShader = glCreateShader(GL_FRAGMENT_SHADER);

	char *pVertShader = (char*) malloc(1024 );
	memset(pVertShader, 0, (1024));
	FILE *pFileVert = fopen("yuv_vert.glsh", "r+b");
	int nLen = 0;
	if (pFileVert)
	{
		fseek(pFileVert, 0, SEEK_END);
		nLen =  ftell(pFileVert);
		rewind(pFileVert);
		fread(pVertShader, nLen, 1, pFileVert);
		fclose(pFileVert);
		debug("%s\n", pVertShader);
	}
	glShaderSource(m_gluVertShader, 1, (const GLchar**)&pVertShader,NULL);

	char *pFragShader = (char*) malloc(1024 );
	memset(pFragShader, 0, (1024));
	FILE *pFileFrag= fopen("yuv_frag.glsh", "r+b");
	if (pFileFrag)
	{
		fseek(pFileFrag, 0, SEEK_END);
		nLen =  ftell(pFileFrag);
		rewind(pFileFrag);
		fread(pFragShader, nLen, 1, pFileFrag);
		fclose(pFileFrag);
		//debug("%s\n", pFragShader);
	}
	glShaderSource(m_gluFragShader, 1, (const GLchar**)&pFragShader,NULL);

	glCompileShader(m_gluVertShader);

	int infologLength = 0;  
	int charsWritten  = 0;   

	glGetShaderiv(m_gluVertShader, GL_INFO_LOG_LENGTH, &infologLength);  
	char *pLog = (char*) malloc(1024 );
	if (infologLength > 0)
	{
		glGetShaderInfoLog(m_gluVertShader, infologLength, &charsWritten, pLog);  
		qDebug("%s\n", pLog);  
	}

	glCompileShader(m_gluFragShader);
	glGetShaderiv(m_gluFragShader, GL_INFO_LOG_LENGTH, &infologLength);  
	if (infologLength > 0)
	{
		glGetShaderInfoLog(m_gluFragShader, infologLength, &charsWritten, pLog);  
		qDebug("%s\n", pLog);  
	}

	m_gluProgram = glCreateProgram();

	glAttachShader(m_gluProgram, m_gluFragShader);
	glAttachShader(m_gluProgram, m_gluVertShader);

	glLinkProgram(m_gluProgram);

	glGetProgramiv(m_gluProgram, GL_INFO_LOG_LENGTH, &infologLength);  
	if (infologLength > 0)  
	{  
		glGetProgramInfoLog(m_gluProgram, infologLength, &charsWritten, pLog);  
		qDebug("%s\n", pLog);  
	}  
	//delete pLog;
	//delete pVertShader;
	//delete pFragShader;
	free(pLog);
	free(pFragShader);
	free(pVertShader);

//	glUseProgram(m_gluProgram);	



}
#endif

/*


*/
