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
# Last modified: 2013-07-26 11:37
#
# Should you need to contact me, you can do so by 
# email - mail your message to <xufooo@gmail.com>.
=============================================================================*/

#ifndef GRAPHICSTEXTITEM_H
#define GRAPHICSTEXTITEM_H

#include <QGraphicsTextItem>

class QFocusEvent;
class QGraphicsItem;
class QGraphicsScene;
class QGraphicsSceneMouseEvent;

class GraphicsTextItem:public QGraphicsTextItem
{
	Q_OBJECT

public:
		enum {Type = UserType + 12};
		int type() const {return Type;}

		GraphicsTextItem(QGraphicsItem *parent=0);

public slots:
		void setText(const QString& string){setPlainText(string);}

signals:
		void lostFocus(GraphicsTextItem *item);
		void selectedChange(QGraphicsItem *item);

protected:
		QVariant itemChange(GraphicsItemChange change, const QVariant &value);
		void focusOutEvent(QFocusEvent *event);
		void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

};

#endif
