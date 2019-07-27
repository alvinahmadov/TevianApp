/**
 *  Copyright (C) 2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 */
#pragma once


#include "Gui/ImageViewTab.hpp"

#include <QTabWidget>


class QWidget;

class QImage;

namespace Tevian
{
	namespace Gui
	{
		/**
		 * \author Alvin Ahmadov
		 *
		 * \name ImageBook
		 * \brief Container class for loaded images.
		 *
		 *
		 * Shows image widgets as tabbed items.
		 *
		 * Contains a pointer list to loaded images
		 * */
		class TEVIAN_API ImageBook : public QTabWidget
		{
		Q_OBJECT
		public:
			///ctor
			ImageBook(QWidget* parent);
			
			/**
			 * \brief Loads image file to the viewer.
			 * */
			bool loadImage(const QString& file);
		
		public slots:
			
			void showWidgets();
		
		private slots:
			
			bool closeTab();
		
		private:
			void init();
		
		};
	}// namespace Gui
}// namespace Tevian