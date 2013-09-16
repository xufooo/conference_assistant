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
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QVBoxLayout>
#include <QMessageBox>

#include "mainframe.h"
#include "create_info.h"
#include "designframe.h"
#include "queryframe.h"

MainFrame::MainFrame(QWidget* parent,Qt::WindowFlags f):QMainWindow(parent,f){
	aboutAct=new QAction(tr("&About"),this);
	connect(aboutAct,SIGNAL(triggered()),this,SLOT(about()));

	helpMenu=menuBar()->addMenu(tr("&Help"));
	helpMenu->addAction(aboutAct);

	tab = new QTabWidget;
	tab->addTab(new CreateInfo(this),tr("Create"));
	tab->addTab(new DesignFrame(this),tr("Design"));
	tab->addTab(new QueryFrame(this),tr("Query"));
	
	setCentralWidget(tab);
}

void MainFrame::about()
{
	QMessageBox::about(this,tr("About Conference Assistant"),tr("You can find the source from <git@github.com:xufooo/conference_assistant.git>\n\nCopyright (C) <2013>  <OoO>\n\nThis program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.\n\nThis program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.\n\nYou should have received a copy of the GNU General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>."));
}
