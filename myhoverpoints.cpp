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
#
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
	qDebug()<<"parent.rect"<<parent->rect();
}

bool MyHoverPoints::eventFilter(QObject *object, QEvent *event)
{
	if(object==m_widget)

		switch (event->type()){
			case QEvent::MouseButtonPress:
				{
					QMouseEvent *me = (QMouseEvent *) event;

					if(findClickPos(me->pos())!=-1)
					{
						qDebug()<<"press";
						return HoverPoints::eventFilter(object,event);
					}
					return false;
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

