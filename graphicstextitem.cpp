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
# To put text on the qgraphicsscene, inherits QGraphicsTextItem.
#
# Last modified: 2013-07-26 11:45
#
# Should you need to contact me, you can do so by 
# email - mail your message to <xufooo@gmail.com>.
=============================================================================*/

#include "graphicstextitem.h"

GraphicsTextItem::GraphicsTextItem(QGraphicsItem  *parent, QGraphicsScene *scene):QGraphicsTextItem(parent, scene)
{
	setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
}

QVariant GraphicsTextItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
	if(change==QGraphicsItem::ItemSelectedHasChanged)
		emit selectedChange(this);
	return value;
}

void GraphicsTextItem::focusOutEvent(QFocusEvent *event)
{
	setTextInteractionFlags(Qt::NoTextInteraction);
	emit lostFocus(this);
	QGraphicsTextItem::focusOutEvent(event);
}

void GraphicsTextItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
	if(textInteractionFlags()==Qt::NoTextInteraction)
		setTextInteractionFlags(Qt::TextEditorInteraction);
	QGraphicsTextItem::mouseDoubleClickEvent(event);
}
