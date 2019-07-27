/**
 *  Copyright (C) 2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 */
#pragma once


#include <memory>

#include "Gui/ViewPort.hpp"

#include <QImage>


class QPainter;

namespace Tevian
{
	namespace Gui
	{
		/**
		 * \author Alvin Ahmadov
		 * \namespace Tevian::Gui
		 * \inherit FaceMarkRenderer
		 *
		 * \brief Base class for FaceMarkRenderer.
		 *
		 * \details
		 * Draws actual image as a background of the widget
		 * and sends painter to child widget to do client
		 * painting.
		 * Holds QImage pointer and deletes it automatically.
		 * */
		class RenderWidget : public QWidget
		{
		Q_OBJECT
		public:
			//ctor
			RenderWidget(QImage* image, QWidget* parent = nullptr);
			
			//dtor
			~RenderWidget() Q_DECL_OVERRIDE;
			
			QSize sizeHint();
			
			/**
			 * \brief Virtual painting function.
			 *
			 *
			 * \c RenderWidget calls method in it's
			 * \c paintEvent method firstly drawing
			 * image.
			 * Then child widget does client painting.
			 * */
			virtual void paint(QPainter*) = 0;
			
			/**
			 * \return Containing image readonly.
			 * */
			const std::unique_ptr<QImage>&
			image() const;
		
		protected:
			void paintEvent(QPaintEvent*) Q_DECL_OVERRIDE;
			
			QPoint getCurrentPos() const
			{
				return m_mousePos;
			}
		
		private:
			
			QPoint m_mousePos;
			
			std::unique_ptr<QImage> m_image;
		};
	}// namespace Gui
}// namespace Tevian