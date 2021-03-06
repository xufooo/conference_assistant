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
	Q_OBJECT

public:
	DesignScene(QObject *parent=0);
	QFont font() const {return myFont;}
	QPixmap background() const {
		if(m_bg)
			return m_background;
		else
			return QPixmap();
		}

	inline bool isBackground(){return m_bg;}

	void setFont(const QFont &font);
	void setFont(QGraphicsItem *item, const QFont &font);

signals:
	void sendFixedSize(bool fixed);
	void itemSelected(QGraphicsItem *item);

public slots:
	void setBackground(const QPixmap &pixmap);
	void emitItemSelected();

private:
	bool isItemChange(int type);

	QPixmap m_tile;
	QPixmap m_background;
	bool m_bg;

	QFont myFont;
};

#endif
