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

#include "bc_generator.h"
#include "create_info.h"

QSqlError initDb()
{
	QSqlDatabase db=QSqlDatabase::addDatabase("QMYSQL");
	db.setHostName("192.168.1.103");
	db.setDatabaseName("cookbook");
	db.setUserName("bcuser");
	db.setPassword("bcpass");

	if(!db.open())
		return db.lastError();

	return QSqlError();
}

CreateInfo::CreateInfo(QWidget *parent):QWidget(parent){

	/*setup ui*/
	QHBoxLayout *mainLayout = new QHBoxLayout(this);
	barcode = new BC_GEN;

	QVBoxLayout *panellayout = new QVBoxLayout;

	QHBoxLayout *namelayout = new QHBoxLayout;
	namelabel = new QLabel(tr("Name :"));
	name = new QLineEdit;
	namelayout->addWidget(namelabel);
	namelayout->addWidget(name);
	panellayout->addLayout(namelayout);

	QHBoxLayout *bclayout = new QHBoxLayout;
	numberlabel = new QLabel(tr("NO.  :"));
	number = new QLineEdit(this);
	bclayout->addWidget(numberlabel);
	bclayout->addWidget(number);
	connect(number,SIGNAL(textChanged(const QString&)),barcode,SLOT(encode(const QString&)));
	panellayout->addLayout(bclayout);

	QHBoxLayout *buttonlayout = new QHBoxLayout;
	p_connect = new QPushButton(tr("Connect"));
	p_insert = new QPushButton(tr("Insert"));
	p_delete = new QPushButton(tr("Delete"));
	buttonlayout->addWidget(p_connect);
	buttonlayout->addWidget(p_insert);
	buttonlayout->addWidget(p_delete);
	panellayout->addLayout(buttonlayout);
	panellayout->addWidget(barcode);

	QTableView *view = new QTableView(this);

	mainLayout->addLayout(panellayout);
	mainLayout->addWidget(view);
	setLayout(mainLayout);

	QTimer::singleShot(5,number,SLOT(setFocus()));//focus on number
	
	/*setup db*/
	if(!QSqlDatabase::drivers().contains("QMYSQL"))
		QMessageBox::critical(this,"Unable to load database","No QMYSQL driver");
	QSqlError err=initDb();
	if(err.type()!=QSqlError::NoError){
		showError(err);
		return;
	}

	model=new QSqlTableModel(this);
	model->setEditStrategy(QSqlTableModel::OnManualSubmit);
	model->setTable("limbs");
	
	model->setHeaderData(model->fieldIndex("thing"),Qt::Horizontal,tr("Thing"));
	model->setHeaderData(model->fieldIndex("legs"),Qt::Horizontal,tr("Legs"));
	model->setHeaderData(model->fieldIndex("arms"),Qt::Horizontal,tr("Arms"));

	if(!model->select()){
		showError(model->lastError());
		return;
	}

	view->setModel(model);
	view->setSelectionMode(QAbstractItemView::SingleSelection);


	
}

void CreateInfo::showError(const QSqlError &err)
{
	QMessageBox::critical(this, tr("Unable to initialize Database"), tr("Error initializing database:")+err.text());
}

