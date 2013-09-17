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
#include <QLabel>
#include <QLineEdit>
#include <QTimer>
#include <QPrinter>
#include <QPrintDialog>
#include <QPrintPreviewWidget>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include "scenesaver.h"
#include <QDebug>

DesignFrame::DesignFrame(QWidget *parent):QWidget(parent),zoomout_count(0),init_zoomout_count(0)
{
	QHBoxLayout *mainlayout=new QHBoxLayout;
	setLayout(mainlayout);

	scene = new DesignScene;
	view = new QGraphicsView;
	bc = new BC_GraphicsItem;
	GraphicsTextItem *name = new GraphicsTextItem;
	name->setObjectName(tr("name"));
	name->setPlainText(tr("Name"));
	regnumber= new GraphicsTextItem;
	regnumber->setObjectName(tr("regnumber"));
	regnumber->setPlainText(tr("000000"));
	scene->addItem(bc);
	scene->addItem(name);
	scene->addItem(regnumber);
	view->setScene(scene);
	connect(scene,SIGNAL(sendFixedSize(bool)),this,SLOT(receiveFixedSize(bool)));
	connect(scene,SIGNAL(itemSelected(QGraphicsItem*)),this,SLOT(itemSelected(QGraphicsItem*)));
	QVBoxLayout *viewlayout=new QVBoxLayout;
	QHBoxLayout *scalelayout=new QHBoxLayout;
	QPushButton *zoomin=new QPushButton(tr("+"));
	zoomin->setFixedSize(20,20);
	QPushButton *zoomout=new QPushButton(tr("-"));
	zoomout->setFixedSize(20,20);
	QPushButton *resetview=new QPushButton(tr("Reset"));
	resetview->setFixedSize(60,20);
	scalelayout->addWidget(zoomout);
	scalelayout->addWidget(resetview);
	scalelayout->addWidget(zoomin);
	connect(zoomin,SIGNAL(clicked()),this,SLOT(zoomIn()));
	connect(zoomout,SIGNAL(clicked()),this,SLOT(zoomOut()));
	connect(resetview,SIGNAL(clicked()),this,SLOT(resetView()));

	viewlayout->addWidget(view);
	viewlayout->addLayout(scalelayout);

	mainlayout->addLayout(viewlayout);


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
	
	QHBoxLayout *bclayout=new QHBoxLayout;
	bc_label=new QLabel(tr("Input BarCode: "));
	bc_line=new QLineEdit;
	connect(bc_line,SIGNAL(textChanged(const QString&)),bc,SLOT(encode(const QString&)));
	connect(bc_line,SIGNAL(textChanged(const QString&)),regnumber,SLOT(setText(const QString&)));
	connect(bc_line,SIGNAL(textChanged(const QString&)),scene,SLOT(update()));
	QTimer::singleShot(0,bc_line,SLOT(setFocus()));//focus on bc_line
	bclayout->addWidget(bc_label);
	bclayout->addWidget(bc_line);

	QHBoxLayout *buttonlayout=new QHBoxLayout;
	openbutton= new QPushButton(tr("Open"));
	connect(openbutton,SIGNAL(clicked()),this,SLOT(open()));
	savebutton= new QPushButton(tr("Save"));
	connect(savebutton,SIGNAL(clicked()),this,SLOT(save()));
	previewbutton= new QPushButton(tr("Preview"));
	connect(previewbutton,SIGNAL(clicked()),this,SLOT(previewScene()));
	printbutton= new QPushButton(tr("Print"));
	connect(printbutton,SIGNAL(clicked()),this,SLOT(printScene()));
	buttonlayout->addWidget(openbutton);
	buttonlayout->addWidget(savebutton);
	buttonlayout->addWidget(previewbutton);
	buttonlayout->addWidget(printbutton);

	layout->addLayout(fontlayout);
	layout->addLayout(bclayout);
	layout->addLayout(buttonlayout);

	mainlayout->addLayout(layout);
	/*CtrlPanel*/
}

