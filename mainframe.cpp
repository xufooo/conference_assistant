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

#include <QTabWidget>
#include <QVBoxLayout>

#include "create_info.h"
#include "designframe.h"
#include "mainframe.h"

MainFrame::MainFrame(QWidget* parent,Qt::WindowFlags f):QDialog(parent,f){

	tab = new QTabWidget(this);
	tab->addTab(new CreateInfo(this),tr("Create"));
	tab->addTab(new DesignFrame(this),tr("Design"));
	
	QVBoxLayout *layout=new QVBoxLayout;
//	layout->setSizeConstraint(QLayout::SetNoConstraint);
	layout->addWidget(tab);
	setLayout(layout);
	setMinimumSize(500,200);
}

//MainFrame::~MainFrame(){}
