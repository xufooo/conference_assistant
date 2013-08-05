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
# Last modified: 2013-08-05 10:54
#
# Should you need to contact me, you can do so by 
# email - mail your message to <xufooo@gmail.com>.
=============================================================================*/

#include "queryframe.h"
#include "designscene.h"
#include <QGraphicsView>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTableView>
#include <QFileDialog>
#include "scenesaver.h"
#include "bc_graphicsitem.h"
#include "graphicstextitem.h"
#include <QSqlDatabase>
#include "connectdb.h"
#include <QSqlError>
#include <QDataWidgetMapper>
#include <QSqlTableModel>
#include <QSortFilterProxyModel>
#include <QMessageBox>
#include <QTimer>
#include <QPrinter>
#include <QPrintDialog>
#include <QDebug>

QueryFrame::QueryFrame(QWidget *parent):QWidget(parent)
{
	/*setup ui*/
	/*left*/
	scene=new DesignScene;
	bc=new BC_GraphicsItem;
	scene->addItem(bc);
	view=new QGraphicsView;
	view->setScene(scene);
	QHBoxLayout *mainlayout=new QHBoxLayout;

	/*right*/
	QLabel *namelabel=new QLabel(tr("Name :"));
	name=new QLineEdit;
	name->setReadOnly(true);
	connect(name,SIGNAL(textChanged(const QString&)),scene,SLOT(update()));
	QLabel *numberlabel=new QLabel(tr("NO. :"));
	number=new QLineEdit;
	number->setReadOnly(true);
	connect(number,SIGNAL(textChanged(const QString&)),scene,SLOT(update()));
	QGridLayout *editlayout=new QGridLayout;
	editlayout->addWidget(namelabel,0,0,1,1);
	editlayout->addWidget(name,0,1,1,3);
	editlayout->addWidget(numberlabel,1,0,1,1);
	editlayout->addWidget(number,1,1,1,3);

	QLabel *searchlabel=new QLabel(tr("Search :"));
	searchbar=new QLineEdit;
	connect(searchbar,SIGNAL(textChanged(const QString&)),this,SLOT(doSearch(const QString&)));
	clear=new QPushButton(tr("Clear"));
	connect(clear,SIGNAL(clicked()),searchbar,SLOT(clear()));
	connect(clear,SIGNAL(clicked()),searchbar,SLOT(setFocus()));
	editlayout->addWidget(searchlabel,2,0,1,1);
	editlayout->addWidget(searchbar,2,1,1,3);
	editlayout->addWidget(clear,3,3);

	loaddesign=new QPushButton(tr("Load Design"));
	connect(loaddesign,SIGNAL(clicked()),this,SLOT(doLoad()));
	connectdb=new QPushButton(tr("Connect DB"));
	connect(connectdb,SIGNAL(clicked()),this,SLOT(doConnect()));
	signin=new QPushButton(tr("Sign In"));
	connect(signin,SIGNAL(clicked()),this,SLOT(doSignin()));
	print=new QPushButton(tr("Print"));
	connect(print,SIGNAL(clicked()),this,SLOT(doPrint()));
	printall=new QPushButton(tr("Print All"));
	connect(printall,SIGNAL(clicked()),this,SLOT(doPrintAll()));
	QGridLayout *buttonlayout=new QGridLayout;
	buttonlayout->addWidget(loaddesign,0,0);
	buttonlayout->addWidget(connectdb,0,1);
	buttonlayout->addWidget(print,1,0);
	buttonlayout->addWidget(printall,1,1);
	buttonlayout->addWidget(signin,2,0,1,2);

	QVBoxLayout *ctrllayout=new QVBoxLayout;
	ctrllayout->addLayout(editlayout);
	ctrllayout->addLayout(buttonlayout);
	
	/*middle*/
	table=new QTableView(this);
	proxy=new QSortFilterProxyModel;
	
	mainlayout->addWidget(view);
	mainlayout->addWidget(table);
	mainlayout->addLayout(ctrllayout);
	setLayout(mainlayout);

	QTimer::singleShot(0,searchbar,SLOT(setFocus()));
}	