void DesignFrame::receiveFixedSize(bool fixed)
{
	if(fixed){
//		view->setFixedSize(view->sceneRect().width()+2,view->sceneRect().height()+2);
		view->setMaximumSize(view->sceneRect().width()+2,view->sceneRect().height()+2);
//		emit toResize(view->size());
		init_zoomout_count=0;
		resetView();
	}
	else
	{
		view->setMaximumSize(QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));
		init_zoomout_count=(view->sceneRect().width()/view->size().width())<(view->sceneRect().height()/view->size().height()) ? (view->sceneRect().width()/view->size().width())/SCALE_RATIO : (view->sceneRect().height()/view->size().height())/SCALE_RATIO;
		resetView();
	}	
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
	fontCombo->blockSignals(true);
	fontCombo->setCurrentFont(font);
	fontCombo->blockSignals(false);
	fontSizeCombo->blockSignals(true);
	fontSizeCombo->setEditText(QString().setNum(font.pointSize()));
	fontSizeCombo->blockSignals(false);
}

void DesignFrame::printScene()
{
	 QPrinter printer;
	 if (QPrintDialog(&printer).exec() == QDialog::Accepted) {
     	QPainter painter(&printer);
     	painter.setRenderHint(QPainter::SmoothPixmapTransform);
		if(scene->isBackground())
     		scene->render(&painter);
		else
		{
			QPixmap white(scene->width(),scene->height());
			white.fill();
			scene->setBackground(white);
			scene->render(&painter);
			scene->setBackground(QPixmap());
		}
	 }
}

void DesignFrame::previewScene()
{
	QPrintPreviewWidget *preview=new QPrintPreviewWidget;
	preview->setZoomMode(QPrintPreviewWidget::FitInView);
	connect(preview,SIGNAL(paintRequested(QPrinter*)),this,SLOT(doPreview(QPrinter*)));
	preview->show();
}

void DesignFrame::doPreview(QPrinter *printer)
{
   	QPainter painter(printer);
   	painter.setRenderHint(QPainter::SmoothPixmapTransform);
	if(scene->isBackground())
   		scene->render(&painter);
	else
	{
		QPixmap white(scene->width(),scene->height());
		white.fill();
		scene->setBackground(white);
		scene->render(&painter);
		scene->setBackground(QPixmap());
	}
}

int DesignFrame::open()
{
	QString fileName=QFileDialog::getOpenFileName(this,tr("Open File"),QDir::currentPath(), tr("All Files (*.sav *.png *.jpg *.bmp)"));
	
	if(!fileName.isEmpty()){
		if(fileName.endsWith(".sav"))
			return SceneSaver::restore(this,fileName);
		QPixmap image(fileName);
		if(image.isNull()){
			QMessageBox::information(this,tr("Failure"),tr("Cannot load %1.").arg(fileName));
			return 0;
		}
		foreach(QGraphicsItem *item,scene->items())
			item->setPos(0,0);
		scene->setBackground(image);
	}
	return 1;
}

void DesignFrame::save()
{
	if(SceneSaver::save(scene))
		QMessageBox::information(this,tr("Saved"),tr("Save Successfully."));
}

void DesignFrame::setBC(BC_GraphicsItem *newbc)
{
	bc=newbc;
	connect(bc_line,SIGNAL(textChanged(const QString&)),bc,SLOT(encode(const QString&)));
}

void DesignFrame::zoomIn()
{
	view->scale(SCALE_RATIO, SCALE_RATIO);
	zoomout_count+=1;
}

void DesignFrame::zoomOut()
{
	if(zoomout_count)
	{
		view->scale(1/SCALE_RATIO, 1/SCALE_RATIO);
		zoomout_count-=1;
	}
}

void DesignFrame::resetView()
{
	view->resetMatrix();
	zoomout_count=init_zoomout_count;
}
