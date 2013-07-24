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
# Last modified: 2013-07-24 12:42
#
# Should you need to contact me, you can do so by 
# email - mail your message to <xufooo@gmail.com>.
=============================================================================*/

#include "designscene.h"
#include <QPainter>

DesignScene::DesignScene(QObject *parent):QGraphicsScene(parent)
{
	m_tile = QPixmap(128,128);
	m_tile.fill(Qt::white);
	QPainter pt(&m_tile);
	QColor color(230,230,230);
	pt.fillRect(0,0,64,64,color);
	pt.fillRect(64,64,64,64,color);
	pt.end();
}

void DesignScene::drawBackground(QPainter *painter,const QRectF &rect)
{
	painter->drawTiledPixmap(rect,m_tile);
	update();
}
