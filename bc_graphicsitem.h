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
# barcode view in QGraphicsScene
#
# Last modified: 2013-07-22 20:46
#
# Should you need to contact me, you can do so by 
# email - mail your message to <xufooo@gmail.com>.
=============================================================================*/

#ifndef BC_GRAPHICSITEM_H
#define BC_GRAPHICSITEM_H

#include <QGraphicsItem>
#include "bc_generator.h"

class BC_GraphicsItem:public BC_GEN,public QGraphicsItem
{
	Q_OBJECT

public:
	enum {Type = UserType + 11};
	int type() const {return Type;}

	BC_GraphicsItem(QGraphicsItem *parent=0);
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget=0);
	QRectF boundingRect() const;

protected:
	void wheelEvent(QGraphicsSceneWheelEvent *event);

private:
	qreal m_scale;	

};

#endif
