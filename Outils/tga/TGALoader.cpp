// TGALoader.cpp
// 2004-2010 by Lionel Fuentes, aka Funto.
// Hope this class will be useful to anyone, if you use it in one of your
// projects, an e-mail to funto66 at gmail dot com would be much appreciated :)
// Version 2.1

#include "TGALoader.h"
#include <vector>

#ifdef TGA_USE_LOG_H
#include "../log/Log.h"
#endif

TGALoader::TGALoader() : _data(), _width(0), _height(0), _bpp(0) {
}

TGALoader::TGALoader(const TGALoader& ref) : _width(ref._width), _height(ref._height), _bpp(ref._bpp) {
	if(ref._data) {
		_data.reset(new unsigned char[_width*_height*_bpp]);
		std::copy(ref._data.get(), ref._data.get() + _width*_height*_bpp, _data.get());
	}
}

TGALoader::TGALoader(std::string const &path, TGAErrorCode* error) : _data(), _width(0), _height(0), _bpp(0) {
	if(!error)
		loadFile(path);
	else
		*error = loadFile(path);
}

TGALoader::~TGALoader() {

}

TGAErrorCode TGALoader::loadFile(std::string const &path) {
	std::ifstream file(path, std::ios::binary);
	
#ifdef TGA_USE_LOG_H
	logInfo("loading \"", path, "\"...");
#endif
#ifdef TGA_USE_LOG_IOSTREAM
	std::cout << "loading \"" << path << "\"..." << std::endl;
#endif
	
	if(!file) {
#ifdef TGA_USE_LOG_H
		logFailed(TGA_FILE_NOT_FOUND);
#endif
#ifdef TGA_USE_LOG_IOSTREAM
		cerr << "TGA_FILE_NOT_FOUND" << endl;
#endif
		return TGA_FILE_NOT_FOUND;
	}
		
	std::istreambuf_iterator<char> begin(file), end;
	std::vector<char> fileContent(begin, end);
	
	return this->loadFromData(reinterpret_cast<std::uint8_t *>(&fileContent[0]));
}


TGAErrorCode TGALoader::loadFromData(unsigned char const *imageData) {
#ifdef TGA_USE_LOG_H
	logInfo("loading from data...");
#endif
#ifdef TGA_USE_LOG_IOSTREAM
	std::cout << "loading from data..." << std::endl;
#endif
		
	_width = ((unsigned int)imageData[13] << 8) + (unsigned int)imageData[12];
	_height = ((unsigned int)imageData[15] << 8) + (unsigned int)imageData[14];
	_bpp = ((unsigned int)imageData[16]) / 8;
	
	// Indicates if the origin of the image is
	// at bottom-left (default) or top-left
	bool const origin_non_bottoleft = (imageData[17] & 0x20) == 0x20;
	
	_data.reset(new unsigned char[_height * _width * _bpp]);
	
	// Put the file pointer after the header (18 first bytes) and after the
	// identification field, which size is indicated in header[0].
	size_t pos_data = 18+(unsigned int)imageData[0];
	
	// data[2] determines the type of image:
	// 1 -> uncompressed, uses a palette
	// 2 -> uncompressed, true colors
	// 9 -> RLE compressed, uses a palette
	// 10 -> RLE compressed, doesn't use a palette
	// Other image types are not supported.
	
	// TYPE 2 - uncompressed, true colors, 24 or 32 bits
	if(imageData[2] == 2 && (_bpp == 3 || _bpp == 4)) {
		for(int i=0 ; i<_width*_height*_bpp ; i+=_bpp) {
			this->_data[i+2] = imageData[pos_data++]; // R
			this->_data[i+1] = imageData[pos_data++]; // G
			this->_data[i] = imageData[pos_data++]; // B
			if(_bpp == 4)
				this->_data[i+3] = imageData[pos_data++]; // A
		}
	}
	
	// TYPE 10 - RLE, true colors, 24 or 32 bits
	else if(imageData[2] == 10 && (_bpp == 3 || _bpp == 4)) {
		unsigned char packet_header, red, green, blue, alpha=0;
		unsigned int nb_pixels; // Number of pixels represented by one packet
		for(int i=0 ; i<_width*_height*_bpp ; i += nb_pixels*_bpp) {
			packet_header = imageData[pos_data++];
			nb_pixels = (unsigned int)(packet_header & 0x7F) + 1;
			
			// RLE packet:
			if((packet_header & 0x80) == 0x80) {
				blue = imageData[pos_data++];
				green = imageData[pos_data++];
				red = imageData[pos_data++];
				if(_bpp == 4)
					alpha = imageData[pos_data++];
				
				for(int j=0 ; j<nb_pixels*_bpp ; j += _bpp) {
					_data[i+j] = red;
					_data[i+j+1] = green;
					_data[i+j+2] = blue;
					if(_bpp == 4)
						_data[i+j+3] = alpha;
				}
			}
			// Raw packet:
			else {
				for(int j=0 ; j<nb_pixels*_bpp ; j+=_bpp) {
					_data[i+j+2] = imageData[pos_data++]; // B
					_data[i+j+1] = imageData[pos_data++]; // G
					_data[i+j] = imageData[pos_data++]; // R
					if(_bpp == 4)
						_data[i+j+3] = imageData[pos_data++]; // A
				}
			}
		}
	}
	
	// For images of an unsupported type, return an error and load nothing
	else {
		_data.reset();
#ifdef TGA_USE_LOG_H
		logFailed(TGA_UNSUPPORTED_TYPE);
#endif
#ifdef TGA_USE_LOG_IOSTREAM
		std::cerr << TGA_UNSUPPORTED_TYPE << std::endl;
#endif
		return TGA_UNSUPPORTED_TYPE;
	}
	
	// Flip vertically for images where the origin is in the top left
	if(origin_non_bottoleft) {
		for(int i=0 ; i<_height/2 ; i++) {
			for(int j=0 ; j<_width*_bpp ; j++) {
				std::swap(_data[i*_width*_bpp + j], _data[(_height-i-1)*_width*_bpp + j]);
			}
		}
	}
		
#ifdef TGA_USE_LOG_H
	logSuccess(TGA_OK);
#endif
#ifdef TGA_USE_LOG_IOSTREAM
	std::cout << TGA_OK << std::endl;
#endif
	return TGA_OK;
}

