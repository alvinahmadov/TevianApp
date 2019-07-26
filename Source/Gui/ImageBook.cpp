/**
 *  Copyright (C) 2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 */


#include <memory>

#include "Gui/ImageBook.hpp"

#include <QException>
#include <QDir>
#include <QMouseEvent>
#include <QImageReader>
#include <QVBoxLayout>
#include <QTabWidget>
#include <QAction>
#include <QDialogButtonBox>
#include <QTabBar>
#include <QMessageBox>
#include <QPushButton>


namespace Tevian
{
	namespace Gui
	{
		//Tab holder
		ImageBook::ImageBook(QWidget* parent)
				: QTabWidget(parent)
		{
			init();
		}
		
		bool ImageBook::loadImage(const QString& file)
		{
			auto tab = new ImageViewTab(file, this);
			
			tabBar()->setCurrentIndex(count() - 1);
			addTab(tab, file.split(QDir::separator()).back());
			return true;
		}
		
		bool ImageBook::closeTab()
		{
			try
			{
				removeTab(tabBar()->currentIndex());
				return true;
			} catch (QException& e)
			{
				
			}
			return false;
		}
		
		void ImageBook::init()
		{
			// m_controls = new Controls()
			//
			// m_topLayout = new QGridLayout(this);
			// m_widgetLayout = new QHBoxLayout();
			// m_topLayout->addItem(m_widgetLayout, 1, 0);
			
			setTabsClosable(true);
			tabBar()->setMovable(true);
			tabBar()->setShape(QTabBar::RoundedNorth);
			tabBar()->setAutoHide(true);
			tabBar()->setVisible(true);
			
			auto closeAct = new QAction(this);
			closeAct->setShortcut(tr("Ctrl+W"));
			closeAct->setEnabled(true);
			addAction(closeAct);
			
			connect(closeAct, SIGNAL(triggered(bool)), this, SLOT(closeTab()));
			connect(tabBar(), &QTabBar::tabCloseRequested, this, &ImageBook::closeTab);
			// setLayout(m_topLayout);
		}
		
		void ImageBook::showWidgets()
		{
			tabBar()->setVisible(true);
		}
		
	}// namespace Gui
}// namespace Tevian

#include <Gui/moc_ImageBook.cpp>