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
class QGraphicsItem;
class QGraphicsView;
class DesignScene;
class BC_GraphicsItem;
class QComboBox;
class QFontComboBox;
class QPushButton;

class DesignFrame:public QWidget
{
	Q_OBJECT
	
public:
	DesignFrame(QWidget *parent=0);

public slots:
	void receiveFixedSize(bool fixed);
	void printScene();

private slots:
	void currentFontChanged(const QFont &font);
	void fontSizeChanged(const QString &size);
	void handleFontChange();
	void itemSelected(QGraphicsItem *item);

private:
	DesignScene *scene;
	QGraphicsView *view;
	BC_GraphicsItem *bc;

	QComboBox *fontSizeCombo;
	QFontComboBox *fontCombo;
	
	QPushButton *savebutton;
	QPushButton *printbutton;


};

#endif