TGALoader& TGALoader::operator=(const TGALoader& ref) {
	TGALoader newLoader(ref);
	std::swap(newLoader, *this);

	return *this;
}

// Convert an error to an explicit string
std::string TGALoader::errorToString(TGAErrorCode error) {
	switch(error) {
		case TGA_OK:
			return "TGA image loaded";
			break;
		case TGA_FILE_NOT_FOUND:
			return "file not found";
			break;
		case TGA_UNSUPPORTED_TYPE:
			return "unsupported type";
			break;
		default:
			return "unknown TGA error...";
			break;
	}
}

#ifdef TGA_OPENGL_SUPPORT

GLuint TGALoader::sendToOpenGL(TGAFiltering filtering) {
	GLuint result = 0;
	if(this->isLoaded()) {
		glGenTextures(1, &result);
		sendToOpenGLWithID(result, filtering);
	}
	return result;
}

void TGALoader::sendToOpenGLWithID( GLuint ID, TGAFiltering filtering) {
	if(this->isLoaded()) {
		glBindTexture(GL_TEXTURE_2D, ID);
		gluBuild2DMipmaps(
				  GL_TEXTURE_2D,
				  _bpp,
				  _width,
				  _height,
				  (_bpp == 3 ? GL_RGB : GL_RGBA),
				  GL_UNSIGNED_BYTE,
				  &_data[0]);
		
		if(filtering == TGA_NO_FILTER) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		}
		else if(filtering == TGA_LINEAR) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		}
		else if(filtering == TGA_BILINEAR) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
					GL_LINEAR_MIPMAP_NEAREST);
		}
		else if(filtering == TGA_TRILINEAR) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
					GL_LINEAR_MIPMAP_LINEAR);
		}
#ifdef GL_TEXTURE_MAX_ANISOTROPY_EXT
		else if(filtering == TGA_ANISOTROPIC) {
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 2.0f) ;
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
					GL_LINEAR_MIPMAP_LINEAR);
		}
#endif
		
		// NB: the application should do this, not the TGA loader...
		//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	}
}

TGAErrorCode TGALoader::loadOpenGLTexture(std::string const &path, GLuint* pID, TGAFiltering filtering ) {
	TGAErrorCode ret = loadFile(path);
	if(ret == TGA_OK) {
		if(pID)
			*pID = sendToOpenGL(filtering);
		else
			sendToOpenGL(filtering);
		_data.release();
	}
	return ret;
}

TGAErrorCode TGALoader::loadOpenGLTextureWithID(std::string const &path, GLuint ID, TGAFiltering filtering) {
	TGAErrorCode ret = loadFile(path);
	if(ret == TGA_OK) {
		sendToOpenGLWithID(ID, filtering);
		_data.release();
	}
	return ret;
}

TGAErrorCode TGALoader::loadOpenGLTextureFromData(std::uint8_t * data, GLuint* pID, TGAFiltering filtering) {
	TGAErrorCode ret = loadFromData(data);
	if(ret == TGA_OK) {
		if(pID != nullptr)
			(*pID) = sendToOpenGL(filtering);
		else
			sendToOpenGL(filtering);
		_data.release();
	}
	return ret;
}

TGAErrorCode TGALoader::loadOpenGLTextureFromDataWithID(std::uint8_t * data, GLuint ID, TGAFiltering filtering) {
	TGAErrorCode ret = loadFromData(data);
	if(ret == TGA_OK) {
		sendToOpenGLWithID(ID, filtering);
		_data.release();
	}
	return ret;
}

#endif // defined TGA_OPENGL_SUPPORT
