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
# Last modified: 2013-07-08 17:30
#
# Should you need to contact me, you can do so by 
# email - mail your message to <xufooo@gmail.com>.
=============================================================================*/
#ifndef _BC_GENERATOR_H
#define _BC_GENERATOR_H

#include <QWidget>
#include <QPainter>
#include <QPixmap>
#include <QVector>
#include <QLine>

#define ENCODER   CODE39
#define LEAST_CHAR 1
#define CODE39_SIZE 43	  //code table size 43 char, '*' not included
#define CODE39_CODE_LEN 9 //code lenth 9 char
#define ADD_CODE_LEN 3    //start & end '*', check sum '?'
#define BASE_LEN   1
#define RATIO_1_2  2
#define RATIO_1_3  3
#define RATIO_H_W  0.3
#define MARGIN		WIDE_BAR_LEN_R3
#define INIT_WIDTH	200
#define INIT_HEIGHT	(INIT_WIDTH*RATIO_H_W)

#define NARROW_BAR_LEN BASE_LEN
#define WIDE_BAR_LEN_R2  (BASE_LEN*RATIO_1_2)
#define WIDE_BAR_LEN_R3  (BASE_LEN*RATIO_1_3)
#define INTER_GAP_LEN  BASE_LEN

#define CHAR_LEN_R2 (NARROW_BAR_LEN*6+WIDE_BAR_LEN_R2*2)
#define CHAR_LEN_R3 (NARROW_BAR_LEN*6+WIDE_BAR_LEN_R3*3)

class MyHoverPoints;

class BC_GEN: public QWidget
{
	Q_OBJECT
public:
	BC_GEN(QWidget* parent=0, int x=0, int y=0);
	virtual ~BC_GEN();
	inline QVector<QLine>* get_encode_buf(){return encode_buf;}
	inline int lenth_calc(const int& char_num){return (char_num*(CHAR_LEN_R3+INTER_GAP_LEN)-INTER_GAP_LEN);}
	inline void setStartPosition(const int& Xposition,const int& Yposition){
		start_Xposition=Xposition;start_Yposition=Yposition;}
	QPixmap* getPixmap() const {return bc_pix;}
	const QString& getCode() {return code;}

public slots:
	int encode(const QString& input);

protected:
	virtual void paintEvent(QPaintEvent *event);

private:
	int insertbuf(const QChar & bc);

	static char code39_table[CODE39_SIZE+1];//44 char include '*'
	static char code39_code_table[CODE39_SIZE+1][CODE39_CODE_LEN+1];//'\0'

	QVector<QLine> *encode_buf;
	QString code;
	uint chksum;
	int global_Xposition;
	int global_Yposition;
	int start_Xposition;
	int start_Yposition;
	int global_height;

	QPixmap *bc_pix;
};

#endif
