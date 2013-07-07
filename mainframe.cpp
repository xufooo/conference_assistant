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
# This is the main framework.
#
# Last modified: 2013-07-07 14:39
#
# Should you need to contact me, you can do so by 
# email - mail your message to <xufooo@gmail.com>.
=============================================================================*/

#include <QLineEdit>
#include <QVBoxLayout>

#include "mainframe.h"
#include "bc_generator.h"

MainFrame::MainFrame(QWidget* parent,Qt::WindowFlags f):QDialog(parent,f){

	QVBoxLayout *mainLayout = new QVBoxLayout(this);
	bc_line = new QLineEdit(this);
	barcode = new BC_GEN(this);
	mainLayout->addWidget(bc_line);
	mainLayout->addWidget(barcode);
	setLayout(mainLayout);
	QObject::connect(bc_line,SIGNAL(textChanged(const QString&)),barcode,SLOT(encode(const QString&)));
}

MainFrame::~MainFrame(){
	delete barcode;
	delete bc_line;
}
