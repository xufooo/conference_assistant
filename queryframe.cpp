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
#include <QSqlQuery>
#include <QSortFilterProxyModel>
#include <QMessageBox>
#include <QTimer>
#include <QPrinter>
#include <QPrintDialog>
#include <QPrintPreviewWidget>
#include <QSplitter>
#include <QDebug>

QueryFrame::QueryFrame(QWidget *parent):QWidget(parent),model(NULL)
{
	/*setup ui*/
	QHBoxLayout *mainlayout=new QHBoxLayout;
	QSplitter *splitter=new QSplitter;

	/*left*/
	QVBoxLayout *viewlayout=new QVBoxLayout;
	QWidget *leftSide=new QWidget;
	scene=new DesignScene;
	bc=new BC_GraphicsItem;
	scene->addItem(bc);
	connect(scene,SIGNAL(sendFixedSize(bool)),this,SLOT(receiveFixedSize(bool)));
	view=new QGraphicsView;
	view->setScene(scene);

	QHBoxLayout *scalelayout=new QHBoxLayout;
	QPushButton *zoomin=new QPushButton(tr("+"));
	zoomin->setFixedSize(20,20);
	QPushButton *zoomout=new QPushButton(tr("-"));
	zoomout->setFixedSize(20,20);
	QPushButton *resetview=new QPushButton(tr("Reset"));
	resetview->setFixedSize(60,20);
	scalelayout->addWidget(zoomout);
	scalelayout->addWidget(resetview);
	scalelayout->addWidget(zoomin);
	connect(zoomin,SIGNAL(clicked()),this,SLOT(zoomIn()));
	connect(zoomout,SIGNAL(clicked()),this,SLOT(zoomOut()));
	connect(resetview,SIGNAL(clicked()),this,SLOT(resetView()));

	viewlayout->addWidget(view);
	viewlayout->addLayout(scalelayout);

	leftSide->setLayout(viewlayout);

	/*right*/
	QWidget *rightSide=new QWidget;

	QLabel *namelabel=new QLabel(tr("Name :"));
	name=new QLineEdit;
	name->setReadOnly(true);
//	connect(name,SIGNAL(textChanged(const QString&)),scene,SLOT(update()));
	QLabel *numberlabel=new QLabel(tr("NO. :"));
	number=new QLineEdit;
	number->setReadOnly(true);
	QLabel *afflabel=new QLabel(tr("Affiliation:"));
	affiliation=new QLineEdit;
	affiliation->setReadOnly(true);
	connect(number,SIGNAL(textChanged(const QString&)),this,SLOT(setName()));
	connect(number,SIGNAL(textChanged(const QString&)),scene,SLOT(update()));
	QGridLayout *editlayout=new QGridLayout;
	editlayout->addWidget(namelabel,0,0,1,1);
	editlayout->addWidget(name,0,1,1,3);
	editlayout->addWidget(numberlabel,1,0,1,1);
	editlayout->addWidget(number,1,1,1,3);
	editlayout->addWidget(afflabel,2,0,1,1);
	editlayout->addWidget(affiliation,2,1,1,3);

	QLabel *searchlabel=new QLabel(tr("Search :"));
	searchbar=new QLineEdit;
	connect(searchbar,SIGNAL(textChanged(const QString&)),this,SLOT(doSearch(const QString&)));
	connect(searchbar,SIGNAL(returnPressed()),this,SLOT(doChop()));
	clear=new QPushButton(tr("Clear"));
	connect(clear,SIGNAL(clicked()),searchbar,SLOT(clear()));
	connect(clear,SIGNAL(clicked()),searchbar,SLOT(setFocus()));
	editlayout->addWidget(searchlabel,3,0,1,1);
	editlayout->addWidget(searchbar,3,1,1,3);
	editlayout->addWidget(clear,4,3);

	loaddesign=new QPushButton(tr("Load Design"));
	connect(loaddesign,SIGNAL(clicked()),this,SLOT(doLoad()));
	connectdb=new QPushButton(tr("Connect DB"));
	connect(connectdb,SIGNAL(clicked()),this,SLOT(doConnect()));
	signin=new QPushButton(tr("Register"));
	connect(signin,SIGNAL(clicked()),this,SLOT(doSignin()));
	preview=new QPushButton(tr("Preview"));
	connect(preview,SIGNAL(clicked()),this,SLOT(previewScene()));
	print=new QPushButton(tr("Print"));
	connect(print,SIGNAL(clicked()),this,SLOT(doPrint()));
	printall=new QPushButton(tr("Print All"));
	connect(printall,SIGNAL(clicked()),this,SLOT(doPrintAll()));
	QGridLayout *buttonlayout=new QGridLayout;
	buttonlayout->addWidget(loaddesign,0,0);
	buttonlayout->addWidget(connectdb,0,1);
	buttonlayout->addWidget(preview,1,0);
	buttonlayout->addWidget(print,1,1);
	buttonlayout->addWidget(printall,2,0);
	buttonlayout->addWidget(signin,2,1);

	QVBoxLayout *ctrllayout=new QVBoxLayout;
	ctrllayout->addLayout(editlayout);
	ctrllayout->addLayout(buttonlayout);
	rightSide->setLayout(ctrllayout);
	
	/*middle*/
	table=new QTableView;
	proxy=new QSortFilterProxyModel;

	splitter->addWidget(leftSide);
	splitter->addWidget(table);
	splitter->addWidget(rightSide);
	
	mainlayout->addWidget(splitter);
//	mainlayout->addWidget(table);
//	mainlayout->addLayout(ctrllayout);
	setLayout(mainlayout);

	QTimer::singleShot(0,searchbar,SLOT(setFocus()));
}	

