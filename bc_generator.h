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
# Last modified: 2013-07-06 12:12
#
# Should you need to contact me, you can do so by 
# email - mail your message to <xufooo@gmail.com>.
=============================================================================*/

#include <QWidget>
#include <QPainter>
#include <QVector>
#include <QLine>

#define ENCODER   CODE39
#define LEAST_CHAR 5
#define CODE39_SIZE 43	  //code table size 43 char
#define CODE39_CODE_LEN 9 //code lenth 9 char
#define BASE_LEN   1
#define RATIO_1_2  2
#define RATIO_1_3  3

#define NARROW_BAR_LEN BASE_LEN
#define WIDE_BAR_LEN_R2  (BASE_LEN*RATIO_1_2)
#define WIDE_BAR_LEN_R3  (BASE_LEN*RATIO_1_3)
#define INTER_GAP_LEN  BASE_LEN

#define CHAR_LEN_R2 (NARROW_BAR_LEN*6+WIDE_BAR_LEN_R2*2)
#define CHAR_LEN_R3 (NARROW_BAR_LEN*6+WIDE_BAR_LEN_R3*3)
#define CHAR_TOT_LEN_R3 (NARROW_BAR_LEN*6+WIDE_BAR_LEN_R3*3+INTER_GAP_LEN*8)

class BC_GEN: public QWidget
{
	Q_OBJECT;
public:
	BC_GEN(QWidget* parent=0,int start_Xposition=0);
	~BC_GEN();
	inline QVector<QLine>* get_encode_buf(){return encode_buf;}
public slots:
	int encode(QString input,int start_Xposition=0);
protected:
	void paintEvent(QPaintEvent *event);
private:
	int insertbuf(const QChar & bc);

	static char code39_table[CODE39_SIZE+1];//44 char include '*'
	static char code39_code_table[CODE39_SIZE+1][CODE39_CODE_LEN+1];
	QVector<QLine> *encode_buf;
	uint chksum;
	int global_Xposition;
};
