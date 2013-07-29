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
# card design display, print, etc.
#
# Last modified: 2013-07-22 19:47
#
# Should you need to contact me, you can do so by 
# email - mail your message to <xufooo@gmail.com>.
=============================================================================*/

#include "designframe.h"
#include "designscene.h"
#include "bc_graphicsitem.h"
#include "graphicstextitem.h"
#include <QGraphicsView>
#include <QGraphicsTextItem>
#include <QComboBox>
#include <QFontComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPrinter>
#include <QPrintDialog>
#include <QDebug>

DesignFrame::DesignFrame(QWidget *parent):QWidget(parent)
{
	scene = new DesignScene;
	view = new QGraphicsView(scene);
	QHBoxLayout *mainlayout=new QHBoxLayout;
	mainlayout->addWidget(view);
	setLayout(mainlayout);
	bc = new BC_GraphicsItem;
	bc->encode("1234567");
	GraphicsTextItem *ti=new GraphicsTextItem;
	ti->setPlainText("lllaaa");
	scene->addItem(bc);
	scene->addItem(ti);
	view->setScene(scene);
	connect(scene,SIGNAL(sendFixedSize(bool)),this,SLOT(receiveFixedSize(bool)));
	connect(scene,SIGNAL(itemSelected(QGraphicsItem*)),this,SLOT(itemSelected(QGraphicsItem*)));

	scene->setBackground(QPixmap("bg1.jpg"));

	/*createCtrlPanel*/
	QVBoxLayout *layout=new QVBoxLayout;

	QHBoxLayout *fontlayout=new QHBoxLayout;
	fontCombo=new QFontComboBox;
	connect(fontCombo,SIGNAL(currentFontChanged(QFont)),this,SLOT(currentFontChanged(QFont)));

	fontSizeCombo=new QComboBox;
	fontSizeCombo->setEditable(true);
	for(int i=6;i<30;i+=2)
		fontSizeCombo->addItem(QString().setNum(i));
	QIntValidator *validator=new QIntValidator(2,100,this);
	fontSizeCombo->setValidator(validator);
	connect(fontSizeCombo,SIGNAL(currentIndexChanged(QString)),this,SLOT(fontSizeChanged(QString)));

	fontlayout->addWidget(fontCombo);
	fontlayout->addWidget(fontSizeCombo);
	
	QHBoxLayout *buttonlayout=new QHBoxLayout;
	savebutton= new QPushButton(tr("Save"),this);
	printbutton= new QPushButton(tr("Print"),this);
	connect(printbutton,SIGNAL(clicked()),this,SLOT(printScene()));
	buttonlayout->addWidget(savebutton);
	buttonlayout->addWidget(printbutton);

	layout->addLayout(fontlayout);
	layout->addLayout(buttonlayout);

	mainlayout->addLayout(layout);
	/*CtrlPanel*/
}

void DesignFrame::receiveFixedSize(bool fixed)
{
	if(fixed)
		view->setFixedSize(view->sceneRect().width()+2,view->sceneRect().height()+2);
	else
		QWidget::setMaximumSize(QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));
}

void DesignFrame::currentFontChanged(const QFont &)
{
	handleFontChange();
}

void DesignFrame::fontSizeChanged(const QString &)
{
	handleFontChange();
}

void DesignFrame::handleFontChange()
{
	QFont font=fontCombo->currentFont();
	font.setPointSize(fontSizeCombo->currentText().toInt());
	
	scene->setFont(font);
}

void DesignFrame::itemSelected(QGraphicsItem *item)
{
	QGraphicsTextItem *textitem=(QGraphicsTextItem*)item;

	QFont font=textitem->font();
	fontCombo->setCurrentFont(font);
	fontSizeCombo->setEditText(QString().setNum(font.pointSize()));
}

void DesignFrame::printScene()
{
	 QPrinter printer;
	 if (QPrintDialog(&printer).exec() == QDialog::Accepted) {
     	QPainter painter(&printer);
     	painter.setRenderHint(QPainter::Antialiasing);
     	scene->render(&painter);
 }
}
