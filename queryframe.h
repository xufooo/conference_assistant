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
# Used for querying sql etc.
#
# Last modified: 2013-07-31 22:21
#
# Should you need to contact me, you can do so by 
# email - mail your message to <xufooo@gmail.com>.
=============================================================================*/

#ifndef QUERYFRAME_H
#define QUERYFRAME_H

#include <QWidget>
#define SCALE_RATIO 1.2
class DesignScene;
class QGraphicsView;
class BC_GraphicsItem;
class GraphicsTextItem;
class QLineEdit;
class QPushButton;
class QTableView;
class QSqlError;
class QSqlTableModel;
class QSortFilterProxyModel;
class QPrinter;

class QueryFrame:public QWidget
{
	Q_OBJECT

public:
		QueryFrame(QWidget *parent=0);
		DesignScene * getScene() const{return scene;}
		void setBC(BC_GraphicsItem *const newbc);
		void setTextItem(GraphicsTextItem *const newtx);

public slots:
		void doSearch(const QString &string, int begin_index=0);
		void searchNext();
		void doChop();
		void doLoad();
		void doConnect();
		void doSignin();
		void previewScene();
		void doPreview(QPrinter *printer);
		void doPrint();
		void doPrintAll();
		void receiveFixedSize(bool fixed);
		void setName();

		void zoomIn();
		void zoomOut();
		void resetView();

private:
		void showError(const QSqlError &err);

		DesignScene *scene;
		QGraphicsView *view;
		GraphicsTextItem *nameitem;
//		GraphicsTextItem *regnumber;
		GraphicsTextItem *affitem;
		BC_GraphicsItem *bc;

		QLineEdit *name;
		QLineEdit *number;
		QLineEdit *affiliation;
		QLineEdit *searchbar;

		QPushButton *clear;
		QPushButton *next;
		QPushButton *loaddesign;
		QPushButton *connectdb;
		QPushButton *signin;
		QPushButton *preview;
		QPushButton *print;
		QPushButton *printall;

		QTableView *table;
		QSqlTableModel *model;
		QSortFilterProxyModel *proxy;

		int zoomout_count;
		int init_zoomout_count;
};

#endif
