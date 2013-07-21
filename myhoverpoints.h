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
# Last modified: 2013-07-20 12:19
#
# Should you need to contact me, you can do so by 
# email - mail your message to <xufooo@gmail.com>.
=============================================================================*/

#ifndef MYHOVERPOINTS_H
#define MYHOVERPOINTS_H

#include "hoverpoints.h"

class MyHoverPoints:public HoverPoints
{
	Q_OBJECT;
public:
	MyHoverPoints(QWidget *parent, PointShape shape=HoverPoints::CircleShape);

	bool eventFilter(QObject *object, QEvent *event);
	void setEditable(bool editable){m_editable=editable;HoverPoints::setEditable(editable);}
	bool editable() const {return m_editable;}
	bool enabled() const {return m_enabled;}

public slots:
	void setEnabled(bool enabled);

signals:
	void updateRect(const QRectF &rect);
	
private:
	QWidget *m_widget;
	bool m_editable;
	bool m_enabled;
};


#endif
