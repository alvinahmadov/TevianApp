/**
 *  Copyright (C) 2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 */


#include "Gui/RenderWidget.hpp"

#include <QPainter>
#include <QPaintEvent>
#include <QPainterPath>
#include <QTextBrowser>
#include <QBoxLayout>


namespace Tevian
{
	namespace Gui
	{
		RenderWidget::RenderWidget(QImage* image, QWidget* parent)
				: QWidget(parent),
				  m_image(image)
		{
			setMinimumSize(m_image->size());
		}
		
		RenderWidget::~RenderWidget()
		{
		}
		
		QSize RenderWidget::sizeHint()
		{
			return m_image->size();
		}
		
		const std::unique_ptr<QImage>&
		RenderWidget::image() const
		{ return m_image; }
		
		void RenderWidget::paintEvent(QPaintEvent* event)
		{
			QPainter painter;
			m_mousePos = event->rect().topLeft();
			painter.begin(this);
			painter.setRenderHint(QPainter::Antialiasing);
			painter.drawImage(rect(), *m_image, m_image->rect());
			
			// painter.save();
			
			paint(&painter);
			
			// painter.restore();
			painter.end();
		}
		
	}// namespace Gui
}// namespace Tevian