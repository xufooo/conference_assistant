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
# Last modified: 2013-07-07 16:57
#
# Should you need to contact me, you can do so by 
# email - mail your message to <xufooo@gmail.com>.
=============================================================================*/

#include <QWidget>
#include <QDialog>
#include <QSqlError>
class QLineEdit;
class BC_GEN;
class QLabel;
class QPushButton;
class QSqlDatabase;
class QSqlTableModel;
class QTableView;

class CreateInfo:public QWidget
{
	Q_OBJECT

public:
	CreateInfo(QWidget *parent=0);

public slots:
	void doConnect();
	void doInsert();
	void doDelete();
	void doSave();

private:
	void showError(const QSqlError &err);

	QLineEdit *number;
	BC_GEN *barcode;
	QLabel *numberlabel;

	QLabel *namelabel;
	QLineEdit *name;

	QPushButton *p_connect;
	QPushButton *p_insert;
	QPushButton *p_delete;
	QPushButton *p_save;

	QTableView *view;
	QSqlTableModel *model;
};

