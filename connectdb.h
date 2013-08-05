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
# Last modified: 2013-08-05 11:57
#
# Should you need to contact me, you can do so by 
# email - mail your message to <xufooo@gmail.com>.
=============================================================================*/
#ifndef CONNECTDIALOG_H
#define CONNECTDIALOG_H

#include <QDialog>
class QSqlDatabase;
class QLineEdit;

class ConnectDialog:public QDialog
{
	Q_OBJECT
public:
	ConnectDialog(QSqlDatabase *const db, QWidget *parent=0);

	void accept();

private:
	QSqlDatabase *database;

	QLineEdit *host_edit;
	QLineEdit *db_edit;
	QLineEdit *username_edit;
	QLineEdit *password_edit;
};

#endif
