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
# This module is used for saving and restoring designscene.
# 
# Last modified: 2013-07-30 11:45
#
# Should you need to contact me, you can do so by 
# email - mail your message to <xufooo@gmail.com>.
=============================================================================*/

#ifndef SCENESAVER_H
#define SCENESAVER_H
#include <QObject>
#include <QString>
#include <QDir>
#include <QMap>
#include <QVariant>
class DesignScene;
class DesignFrame;

class SceneSaver:public QObject
{
	Q_OBJECT
public:
		SceneSaver(DesignScene *const scene,QString filename=QDir::currentPath()+"/design.sav", QObject* parent=0);
//		void setFilename(const QString& filename){savefilename=filename;}
//		const QString& getFilename(){return savefilename;}
//		int save();
//		int save(const QString& filename);
		static int save(DesignScene *const scene, QString filename=QDir::currentPath()+"/design.sav");
		static int save(DesignFrame *const frame, QString filename=QDir::currentPath()+"/design.sav");
//		int restore();
//		int restore(DesignScene* const scene);
		static int restore(DesignScene *const scene, QString filename=QDir::currentPath()+"/design.sav");
		static int restore(DesignFrame *const frame, QString filename=QDir::currentPath()+"/design.sav");

private:
		DesignScene *savescene;
		QString savefilename;

//		QMap<QString,QVariant> saver;
};

#endif

