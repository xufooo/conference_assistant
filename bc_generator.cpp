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
# Last modified: 2013-07-06 12:11
#
# Should you need to contact me, you can do so by 
# email - mail your message to <xufooo@gmail.com>.
=============================================================================*/

#include "bc_generator.h"
#include <iostream>

	char BC_GEN::code39_table[CODE39_SIZE+1]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','-','.',' ','$','/','+','%','*'};
	char BC_GEN::code39_code_table[CODE39_SIZE+1][CODE39_CODE_LEN+1]={"bwbWBwBwb","BwbWbwbwB","bwBWbwbwB","BwBWbwbwb","bwbWBwbwB",\
	"BwbWBwbwb","bwBWBwbwb","bwbWbwBwB","BwbWbwBwb","bwBWbwBwb",\
	"BwbwbWbwB","bwBwbWbwB","BwBwbWbwb","bwbwBWbwB","BwbwBWbwb",\
	"bwBwBWbwb","bwbwbWBwB","BwbwbWBwb","bwBwbWBwb","bwbwBWBwb",\
	"BwbwbwbWB","bwBwbwbWB","BwBwbwbWb","bwbwBwbWB","BwbwBwbWb",\
	"bwBwBwbWb","bwbwbwBWB","BwbwbwBWb","bwBwbwBWb","bwbwBwBWb",\
	"BWbwbwbwB","bWBwbwbwB","BWBwbwbwb","bWbwBwbwB","BWbwBwbwb",\
	"bWBwBwbwb",\
	"bWbwbwBwB","BWbwbwBwb","bWBwbwBwb","bWbWbWbwb","bWbWbwbWb",\
	"bWbwbWbWb","bwbWbWbWb",\
	"bWbwBwBwb"};

BC_GEN::BC_GEN(QWidget* parent,int start_Xposition):QWidget(parent),chksum(0),global_Xposition(start_Xposition){
	encode_buf = new QVector<QLine>();

}

BC_GEN::~BC_GEN(){
	delete encode_buf;
}

int BC_GEN::insertbuf(const QChar & bc)
{
	for(int index=0;index<(CODE39_SIZE+1);++index){//include '*'
		if(bc.toAscii()==code39_table[index]){//found char
			global_Xposition+=INTER_GAP_LEN;//char gap
			for(int i=0;i<CODE39_CODE_LEN;++i){
				switch(code39_code_table[index][i]){
					case 'B':
						for(int j=0;j<WIDE_BAR_LEN_R3;++j){
							encode_buf->append(QLine(global_Xposition,0,global_Xposition,global_Yposition));
							global_Xposition+=1;}
						break;
					case 'b':
						for(int j=0;j<NARROW_BAR_LEN;++j){
							encode_buf->append(QLine(global_Xposition,0,global_Xposition,global_Yposition));
							global_Xposition+=1;}
						break;
					case 'W':
						global_Xposition+=WIDE_BAR_LEN_R3;
						break;
					case 'w':
						global_Xposition+=NARROW_BAR_LEN;
						break;
					default:
						return -2;//code error
				}
			}
			chksum+=index;
			return 1;//insert successful
		}
	}
	return -3;//not found char
}

						
int BC_GEN::encode(QString input, int start_Xposition){
	if(input.size() < LEAST_CHAR)
		return -4;//too short

	encode_buf->clear();//clean these
	chksum=0;
	global_Xposition=start_Xposition-INTER_GAP_LEN;//init position
	int width=lenth_calc(input.size()+ADD_CODE_LEN);
	global_Yposition=width*RATIO_H_W;//init height
	this->resize(width,global_Yposition);

	//code39 start
	if(insertbuf(QChar('*'))!=1) return -5;//start character
	for(int i=0; i< input.size(); ++i){
		if(insertbuf(input.at(i))!=1)
			return -5;
	}
	if(insertbuf(code39_table[chksum%CODE39_SIZE])!=1) return -5;//chksum
	if(insertbuf(QChar('*'))!=1) return -5;//end character

	return 1;//successfully encode
}

void BC_GEN::paintEvent(QPaintEvent *event){
     QPainter painter;
     painter.begin(this);
	 painter.drawLines(*encode_buf);
     painter.end();	
}

