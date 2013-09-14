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
# This module is used for saving and restoring scene.
#
# Last modified: 2013-07-30 12:27
#
# Should you need to contact me, you can do so by 
# email - mail your message to <xufooo@gmail.com>.
=============================================================================*/

#include "scenesaver.h"
#include "designframe.h"
#include "designscene.h"
#include "bc_graphicsitem.h"
#include "graphicstextitem.h"
#include "queryframe.h"
#include <QFileDialog>
#include <QDataStream>
#include <QDebug>

int SceneSaver::save(DesignScene* const scene, QString filename)
{
	if(scene==NULL)
		return 0;
	QMap<QString,QVariant> saver;
	saver.insert("Scene/Rect",scene->sceneRect());
	if(scene->isBackground())
		saver.insert("Scene/Background",scene->background());
	else
		saver.insert("Scene/Background",scene->isBackground());
	saver.insert("Scene/Rect",scene->sceneRect());
//	saver.insert("Scene/Font",scene->font());
	saver.insert("Scene/ItemNumber",scene->items().size());

	for(int i=0;i<scene->items().size();++i){
		QString Item=QString("Item/%1/").arg(i);
		int type=scene->items().at(i)->type();
		saver.insert(Item+"Type",type);
		if(type==BC_GraphicsItem::Type)
		{
			BC_GraphicsItem *item=qgraphicsitem_cast<BC_GraphicsItem*>(scene->items().at(i));
			saver.insert(Item+"BarCode",item->getCode());
		}
		else if(type==GraphicsTextItem::Type)
		{
			GraphicsTextItem *item=qgraphicsitem_cast<GraphicsTextItem*>(scene->items().at(i));
			qDebug()<<"ObjectName:"<<item->objectName();//ooo
			saver.insert(Item+"ObjectName",item->objectName());
			saver.insert(Item+"Context",item->toPlainText());
			saver.insert(Item+"Font",item->font());
		}
		saver.insert(Item+"Pos",scene->items().at(i)->pos());
		saver.insert(Item+"Scale",scene->items().at(i)->scale());
	}

	QString name=QFileDialog::getSaveFileName(0, "Save Design", filename, tr("Design Files(*.sav)"));
	QFile file(name);
	if(!file.open(QIODevice::WriteOnly))
		return 0;
	QDataStream out(&file);
	out << (quint32)0xDE5190;
	out << saver;

	return 1;
}

int SceneSaver::restore(DesignScene* const scene, QString filename)
{
	if(scene==NULL)
		return 0;
	QFile file(filename);
	if(!file.open(QIODevice::ReadOnly))
		return 0;
	QDataStream in(&file);
	QMap<QString,QVariant> loader;

	quint32 magic;
	in >> magic;
	if(magic != 0xDE5190)
		return 0;
	in >> loader;

	scene->clear();

	scene->setSceneRect(loader.value("Scene/Rect").toRectF());//set RectF
	if(loader.value("Scene/Background").type()!=QVariant::Bool)
		scene->setBackground(loader.value("Scene/Background").value<QPixmap>());//set Background

	for(int i=0;i<loader.value("Scene/ItemNumber").toInt();++i)
	{
		if(loader.value(QString("Item/%1/Type").arg(i)).toInt()==BC_GraphicsItem::Type)
		{
			BC_GraphicsItem *item=new BC_GraphicsItem;
			scene->addItem(item);
			item->encode(loader.value(QString("Item/%1/BarCode").arg(i)).toString());
			item->setPos(loader.value(QString("Item/%1/Pos").arg(i)).toPointF());
			item->setScale(loader.value(QString("Item/%1/Scale").arg(i)).toDouble());
		}
		else if(loader.value(QString("Item/%1/Type").arg(i)).toInt()==GraphicsTextItem::Type)
		{
			GraphicsTextItem *item=new GraphicsTextItem;
			scene->addItem(item);
			item->setPlainText(loader.value(QString("Item/%1/Context").arg(i)).toString());
			item->setFont(loader.value(QString("Item/%1/Font").arg(i)).value<QFont>());
			item->setPos(loader.value(QString("Item/%1/Pos").arg(i)).toPointF());
			item->setScale(loader.value(QString("Item/%1/Scale").arg(i)).toDouble());
		}
	}
//	scene->setFont(loader.value("Scene/Font").value<QFont>());//set Background

	return 1;
}

int SceneSaver::save(DesignFrame* const frame, QString filename)
{
	if(frame->getScene()==NULL)
		return 0;
	DesignScene *scene=frame->getScene();
	QMap<QString,QVariant> saver;
	saver.insert("Scene/Rect",scene->sceneRect());
	if(scene->isBackground())
		saver.insert("Scene/Background",scene->background());
	else
		saver.insert("Scene/Background",scene->isBackground());
	saver.insert("Scene/Rect",scene->sceneRect());
//	saver.insert("Scene/Font",scene->font());
	saver.insert("Scene/ItemNumber",scene->items().size());

	for(int i=0;i<scene->items().size();++i){
		QString Item=QString("Item/%1/").arg(i);
		int type=scene->items().at(i)->type();
		saver.insert(Item+"Type",type);
		if(type==BC_GraphicsItem::Type)
		{
			BC_GraphicsItem *item=qgraphicsitem_cast<BC_GraphicsItem*>(scene->items().at(i));
			saver.insert(Item+"BarCode",item->getCode());
		}
		else if(type==GraphicsTextItem::Type)
		{
			GraphicsTextItem *item=qgraphicsitem_cast<GraphicsTextItem*>(scene->items().at(i));
			qDebug()<<"ObjectName:"<<item->objectName();//ooo
			saver.insert(Item+"ObjectName",item->objectName());
			saver.insert(Item+"Context",item->toPlainText());
			saver.insert(Item+"Font",item->font());
		}
		saver.insert(Item+"Pos",scene->items().at(i)->pos());
		saver.insert(Item+"Scale",scene->items().at(i)->scale());
	}

	QString name=QFileDialog::getSaveFileName(0, "Save Design", filename, tr("Design Files(*.sav)"));
	QFile file(name);
	if(!file.open(QIODevice::WriteOnly))
		return 0;
	QDataStream out(&file);
	out << (quint32)0xDE5190;
	out << saver;

	return 1;
}

