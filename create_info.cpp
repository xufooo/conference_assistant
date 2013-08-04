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

#include <QDebug>

ConnectDialog::ConnectDialog(QSqlDatabase *const db, QWidget *parent):QDialog(parent,Qt::Dialog),database(db)
{
	setWindowTitle(tr("SQL Database Info"));

	QLabel *host_label=new QLabel("HostName:");
	host_edit=new QLineEdit("192.168.1.103");
	QLabel *db_label=new QLabel("DatabaseName:");
	db_edit=new QLineEdit("cookbook");
	QLabel *username_label=new QLabel("UserName:");
	username_edit=new QLineEdit("bcuser");
	QLabel *password_label=new QLabel("PassWord:");
	password_edit=new QLineEdit("bcpass");
	
	QGridLayout *layout=new QGridLayout(this);
	layout->addWidget(host_label,0,0);
	layout->addWidget(host_edit,0,1);
	layout->addWidget(db_label,1,0);
	layout->addWidget(db_edit,1,1);
	layout->addWidget(username_label,2,0);
	layout->addWidget(username_edit,2,1);
	layout->addWidget(password_label,3,0);
	layout->addWidget(password_edit,3,1);

	QPushButton *cancel=new QPushButton("Cancel");
	QPushButton *ok=new QPushButton("OK");
	ok->setDefault(true);
	connect(cancel,SIGNAL(clicked()),this,SLOT(reject()));
	connect(ok,SIGNAL(clicked()),this,SLOT(accept()));

	layout->addWidget(cancel,4,0);
	layout->addWidget(ok,4,1);

	setLayout(layout);
}

void ConnectDialog::accept()
{
	database->setHostName(host_edit->text());
	database->setDatabaseName(db_edit->text());
	database->setUserName(username_edit->text());
	database->setPassword(password_edit->text());
	QDialog::accept();
}

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
	buttonlayout->addWidget(p_connect);
	buttonlayout->addWidget(p_insert);
	buttonlayout->addWidget(p_delete);
	panellayout->addLayout(buttonlayout);
	panellayout->addWidget(barcode);

	view = new QTableView(this);

	mainLayout->addLayout(panellayout);
	mainLayout->addWidget(view);
	setLayout(mainLayout);

	QTimer::singleShot(5,number,SLOT(setFocus()));//focus on number
	
	connect(p_connect,SIGNAL(clicked()),this,SLOT(doConnect()));
	
	/*setup db*/
	if(!QSqlDatabase::drivers().contains("QMYSQL"))
		QMessageBox::critical(this, tr("Unable to load database"), tr("No QMYSQL driver"));
}

void CreateInfo::doConnect()
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