void QueryFrame::doChop()
{
	if(BC_GEN::verify(searchbar->text())){
		QString input=searchbar->text();
		input.chop(1);	
		searchbar->setText(input);
	}
	else
		return;
}

void QueryFrame::doSearch(const QString &string)
{
	if(model==NULL)
		return;

	QString input=string;
	proxy->setFilterFixedString(input);
	proxy->setFilterKeyColumn(2);//regnumber
	QModelIndex matchingIndex=proxy->mapToSource(proxy->index(0,0));
	if(matchingIndex.isValid()){
		table->setCurrentIndex(matchingIndex);
		return;
	}

	QSortFilterProxyModel fn;
	fn.setSourceModel(model);
	fn.setFilterFixedString(input);
	fn.setFilterKeyColumn(5);//firstname
	QModelIndex fn_mi=fn.mapToSource(fn.index(0,0));
	if(fn_mi.isValid()){
		table->setCurrentIndex(fn_mi);
		return;
	}

	QSortFilterProxyModel ln;
	ln.setSourceModel(model);
	ln.setFilterFixedString(input);
	ln.setFilterKeyColumn(3);//lastname
	QModelIndex ln_mi=ln.mapToSource(ln.index(0,0));
	if(ln_mi.isValid()){
		table->setCurrentIndex(ln_mi);
		return;
	}

	QSortFilterProxyModel ph;
	ph.setSourceModel(model);
	ph.setFilterFixedString(input);
	ph.setFilterKeyColumn(9);//phone
	QModelIndex ph_mi=ph.mapToSource(ph.index(0,0));
	if(ph_mi.isValid())
		table->setCurrentIndex(ph_mi);
}
										
void QueryFrame::doLoad()
{
	QString fileName=QFileDialog::getOpenFileName(this,tr("Open File"),QDir::currentPath(),tr("Design Files (*.sav)"));

	if(fileName.isEmpty())
		return;

//	delete name;
//	delete regnumber;
	SceneSaver::restore(this,fileName);
}

