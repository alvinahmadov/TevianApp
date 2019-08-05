/**
 *  Copyright (C) 2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 */


#include "Gui/ViewPort.hpp"

#include <QWheelEvent>
#include <QMouseEvent>
#include <QScrollBar>


namespace Tevian
{
	namespace Gui
	{
		ViewPort::ViewPort(QWidget* parent)
				: QGraphicsView(parent)
		{
			init();
		}
		
		ViewPort::ViewPort(QGraphicsScene* scene, QWidget* parent)
				: QGraphicsView(scene, parent)
		{
			init();
		}
		
		void ViewPort::scale(qreal v)
		{
			QGraphicsView::scale(v, v);
		}
		
		void ViewPort::init()
		{
			setAlignment(Qt::AlignCenter);
			setRenderHint(QPainter::Antialiasing, true);
			setDragMode(QGraphicsView::DragMode::ScrollHandDrag);
			setOptimizationFlags(QGraphicsView::DontAdjustForAntialiasing);
			setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
			setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
			setContextMenuPolicy(Qt::ContextMenuPolicy::ActionsContextMenu);
		}
		
		void ViewPort::wheelEvent(QWheelEvent* event)
		{
			if (horizontalScrollBar()->isEnabled() || verticalScrollBar()->isEnabled())
			{
				setDragMode(QGraphicsView::DragMode::ScrollHandDrag);
			} else
			{
				setDragMode(QGraphicsView::DragMode::NoDrag);
			}
			event->ignore();
		}
	}// namespace Gui
}// namespace Tevian