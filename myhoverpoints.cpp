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
# Use this module to add hoverpoints to qobjects for scaling them. inherits HoverPoints.
#
# Last modified: 2013-07-20 12:47
#
# Should you need to contact me, you can do so by 
# email - mail your message to <xufooo@gmail.com>.
=============================================================================*/

#include "myhoverpoints.h"
#include <QDebug>

MyHoverPoints::MyHoverPoints(QWidget *parent, PointShape shape):HoverPoints(parent,shape)
{
	m_widget=parent;
	setConnectionType(HoverPoints::HVLConnection);
	QPolygonF myhoverpoints=QPolygonF(QRectF(parent->rect()));
	myhoverpoints.resize(4);
	setPoints(myhoverpoints);
	qDebug()<<"parent.rect"<<parent->rect();
}

bool MyHoverPoints::eventFilter(QObject *object, QEvent *event)
{
	if(object==m_widget)

		switch (event->type()){
			case QEvent::MouseButtonPress:
				{
					QMouseEvent *me = (QMouseEvent *) event;
					if(me->button()==Qt::RightButton)
						return false;

					if(findClickPos(me->pos())!=-1)
						return HoverPoints::eventFilter(object,event);

					break;
				}

			case QEvent::Resize:
				{
					QPolygonF myhoverpoints=QPolygonF(QRectF(m_widget->rect()));
					myhoverpoints.resize(4);
					setPoints(myhoverpoints);
					return true;
					break;
				}

			default:
				{
					return HoverPoints::eventFilter(object,event);
					break;
				}
		}
	return false;
}

