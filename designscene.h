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
# Design Scene
#
# Last modified: 2013-07-24 12:39
#
# Should you need to contact me, you can do so by 
# email - mail your message to <xufooo@gmail.com>.
=============================================================================*/

#ifndef DESIGNSCENE_H
#define DESIGNSCENE_H

#include <QGraphicsScene>
#include <QPixmap>

class DesignScene:public QGraphicsScene
{
	Q_OBJECT;

public:
	DesignScene(QObject *parent=0);

protected:
	void drawBackground(QPainter *painter,const QRectF &rect);

private:
	QPixmap m_tile;
};

#endif
