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
# Last modified: 2013-11-29 21:06
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
#include <QFileDialog>

#include "bc_generator.h"
#include "create_info.h"
#include "connectdb.h"
#include "xlslib.h"
#include <QDebug>

#define REGNUMBER_POS 2
#define LASTNAME_POS 3
#define MIDDLENAME_POS 4
#define FIRSTNAME_POS 5
#define PHONE_POS 9
#define AFFILIATION_POS 10
#define REGISTRATION_POS 21

CreateInfo::CreateInfo(QWidget *parent):QWidget(parent),model(NULL){

	/*setup ui*/
	QHBoxLayout *mainLayout = new QHBoxLayout(this);
	barcode = new BC_GEN;

	QVBoxLayout *panellayout = new QVBoxLayout;

	QGridLayout *editlayout = new QGridLayout;
//	namelabel = new QLabel(tr("Name :"));
//	name = new QLineEdit;

	numberlabel = new QLabel(tr("NO. :"));
	number = new QLineEdit(this);
//	editlayout->addWidget(namelabel,0,0);
//	editlayout->addWidget(name,0,1);
	editlayout->addWidget(numberlabel,0,0);
	editlayout->addWidget(number,0,1);
	connect(number,SIGNAL(textChanged(const QString&)),barcode,SLOT(encode(const QString&)));
	panellayout->addLayout(editlayout);

	QHBoxLayout *buttonlayout = new QHBoxLayout;
	p_connect = new QPushButton(tr("Connect"));
	p_insert = new QPushButton(tr("Insert"));
	p_delete = new QPushButton(tr("Delete"));
	p_save = new QPushButton(tr("Save"));
	p_export = new QPushButton(tr("Export"));
	buttonlayout->addWidget(p_connect);
	buttonlayout->addWidget(p_insert);
	buttonlayout->addWidget(p_delete);
	buttonlayout->addWidget(p_save);
	buttonlayout->addWidget(p_export);
	panellayout->addLayout(buttonlayout);
	panellayout->addWidget(barcode);

	connect(p_connect,SIGNAL(clicked()),this,SLOT(doConnect()));
	connect(p_insert,SIGNAL(clicked()),this,SLOT(doInsert()));
	connect(p_delete,SIGNAL(clicked()),this,SLOT(doDelete()));
	connect(p_save,SIGNAL(clicked()),this,SLOT(doSave()));
	connect(p_export,SIGNAL(clicked()),this,SLOT(doExport()));

	view = new QTableView(this);

	mainLayout->addLayout(panellayout);
	mainLayout->addWidget(view);
	setLayout(mainLayout);

	QTimer::singleShot(1,number,SLOT(setFocus()));//focus on number
	
	/*setup db*/
	if(!QSqlDatabase::drivers().contains("QSQLITE"))
		QMessageBox::critical(this, tr("Unable to load database"), tr("No QSQLITE driver"));
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
	model->setHeaderData(model->fieldIndex(tr("picurl")),Qt::Horizontal,tr("Registration"));

	if(!model->select()){
		showError(model->lastError());
		return;
	}

	view->setModel(model);
	for(int i=0;i<model->columnCount();++i)
	{
		if((i==REGNUMBER_POS)|(i==LASTNAME_POS)|(i==MIDDLENAME_POS)|(i==FIRSTNAME_POS)|(i==PHONE_POS)|(i==AFFILIATION_POS)|(i==REGISTRATION_POS))
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
	if(model==NULL)
		return;

	if(!model->insertRow(model->rowCount())){
		showError(model->lastError());
			return;
	}
	view->setCurrentIndex(model->index(model->rowCount()-1,0));
}

void CreateInfo::doDelete()
{
	if(model==NULL)
		return;

	if(!model->removeRow(view->currentIndex().row()))
		showError(model->lastError());
}

void CreateInfo::doSave()
{
	if(model==NULL)
		return;

	model->database().transaction();
	if(model->submitAll()){
		model->database().commit();
	}
	else{
		model->database().rollback();
		showError(model->lastError());
	}
}

void CreateInfo::doExport()
{
	if(model==NULL)
		return;

	/* add export function below */
	xlslib_core::workbook *wb_obj=new xlslib_core::workbook;
	xlslib_core::worksheet *ws_obj=wb_obj->sheet("export");

	ws_obj->label(0,0,"Regnumber");
	ws_obj->label(0,1,"Lastname");
	ws_obj->label(0,2,"Middlename");
	ws_obj->label(0,3,"Firstname");
	ws_obj->label(0,4,"Phone");
	ws_obj->label(0,5,"Affiliation");
	ws_obj->label(0,6,"Registration");
	xlslib_core::range *range_obj=ws_obj->rangegroup(0,0,0,6);
	range_obj->cellcolor(xlslib_core::CLR_LITE_GREEN);

	for(int j=0,k=0;j<model->columnCount();++j)
	{
		if((j!=REGNUMBER_POS)&(j!=LASTNAME_POS)&(j!=MIDDLENAME_POS)&(j!=FIRSTNAME_POS)&(j!=PHONE_POS)&(j!=AFFILIATION_POS)&(j!=REGISTRATION_POS))
			continue;
		for(int i=0;i<model->rowCount();++i)
			ws_obj->label(i+1,k,model->index(i,j).data().toString().toStdWString());
		++k;
	}
	QString name=QFileDialog::getSaveFileName(0, "Export to *.xls", tr("export.xls"), tr("Excel Files(*.xls)"));
	QFile file(name);
	if(!file.open(QIODevice::WriteOnly))
		return;
	wb_obj->Dump(name.toStdString());

}

void CreateInfo::showError(const QSqlError &err)
{
	QMessageBox::critical(this, tr("An Error Occur"), tr("Error: ")+err.text());
}

