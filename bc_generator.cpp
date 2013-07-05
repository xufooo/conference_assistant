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
# Last modified: 2013-07-04 21:14
#
# Should you need to contact me, you can do so by 
# email - mail your message to <xufooo@gmail.com>.
=============================================================================*/

#include "bc_generator.h"

BC_GEN::BC_GEN(QWidget* parent):chksum(0),encode_buf(NULL){

}

BC_GEN::~BC_GEN(){
	delete encode_buf;
}

int BC_GEN::encoding(QString input, int type){
	if(input.size() < LEAST_CHAR)
		return -1;//too short
	//enc39 start
	int codesize = (input.size()+3)*type;
	encode_buf = new uchar((input.size()+3)*CHAR_PIX_R3);
	encode_buf[0]='W';//start character
	for(int i=0; i< input.size(); ++i){
		for(int j=0; j< 43; ++j){ 
			if(input.at(i)!= QChar(code39_table[j]))
				continue;
			chksum+=j;
			switch(j){
				case 0:
				encode_buf[i+1]=;


	
