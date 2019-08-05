/**
 *  Copyright (C) 2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 */

#include "Gui/ImageBook.hpp"

#include <QDir>
#include <QImageReader>
#include <QTabWidget>
#include <QAction>
#include <QTabBar>


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
			
			auto current = addTab(tab, file.split(QDir::separator()).back());
			tabBar()->setCurrentIndex(current);
			return true;
		}
		
		bool ImageBook::closeTab()
		{
			try
			{
				removeTab(tabBar()->currentIndex());
				return true;
			} catch (...)
			{
				return false;
			}
		}
		
		void ImageBook::init()
		{
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
		}
		
		void ImageBook::showWidgets()
		{
			tabBar()->setVisible(true);
		}
		
	}// namespace Gui
}// namespace Tevian