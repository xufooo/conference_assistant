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
# Last modified: 2013-07-22 20:58
#
# Should you need to contact me, you can do so by 
# email - mail your message to <xufooo@gmail.com>.
=============================================================================*/

#include "bc_graphicsitem.h"
#include <QGraphicsSceneWheelEvent>

#include <QDebug>

BC_GraphicsItem::BC_GraphicsItem(QGraphicsItem *parent):QGraphicsItem(parent),m_scale(1)
{
	setFlags(QGraphicsItem::ItemIsMovable|QGraphicsItem::ItemIsSelectable);
}

QRectF BC_GraphicsItem::boundingRect() const
{
	return rect();
}

void BC_GraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	painter->drawPixmap(rect(),*(getPixmap()));
	if(isSelected())
	{
		painter->save();
		painter->setPen(Qt::DashLine);
		painter->drawRect(rect());
		painter->restore();
	}
}

void BC_GraphicsItem::wheelEvent(QGraphicsSceneWheelEvent *event)
{
	if(!isSelected())
	{
		event->ignore();
		return;
	}

	m_scale -= event->delta() / qreal(1200);
    m_scale = qMax(qreal(1), qMin(qreal(5), m_scale));
    setScale(m_scale);
}
