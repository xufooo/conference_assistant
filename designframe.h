/*=============================================================================
# Copyright (C) 2013 OoO
#
# This program is free software: you can redistribute it and/or modify 
# it under the terms of the GNU General Public License as published by 
# the Free Software Foundation, either version 3 of the License, or 
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful, 
# but WITHOUT ANY WARRANTY; without even the implied warranty of 
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License 
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
#
# Description: 
# card design display, print, etc.
#
# Last modified: 2013-07-22 19:32
#
# Should you need to contact me, you can do so by 
# email - mail your message to <xufooo@gmail.com>.
=============================================================================*/

#ifndef DESIGNFRAME_H
#define DESIGNFRAME_H

#include <QWidget>
#define SCALE_RATIO 1.2
class QGraphicsItem;
class QGraphicsView;
class DesignScene;
class BC_GraphicsItem;
class QGraphicsTextItem;
class QComboBox;
class QFontComboBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QPrinter;

class DesignFrame:public QWidget
{
	Q_OBJECT
	
public:
	DesignFrame(QWidget *parent=0);
	DesignScene * getScene() const {return scene;}
	void setBC(BC_GraphicsItem* const newbc);

//signals:
//	void toResize(const QSize &size);

public slots:
	void receiveFixedSize(bool fixed);
	void printScene();
	void previewScene();
	void doPreview(QPrinter *printer);
	void zoomIn();
	void zoomOut();
	void resetView();

private slots:
	void currentFontChanged(const QFont &font);
	void fontSizeChanged(const QString &size);
	void handleFontChange();
	void itemSelected(QGraphicsItem *item);
	
	int open();
	void save();

private:
	DesignScene *scene;
	QGraphicsView *view;
	BC_GraphicsItem *bc;
//	QGraphicsTextItem *regnumber;

	QComboBox *fontSizeCombo;
	QFontComboBox *fontCombo;

	QLabel *bc_label;
	QLineEdit *bc_line;
	
	QPushButton *openbutton;
	QPushButton *savebutton;
	QPushButton *previewbutton;
	QPushButton *printbutton;

	int zoomout_count;
	int init_zoomout_count;
};

#endif