void QueryFrame::doConnect()
{
	QSqlDatabase db=QSqlDatabase::addDatabase("QSQLITE");
	ConnectDialog cd(&db);
	if(!cd.exec())
		return;//reject

	db.open();
	if(db.lastError().type()!=QSqlError::NoError){
		showError(db.lastError());
		return;
	}

//	db.exec("SET NAMES 'UTF8'");//to avoid Chinese Char "??"

	model=new QSqlTableModel(this);
	model->setEditStrategy(QSqlTableModel::OnFieldChange);
	model->setTable("pre_regusers");
	
	model->setHeaderData(model->fieldIndex("regnumber"),Qt::Horizontal,tr("Reg. NO."));
	model->setHeaderData(model->fieldIndex("firstname"),Qt::Horizontal,tr("First Name"));
	model->setHeaderData(model->fieldIndex("middlename"),Qt::Horizontal,tr("Middle Name"));
	model->setHeaderData(model->fieldIndex("lastname"),Qt::Horizontal,tr("Last Name"));
	model->setHeaderData(model->fieldIndex("fax"),Qt::Horizontal,tr("Phone"));
	model->setHeaderData(model->fieldIndex("company"),Qt::Horizontal,tr("Affiliation"));
	model->setHeaderData(model->fieldIndex(tr("picurl")),Qt::Horizontal,tr("Registration"));
	
	if(!model->select()){
		showError(model->lastError());
		return;
	}

	table->setModel(model);
	for(int i=0;i<model->columnCount();++i)
	{
		if((i==2)|(i==3)|(i==4)|(i==5)|(i==9)|(i==10)|(i==21))
			continue;
		table->hideColumn(i);
	}

	table->setSelectionMode(QAbstractItemView::SingleSelection);
	table->setSelectionBehavior(QAbstractItemView::SelectRows);
	table->setEditTriggers(QAbstractItemView::NoEditTriggers);
//	view->horizontalHeader()->setStretchLastSection(true);

	QDataWidgetMapper *mapper=new QDataWidgetMapper(this);
	mapper->setModel(model);
//	mapper->addMapping(name,model->fieldIndex("name"));
	mapper->addMapping(number,model->fieldIndex("regnumber"));
	mapper->addMapping(affiliation,model->fieldIndex("company"));

	connect(table->selectionModel(),SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),mapper,SLOT(setCurrentModelIndex(QModelIndex)));

	table->setCurrentIndex(model->index(0,0));

	proxy->setSourceModel(model);
}

void QueryFrame::previewScene()
{
	QPrintPreviewWidget *preview=new QPrintPreviewWidget;
	preview->setZoomMode(QPrintPreviewWidget::FitInView);
	connect(preview,SIGNAL(paintRequested(QPrinter*)),this,SLOT(doPreview(QPrinter*)));
	preview->show();
}

void QueryFrame::doPreview(QPrinter *printer)
{
   	QPainter painter(printer);
   	painter.setRenderHint(QPainter::SmoothPixmapTransform);
	if(scene->isBackground())
   		scene->render(&painter);
	else
	{
		QPixmap white(scene->width(),scene->height());
		white.fill();
		scene->setBackground(white);
		scene->render(&painter);
		scene->setBackground(QPixmap());
	}
}

void QueryFrame::doPrint()
{
	QPrinter printer;
	if (QPrintDialog(&printer).exec() == QDialog::Accepted) {
    	QPainter painter(&printer);
     	painter.setRenderHint(QPainter::SmoothPixmapTransform);
		//print without background
//		if(scene->isBackground())
//     		scene->render(&painter);
//		else
//		{
			QPixmap pre_background=scene->background();
			QPixmap white(scene->width(),scene->height());
			white.fill();
			scene->setBackground(white);
			scene->render(&painter);
			scene->setBackground(pre_background);
//		}
	}
}

void QueryFrame::doPrintAll()
{
	QPrinter printer;
	if (QPrintDialog(&printer).exec() == QDialog::Accepted) {
		QPainter painter(&printer);
		painter.setRenderHint(QPainter::SmoothPixmapTransform);
		QModelIndex currentindex=table->currentIndex();
		int rowcount=model->rowCount();
		for(int i=0;i<rowcount;++i)
		{
			table->selectRow(i);
			//print without background
//			if(scene->isBackground())
//     			scene->render(&painter);
//			else
//			{
				QPixmap pre_background=scene->background();
				QPixmap white(scene->width(),scene->height());
				white.fill();
				scene->setBackground(white);
				scene->render(&painter);
				scene->setBackground(pre_background);
//			}
			if(i!=rowcount-1)
				printer.newPage();
		}
		table->setCurrentIndex(currentindex);
		QMessageBox::information(this, tr("Print All"), tr("Print All Complete."));
	}
}

