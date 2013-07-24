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
# card design display, print, etc.
#
# Last modified: 2013-07-22 19:47
#
# Should you need to contact me, you can do so by 
# email - mail your message to <xufooo@gmail.com>.
=============================================================================*/

#include "designframe.h"
#include "designscene.h"
#include "bc_graphicsitem.h"
#include <QGraphicsTextItem>
#include <QDebug>

DesignFrame::DesignFrame(QWidget *parent):QGraphicsView(parent)
{
	sc = new DesignScene;
	bc = new BC_GraphicsItem;
	bc->encode("1234567");
	QGraphicsTextItem *ti=new QGraphicsTextItem("lalala");
	ti->setFlag(QGraphicsItem::ItemIsMovable);
	ti->setFlag(QGraphicsItem::ItemIsSelectable);
	sc->addItem(bc);
	sc->addItem(ti);
	setScene(sc);
	QObject::connect(sc,SIGNAL(sendFixedSize(bool)),this,SLOT(receiveFixedSize(bool)));
	sc->setBackground(QPixmap("bg1.jpg"));
}

void DesignFrame::receiveFixedSize(bool fixed)
{
	if(fixed)
		setFixedSize(sceneRect().width()+2,sceneRect().height()+2);
	else
		QWidget::setMaximumSize(QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));
}

