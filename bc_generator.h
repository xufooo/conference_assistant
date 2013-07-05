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
# This module is used to generate barcode.
#
# Last modified: 2013-07-05 10:05
#
# Should you need to contact me, you can do so by 
# email - mail your message to <xufooo@gmail.com>.
=============================================================================*/

#include <QString>
#include <QWidget>
#include <QPixmap>

#define ENCODER   CODE39
#define LEAST_CHAR 5
#define BASE_PIX  1
#define RATIO_1_2 2
#define RATIO_1_3 3

#define NARROW_BAR_PIX BASE_PIX
#define WIDE_BAR_PIX_R2  (BASE_PIX*RATIO_1_2)
#define WIDE_BAR_PIX_R3  (BASE_PIX*RATIO_1_3)
#define INTER_GAP_PIX  BASE_PIX

#define CHAR_PIX_R2 (NARROW_BAR_PIX*6+WIDE_BAR_PIX_R2*2+INTER_GAP_PIX*8)
#define CHAR_PIX_R3 (NARROW_BAR_PIX*6+WIDE_BAR_PIX_R3*3+INTER_GAP_PIX*8)

class BC_GEN: public QPixmap
{
	Q_OBJECT;
public:
	BC_GEN(QWidget* parent);
	~BC_GEN();
	int show_barcode(QString barcode);
public slots:
	int encoding(QString input, int type);
public:
	int enc39(QString input, int type);
private:
	char code39_table={'0','1','2','3','4','5','6','7','8','9',\
					   'A','B','C','D','E','F','G','H','I','J',\
					   'K','L','M','N','O','P','Q','R','S','T',\
					   'U','V','W','X','Y','Z',\
					   '-','.',' ','$','/','+','%'};
	char *encode_buf;
	uint chksum;
};
