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
# Last modified: 2013-07-04 20:57
#
# Should you need to contact me, you can do so by 
# email - mail your message to <xufooo@gmail.com>.
=============================================================================*/

#include <QApplication>
#include <QDialog>
#include <QPixmap>
#include <QPainter>
#include "bc_generator.h"

int main(int argc, char** argv)
{
	QApplication app(argc, argv);
//	QPixmap pix;
	BC_GEN bcg;
	bcg.encode(QString("ABCDEFG12345"));
	bcg.show();
	return app.exec();
//	return 0;
}	
	
