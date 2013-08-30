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
# This module is used for creating information.
#
# Last modified: 2013-07-07 16:59
#
# Should you need to contact me, you can do so by 
# email - mail your message to <xufooo@gmail.com>.
=============================================================================*/

#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QMessageBox>
#include <QTableView>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QDataWidgetMapper>
#include <QHeaderView>

#include "bc_generator.h"
#include "create_info.h"
#include "connectdb.h"
#include <QDebug>

CreateInfo::CreateInfo(QWidget *parent):QWidget(parent){

	/*setup ui*/
	QHBoxLayout *mainLayout = new QHBoxLayout(this);
	barcode = new BC_GEN;

	QVBoxLayout *panellayout = new QVBoxLayout;

	QGridLayout *editlayout = new QGridLayout;
	namelabel = new QLabel(tr("Name :"));
	name = new QLineEdit;

	numberlabel = new QLabel(tr("NO. :"));
	number = new QLineEdit(this);
	editlayout->addWidget(namelabel,0,0);
	editlayout->addWidget(name,0,1);
	editlayout->addWidget(numberlabel,1,0);
	editlayout->addWidget(number,1,1);
	connect(number,SIGNAL(textChanged(const QString&)),barcode,SLOT(encode(const QString&)));
	panellayout->addLayout(editlayout);

	QHBoxLayout *buttonlayout = new QHBoxLayout;
	p_connect = new QPushButton(tr("Connect"));
	p_insert = new QPushButton(tr("Insert"));
	p_delete = new QPushButton(tr("Delete"));
	p_save = new QPushButton(tr("Save"));
	buttonlayout->addWidget(p_connect);
	buttonlayout->addWidget(p_insert);
	buttonlayout->addWidget(p_delete);
	buttonlayout->addWidget(p_save);
	panellayout->addLayout(buttonlayout);
	panellayout->addWidget(barcode);

	connect(p_connect,SIGNAL(clicked()),this,SLOT(doConnect()));
	connect(p_insert,SIGNAL(clicked()),this,SLOT(doInsert()));
	connect(p_delete,SIGNAL(clicked()),this,SLOT(doDelete()));
	connect(p_save,SIGNAL(clicked()),this,SLOT(doSave()));

	view = new QTableView(this);

	mainLayout->addLayout(panellayout);
	mainLayout->addWidget(view);
	setLayout(mainLayout);

	QTimer::singleShot(1,number,SLOT(setFocus()));//focus on number
	
	/*setup db*/
	if(!QSqlDatabase::drivers().contains("QSQLITE"))
		QMessageBox::critical(this, tr("Unable to load database"), tr("No QMYSQL driver"));
}

void CreateInfo::doConnect()
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
//	db.exec("SET NAMES 'UTF8'");

	model=new QSqlTableModel(this);
	model->setEditStrategy(QSqlTableModel::OnManualSubmit);
	QSqlQuery query;
//	query.exec("CREATE TABLE IF NOT EXISTS test (name text NOT NULL, number text NOT NULL UNIQUE, signin integer NOT NULL DEFAULT 0);");
	if(query.lastError().type()!=QSqlError::NoError)
		showError(query.lastError());
	model->setTable(tr("pre_regusers"));
	
	model->setHeaderData(model->fieldIndex("regnumber"),Qt::Horizontal,tr("Reg. NO."));
	model->setHeaderData(model->fieldIndex("firstname"),Qt::Horizontal,tr("First Name"));
	model->setHeaderData(model->fieldIndex("middlename"),Qt::Horizontal,tr("Middle Name"));
	model->setHeaderData(model->fieldIndex("lastname"),Qt::Horizontal,tr("Last Name"));
	model->setHeaderData(model->fieldIndex("fax"),Qt::Horizontal,tr("Phone"));
	model->setHeaderData(model->fieldIndex("company"),Qt::Horizontal,tr("Affiliation"));
//	model->setHeaderData(model->fieldIndex(tr("signin")),Qt::Horizontal,tr("Sign In"));

	if(!model->select()){
		showError(model->lastError());
		return;
	}

	view->setModel(model);
	for(int i=0;i<model->columnCount();++i)
	{
		if((i==2)|(i==3)|(i==4)|(i==5)|(i==9)|(i==10))
			continue;
		view->hideColumn(i);
	}

	view->setSelectionMode(QAbstractItemView::SingleSelection);
//	view->horizontalHeader()->setStretchLastSection(true);

	QDataWidgetMapper *mapper=new QDataWidgetMapper(this);
	mapper->setModel(model);
//	mapper->addMapping(name,model->fieldIndex(tr("name")));
	mapper->addMapping(number,model->fieldIndex("regnumber"));

	connect(view->selectionModel(),SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),mapper,SLOT(setCurrentModelIndex(QModelIndex)));

	view->setCurrentIndex(model->index(0,0));
}

void CreateInfo::doInsert()
{
	if(!model->insertRow(model->rowCount())){
		showError(model->lastError());
			return;
	}
	view->setCurrentIndex(model->index(model->rowCount()-1,0));
}

void CreateInfo::doDelete()
{
	if(!model->removeRow(view->currentIndex().row()))
		showError(model->lastError());
}

void CreateInfo::doSave()
{
	model->database().transaction();
	if(model->submitAll()){
		model->database().commit();
	}
	else{
		model->database().rollback();
		showError(model->lastError());
	}
}

void CreateInfo::showError(const QSqlError &err)
{
	QMessageBox::critical(this, tr("An Error Occur"), tr("Error: ")+err.text());
}