void QueryFrame::doSearch(const QString& string)
{
	QString input;
	if(string.endsWith("\n")||string.endsWith("\t")||string.endsWith("\r")){
		input=string.trimmed();
		if(BC_GEN::verify(input))
			input.chop(1);	
		else
			return;
	}
	else
		input=string;
	proxy->setFilterFixedString(input);
	proxy->setFilterKeyColumn(1);
	QModelIndex matchingIndex=proxy->mapToSource(proxy->index(0,0));
	if(matchingIndex.isValid()){
		table->setCurrentIndex(matchingIndex);
		return;
	}
	QSortFilterProxyModel pro;
	pro.setSourceModel(model);
	pro.setFilterFixedString(input);
	QModelIndex mi=pro.mapToSource(pro.index(0,0));
	if(mi.isValid())
		table->setCurrentIndex(mi);
}
										
void QueryFrame::doLoad()
{
	QString fileName=QFileDialog::getOpenFileName(this,tr("Open File"),QDir::currentPath(),tr("Design Files (*.sav)"));

	if(fileName.isEmpty())
		return;

	SceneSaver::restore(this,fileName);
}

void QueryFrame::doConnect()
{
	QSqlDatabase db=QSqlDatabase::addDatabase("QMYSQL");
	ConnectDialog cd(&db);
	if(!cd.exec())
		return;//reject

	db.open();
	if(db.lastError().type()!=QSqlError::NoError){
		showError(db.lastError());
		return;
	}

	model=new QSqlTableModel(this);
	model->setEditStrategy(QSqlTableModel::OnFieldChange);
	model->setTable("test");
	
	model->setHeaderData(model->fieldIndex("name"),Qt::Horizontal,tr("Name"));
	model->setHeaderData(model->fieldIndex("number"),Qt::Horizontal,tr("NO."));
	model->setHeaderData(model->fieldIndex("signin"),Qt::Horizontal,tr("Sign In"));

	if(!model->select()){
		showError(model->lastError());
		return;
	}

	table->setModel(model);
	table->setSelectionMode(QAbstractItemView::SingleSelection);
	table->setSelectionBehavior(QAbstractItemView::SelectRows);
	table->setEditTriggers(QAbstractItemView::NoEditTriggers);
//	view->horizontalHeader()->setStretchLastSection(true);

	QDataWidgetMapper *mapper=new QDataWidgetMapper(this);
	mapper->setModel(model);
	mapper->addMapping(name,model->fieldIndex("name"));
	mapper->addMapping(number,model->fieldIndex("number"));

	connect(table->selectionModel(),SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),mapper,SLOT(setCurrentModelIndex(QModelIndex)));

	table->setCurrentIndex(model->index(0,0));

	proxy->setSourceModel(model);
}

void QueryFrame::doPrint()
{
	QPrinter printer;
	if (QPrintDialog(&printer).exec() == QDialog::Accepted) {
    	QPainter painter(&printer);
     	painter.setRenderHint(QPainter::SmoothPixmapTransform);
		if(scene->isBackground())
     		scene->render(&painter);
		else
		{
			QPixmap white(scene->width(),scene->height());
			white.fill();
			scene->setBackground(white);
			scene->render(&painter);
			scene->setBackground(NULL);
		}
	}
}

void QueryFrame::doPrintAll()
{
}

void QueryFrame::doSignin()
{
	if(!model->setData(model->index(table->currentIndex().row(),model->fieldIndex("signin")),true))
		showError(model->lastError());
}

void QueryFrame::setBC(BC_GraphicsItem *newbc)
{
	bc=newbc;
	connect(number,SIGNAL(textChanged(const QString&)),bc,SLOT(encode(const QString&)));
}

void QueryFrame::setTextItem(GraphicsTextItem *newtx)
{
	tx=newtx;
	connect(name,SIGNAL(textChanged(const QString&)),tx,SLOT(setText(const QString&)));
}

void QueryFrame::showError(const QSqlError &err)
{
	QMessageBox::critical(this, tr("An Error Occur"), tr("Error: ")+err.text());
}
