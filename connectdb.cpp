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
# Small class, help to connect database.
#
# Last modified: 2013-08-05 12:06
#
# Should you need to contact me, you can do so by 
# email - mail your message to <xufooo@gmail.com>.
=============================================================================*/

#include "connectdb.h"
#include <QSqlDatabase>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>

ConnectDialog::ConnectDialog(QSqlDatabase *const db, QWidget *parent):QDialog(parent,Qt::Dialog),database(db)
{
	setWindowTitle(tr("SQL Database Info"));

	QLabel *host_label=new QLabel("HostName:");
	host_edit=new QLineEdit("192.168.1.103");
	QLabel *db_label=new QLabel("DatabaseName:");
	db_edit=new QLineEdit("cookbook");
	QLabel *username_label=new QLabel("UserName:");
	username_edit=new QLineEdit("cbuser");
	QLabel *password_label=new QLabel("PassWord:");
	password_edit=new QLineEdit("cbpass");
	
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

