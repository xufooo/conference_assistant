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
# This Project is aimed for conference holding.
#
# Last modified: 2013-07-07 12:23
#
# Should you need to contact me, you can do so by 
# email - mail your message to <xufooo@gmail.com>.
=============================================================================*/

#include <QApplication>
#include "mainframe.h"
#include "bc_generator.h"
#include <QWidget>
#include <QPixmap>
#include "designframe.h"
#include "bc_graphicsitem.h"

int main(int argc, char** argv)
{
	QApplication app(argc, argv);
//	BC_GEN bcg;
//	bcg.encode(QString("abcdefGHIJK012345"));
//	bcg.show();
	MainFrame mainframe;
	mainframe.show();
	
//	QWidget main;
//	ArthurFrame a;
//	BC_GEN bcg(&a,true);
//	bcg.encode(QString("abcdefGHIJK012345"));
//	HoverPoints point(&a,HoverPoints::RectangleShape);
//	a.show();
//	main.show();

//	XFormWidget *xfw=new XFormWidget(0);
//	xfw->show();

//	DesignFrame df;
//	df.show();

	return app.exec();
//	return 0;
}	
	