int SceneSaver::restore(DesignFrame* const frame, QString filename)
{
	if(frame->getScene()==NULL)
		return 0;
	DesignScene *scene=frame->getScene();
	QFile file(filename);
	if(!file.open(QIODevice::ReadOnly))
		return 0;
	QDataStream in(&file);
	QMap<QString,QVariant> loader;

	quint32 magic;
	in >> magic;
	if(magic != 0xDE5190)
		return 0;
	in >> loader;

	scene->clear();

	scene->setSceneRect(loader.value("Scene/Rect").toRectF());//set RectF
//	if(loader.value("Scene/Background").type()!=QVariant::Bool)
		scene->setBackground(loader.value("Scene/Background").value<QPixmap>());//set Background

	for(int i=0;i<loader.value("Scene/ItemNumber").toInt();++i)
	{
		if(loader.value(QString("Item/%1/Type").arg(i)).toInt()==BC_GraphicsItem::Type)
		{
			BC_GraphicsItem *item=new BC_GraphicsItem;
			scene->addItem(item);
			frame->setBC(item);

			item->encode(loader.value(QString("Item/%1/BarCode").arg(i)).toString());
			item->setPos(loader.value(QString("Item/%1/Pos").arg(i)).toPointF());
			item->setScale(loader.value(QString("Item/%1/Scale").arg(i)).toDouble());
		}
		else if(loader.value(QString("Item/%1/Type").arg(i)).toInt()==GraphicsTextItem::Type)
		{
			GraphicsTextItem *item=new GraphicsTextItem;
			item->setObjectName(loader.value(QString("Item/%1/ObjectName").arg(i)).toString());
			scene->addItem(item);
			item->setPlainText(loader.value(QString("Item/%1/Context").arg(i)).toString());
			item->setFont(loader.value(QString("Item/%1/Font").arg(i)).value<QFont>());
			item->setPos(loader.value(QString("Item/%1/Pos").arg(i)).toPointF());
			item->setScale(loader.value(QString("Item/%1/Scale").arg(i)).toDouble());
//			scene->setFont(item, loader.value("Scene/Font").value<QFont>());//set Font
		}
	}

	return 1;
}

int SceneSaver::restore(QueryFrame * const frame, QString filename)
{
	if(frame->getScene()==NULL)
		return 0;
	DesignScene *scene=frame->getScene();
	QFile file(filename);
	if(!file.open(QIODevice::ReadOnly))
		return 0;
	QDataStream in(&file);
	QMap<QString,QVariant> loader;

	quint32 magic;
	in >> magic;
	if(magic != 0xDE5190)
		return 0;
	in >> loader;

	scene->clear();

	scene->setSceneRect(loader.value("Scene/Rect").toRectF());//set RectF
//	if(loader.value("Scene/Background").type()!=QVariant::Bool)
		scene->setBackground(loader.value("Scene/Background").value<QPixmap>());//set Background

	for(int i=0;i<loader.value("Scene/ItemNumber").toInt();++i)
	{
		if(loader.value(QString("Item/%1/Type").arg(i)).toInt()==BC_GraphicsItem::Type)
		{
			BC_GraphicsItem *item=new BC_GraphicsItem;
			scene->addItem(item);
			frame->setBC(item);

//			item->encode(loader.value(QString("Item/%1/BarCode").arg(i)).toString());
			item->setPos(loader.value(QString("Item/%1/Pos").arg(i)).toPointF());
			item->setScale(loader.value(QString("Item/%1/Scale").arg(i)).toDouble());
		}
		else if(loader.value(QString("Item/%1/Type").arg(i)).toInt()==GraphicsTextItem::Type)
		{
			GraphicsTextItem *item=new GraphicsTextItem;
			item->setObjectName(loader.value(QString("Item/%1/ObjectName").arg(i)).toString());
			scene->addItem(item);
			frame->setTextItem(item);
			item->setFont(loader.value(QString("Item/%1/Font").arg(i)).value<QFont>());
//			item->setPlainText(loader.value(QString("Item/%1/Context").arg(i)).toString());
			item->setPos(loader.value(QString("Item/%1/Pos").arg(i)).toPointF());
			item->setScale(loader.value(QString("Item/%1/Scale").arg(i)).toDouble());
//			scene->setFont(item, loader.value("Scene/Font").value<QFont>());//set Font
		}
	}

	return 1;
}

SceneSaver::SceneSaver(DesignScene *const scene,QString filename, QObject* parent):QObject(parent),savescene(scene),savefilename(filename)
{}