void QueryFrame::doSignin()
{
	if(model==NULL)
		return;
//	if(!model->setData(model->index(table->currentIndex().row(),model->fieldIndex("picurl")),tr("Registered")))
	if(!model->setData(model->index(table->currentIndex().row(),model->fieldIndex("picurl")),model->index(table->currentIndex().row(),model->fieldIndex("picurl")).data().toInt()+1))
		showError(model->lastError());
}

void QueryFrame::setBC(BC_GraphicsItem *newbc)
{
	bc=newbc;
	connect(number,SIGNAL(textChanged(const QString&)),bc,SLOT(encode(const QString&)));
	bc->encode(number->text());
}

void QueryFrame::setTextItem(GraphicsTextItem *newtx)
{
	QString objectname=newtx->objectName();
	qDebug()<<"objectname:"<<objectname;
	if(objectname=="name")
	{
	nameitem=newtx;
	connect(name,SIGNAL(textChanged(const QString&)),nameitem,SLOT(setText(const QString&)));
	nameitem->setText(name->text());
	}
/*	else if(objectname=="regnumber")
	{
	regnumber=newtx;
	connect(number,SIGNAL(textChanged(const QString&)),regnumber,SLOT(setText(const QString&)));
	regnumber->setText(number->text());
	}*/
	else if(objectname=="affiliation")
	{
	affitem=newtx;
	connect(affiliation,SIGNAL(textChanged(const QString&)),affitem,SLOT(setText(const QString&)));
	affitem->setText(affiliation->text());
	}
}

void QueryFrame::showError(const QSqlError &err)
{
	QMessageBox::critical(this, tr("An Error Occur"), tr("Error: ")+err.text());
}

void QueryFrame::receiveFixedSize(bool fixed)
{
	if(fixed){
//		view->setFixedSize(view->sceneRect().width()+2,view->sceneRect().height()+2);
		view->setMaximumSize(view->sceneRect().width()+2,view->sceneRect().height()+2);
		init_zoomout_count=0;
		resetView();
	}
	else{
		view->setMaximumSize(QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));
		init_zoomout_count=(view->sceneRect().width()/view->size().width())<(view->sceneRect().height()/view->size().height()) ? (view->sceneRect().width()/view->size().width())/SCALE_RATIO : (view->sceneRect().height()/view->size().height())/SCALE_RATIO;
		resetView();
	}
}

void QueryFrame::setName()
{
	QString string2upper=model->index(table->currentIndex().row(),model->fieldIndex("firstname")).data().toString();
	QString fname;
	if(string2upper.contains(QRegExp("[\\x4e00-\\x9fa5]+"))){
		fname=model->index(table->currentIndex().row(),model->fieldIndex("lastname")).data().toString()+model->index(table->currentIndex().row(),model->fieldIndex("middlename")).data().toString()+model->index(table->currentIndex().row(),model->fieldIndex("firstname")).data().toString();
	}
	else{
		if(!string2upper.isEmpty())
			fname+=string2upper.replace(0,1,string2upper.at(0).toUpper());
		string2upper=model->index(table->currentIndex().row(),model->fieldIndex("middlename")).data().toString();
		if(!string2upper.isEmpty())
			fname+="  "+string2upper.replace(0,1,string2upper.at(0).toUpper());
		string2upper=model->index(table->currentIndex().row(),model->fieldIndex("lastname")).data().toString();
		if(!string2upper.isEmpty())
			fname+="  "+string2upper.replace(0,1,string2upper.at(0).toUpper());
	}

	name->setText(fname);

}

void QueryFrame::zoomIn()
{
	view->scale(SCALE_RATIO, SCALE_RATIO);
	zoomout_count+=1;
}

void QueryFrame::zoomOut()
{
	if(zoomout_count)
	{
		view->scale(1/SCALE_RATIO, 1/SCALE_RATIO);
		zoomout_count-=1;
	}
}

void QueryFrame::resetView()
{
	view->resetMatrix();
	zoomout_count=init_zoomout_count;
}
