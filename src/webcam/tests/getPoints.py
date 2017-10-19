#!/usr/bin/python
# -*- coding: utf-8 -*-
import sys
from PyQt4 import QtGui, QtCore
from PyQt4.QtGui import *
from PyQt4.QtCore import * 

class DrawImage(QMainWindow): 
    def __init__(self, parent=None):
        super(QMainWindow, self).__init__(parent)
        self.setWindowTitle('Cliquez exterieur vers interieur du gateau'\
            +', haut vers bas')
        # Ouvre le fichier capture.ppm
        self.local_image = QImage('capture.ppm')
        self.local_grview = QGraphicsView()
        self.setCentralWidget( self.local_grview )
        self.local_scene = QGraphicsScene()
        self.image_format = self.local_image.format()
        self.pixMapItem = QGraphicsPixmapItem(QPixmap(self.local_image)\
            , None, self.local_scene)
        self.local_grview.setScene( self.local_scene )
        self.pixMapItem.mousePressEvent = self.pixelSelect
        # Ouvre un fichier pour mettre les coordonnées des points dedans
        self.fichier = open('points.txt', 'w')
		
    def pixelSelect( self, event ):
        posX = event.pos().x()
        posY = event.pos().y()
        position = QPoint( posX, posY )
        self.fichier.write(str(int(posX)) + "," + str(int(posY)) + "\n")
        pen = QtGui.QPen(QtCore.Qt.green)
        self.local_scene.addRect(posX-1, posY-2, 2, 4, pen)
        self.local_scene.addRect(posX-2, posY-1, 4, 2, pen)
					
	def __del__(self):
		self.fichier.close()


def main():
    form = DrawImage()
    form.show()
    return qApp.exec_()

if __name__ == '__main__':
    app = QtGui.QApplication(sys.argv)
    sys.exit(main()) 
