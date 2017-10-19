// GLUtils.h - fonctions qui intéragissent avec OpenGL

#ifndef GLUTILS_H
#define GLUTILS_H

#include "CalibrageWebcam.h"
#include "ImageFiltree.h"
#include <GL/glfw.h>

// Création, mise à jour et suppression de textures :
GLuint CreateTextureRGB(int w, int h, const unsigned char* data = NULL);
void UpdateTextureRGB(GLuint id, int w, int h, const unsigned char* data);
void UpdateTextureRGBInvert(GLuint id, int w, int h, const unsigned char* data);
void DeleteTexture(GLuint* pid);

// Préparation du contexte OpenGL pour pouvoir afficher en 2D, en coordonnées de pixels
void Setup2DDisplay(const Color& backgroundColor);

// Affichage :
// - d'une texture dont les données sont déjà à jour :
void DisplayTexture(GLuint id, int x, int y, int w, int h);

// - de données brutes :
void DisplayData(const unsigned char* data, int dataWidth, int dataHeight, GLuint id, int x, int y, int w, int h);

// - d'une ImageFiltree :
void DisplayImage(ImageFiltree* image, GLuint id, int x, int y, int w, int h);

// - des rectangles représentant les taches d'une ImageFiltree :
void DisplayTaches(const ImageFiltree* image, int x, int y, int w, int h, const Color& color1, const Color& color2);

// - d'un rectangle :
void DisplayRect(int x, int y, int w, int h, const Color& color);

// - d'un rectangle en pointillés :
void DisplayRectStipple(int x, int y, int w, int h, const Color& color1, const Color& color2);

// - d'un rectangle rempli :
void DisplayFilledRect(int x, int y, int w, int h, const Color& color);

// - d'une croix :
void DisplayCroix(int x, int y, const Color& color);

// - pour une tache donnée, d'un rectangle avec une croix pour le barycentre.
// (x, y, w, h) définissent la position sur l'écran de l'image à laquelle appartient la tache.
void DisplayTache(const ImageFiltree* image, const ImageFiltree::Tache& tache, int x, int y, int w, int h, const Color& color1, const Color& color2);

// - de la position de la souris :
void DisplayMousePos(const Color& color1, const Color& color2, int size);

// - des positions cliquees
// TODO : ecrire plus proprement ?
void DisplayPositionsCliquees(const CalibrageWebcam& calibrage, int maisCourant, int camHeight, const Color& color);

#endif // GLUTILS_H
