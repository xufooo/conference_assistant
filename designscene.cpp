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
#include "graphicstextitem.h"
#include <QGraphicsTextItem>
#include <QPainter>
#include <QTimer>
#include <QApplication>
#include <QDesktopWidget>

DesignScene::DesignScene(QObject *parent):QGraphicsScene(parent),m_bg(false)
{
	m_tile = QPixmap(128,128);
	m_tile.fill(Qt::white);
	QPainter pt(&m_tile);
	QColor color(230,230,230);
	pt.fillRect(0,0,64,64,color);
	pt.fillRect(64,64,64,64,color);
	pt.end();
	setBackgroundBrush(m_tile);
	setSceneRect(0,0,300,300);
	connect(this,SIGNAL(selectionChanged()),this,SLOT(emitItemSelected()));
}

void DesignScene::setBackground(const QPixmap &pixmap)
{
	if(pixmap.isNull()){
		m_bg=false;
		setBackgroundBrush(m_tile);
	}
	else
	{
		m_bg=true;
		m_background=pixmap;
		setBackgroundBrush(m_background);
//		if(width()>m_background.width()||height()>m_background.height()){
//			foreach(QGraphicsItem *item,items())
//				item->setPos(0,0);
//		}
		setSceneRect(sceneRect().x(),sceneRect().y(),m_background.width(),m_background.height());
		QRect desktop_rect=QApplication::desktop()->availableGeometry();
		if(m_background.width()>desktop_rect.width()-50 || m_background.height()>desktop_rect.height()-100)
		{
			emit sendFixedSize(!m_bg);
			update();
			return;
		}
	}
	emit sendFixedSize(m_bg);
	update();
}

void DesignScene::setFont(const QFont &font)
{
	if(myFont==font)
		return;
	myFont=font;
	if(isItemChange(GraphicsTextItem::Type)){
		GraphicsTextItem *item=qgraphicsitem_cast<GraphicsTextItem *>(selectedItems().first());
		if(item)
			item->setFont(myFont);
	}
	update();
}

void DesignScene::setFont(QGraphicsItem *item, const QFont &font)
{
	myFont=font;
	if(item->type()==GraphicsTextItem::Type){
		GraphicsTextItem *myitem=qgraphicsitem_cast<GraphicsTextItem *>(item);
		if(myitem)
			myitem->setFont(myFont);
	}
	update();
}
bool DesignScene::isItemChange(int type)
{
	foreach(QGraphicsItem *item, selectedItems()){
		if(item->type()==type)
			return true;
	}
	return false;
}

void DesignScene::emitItemSelected()
{
	if(isItemChange(GraphicsTextItem::Type)){
		GraphicsTextItem *item=qgraphicsitem_cast<GraphicsTextItem *>(selectedItems().first());
		if(item)
			emit itemSelected(item);
	}
}
	
