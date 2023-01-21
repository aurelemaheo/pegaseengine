/* image.h */

#ifndef IMAGE_H
#define IMAGE_H

#include <stdlib.h>
#include <GL/glut.h>

enum ImageType
{
  IMAGE_TYPE_GRAY,
  IMAGE_TYPE_GRAYA,
  IMAGE_TYPE_RGB,
  IMAGE_TYPE_RGBA
};

struct Image
{
  int            width;
  int            height;
  ImageType      type;
  unsigned char *data;
};

/* OpenGL texture info */
struct gl_texture_t
{
  GLsizei width;
  GLsizei height;

  GLenum format;
  GLint	internalFormat;
  GLuint id;

  GLubyte *texels;

} ;

class Image_Manager
{
public:

	Image_Manager(){};

Image*				image_load_pnm (const char *filename);
GLuint				image_load_tga (const char *filename);
void					image_destroy  (Image *image);
gl_texture_t *		ReadTGAFile (const char *filename);


};

#endif
