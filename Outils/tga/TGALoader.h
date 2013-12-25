// TGALoader.h
// 2004-2010 by Lionel Fuentes, aka Funto.
// Hope this class will be useful to anyone, if you use it in one of your
// projects, an e-mail to funto66 at gmail dot com would be much appreciated :)
// Version 2.1

#ifndef TGA_LOADER_H
#define TGA_LOADER_H

// Configuration:
//#define TGA_LOADER_INCLUDE_GLEW // Define this if the project uses GLEW,
// for including GL/glew.h before GL/gl.h
#define TGA_OPENGL_SUPPORT
#define TGA_USE_LOG_H // Define this to use log/Log.h for logging (logError(), etc)
//#define TGA_USE_LOG_IOSTREAM // Define this to use std::cout/std::cerr for logging

// ---------------------------------------------------------------------
#ifdef TGA_LOADER_INCLUDE_GLEW
#include <GL/glew.h>
#endif

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <memory>

#ifdef TGA_OPENGL_SUPPORT
#ifdef WIN32
#include <windows.h>
#endif
#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif
#endif // defined TGA_OPENGL_SUPPORT

enum TGAErrorCode {
	TGA_OK,
	TGA_FILE_NOT_FOUND,
	TGA_UNSUPPORTED_TYPE,
};

#ifdef TGA_OPENGL_SUPPORT
enum TGAFiltering {
	TGA_NO_FILTER,
	TGA_LINEAR,
	TGA_BILINEAR,
	TGA_TRILINEAR
#ifdef GL_TEXTURE_MAX_ANISOTROPY_EXT
	,TGA_ANISOTROPIC
#endif
};
#endif

class TGALoader {
public:
	TGALoader();
	TGALoader(const TGALoader& ref);
	TGALoader(std::string const &path, TGAErrorCode* error=nullptr);
	virtual ~TGALoader();
	TGAErrorCode loadFile(std::string const &path);
	TGAErrorCode loadFromData(unsigned char const *data);
	
	TGALoader& operator=(const TGALoader& ref);
	
	// Convert an error to an explicit string
	static std::string errorToString(TGAErrorCode error);
	
#ifdef TGA_OPENGL_SUPPORT
	GLuint sendToOpenGL(TGAFiltering filtering=TGA_NO_FILTER);
	
	void sendToOpenGLWithID(GLuint ID, TGAFiltering filtering=TGA_NO_FILTER);
	
	TGAErrorCode loadOpenGLTexture(std::string const &path, GLuint* pID=nullptr, TGAFiltering filtering=TGA_NO_FILTER);
	
	TGAErrorCode loadOpenGLTextureWithID(std::string const &path, GLuint ID, TGAFiltering filtering=TGA_NO_FILTER);
	
	TGAErrorCode loadOpenGLTextureFromData(unsigned char *data, GLuint* pID=nullptr, TGAFiltering filtering=TGA_NO_FILTER);
	
	TGAErrorCode loadOpenGLTextureFromDataWithID(unsigned char *data, GLuint ID, TGAFiltering filtering=TGA_NO_FILTER);
	
#endif
	
	inline unsigned char const *getData() const {return _data.get();}
	inline bool isLoaded() const {return _data ? true : false;}
	inline unsigned int getHeight() const {return _height;}
	inline unsigned int getWidth() const {return _width;}
	inline unsigned int getBpp() const {return _bpp;}
	
private:
	std::unique_ptr<unsigned char[]> _data;
	unsigned int _width, _height;
	unsigned int _bpp; // Bytes Per Pixel : 0, 3 or 4
	
};

// Overloading << for printing error codes using iostream
inline std::ostream& operator<<(std::ostream& os, const TGAErrorCode& error) {
	return os << TGALoader::errorToString(error);
}

#endif // !defined TGA_LOADER_H
