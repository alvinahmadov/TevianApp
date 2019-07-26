/**
 *  Copyright (C) 2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 */
#pragma once


#include "Defines.hpp"

#include <memory>

#include <QGraphicsView>


namespace Tevian
{
	namespace Gui
	{
		class TEVIAN_API ViewPort : public QGraphicsView
		{
		Q_OBJECT
		public:
			ViewPort(QWidget* parent = Q_NULLPTR);
			
			ViewPort(QGraphicsScene* scene, QWidget* parent = Q_NULLPTR);
			
			void scale(qreal v);
		
		protected:
			void init();
		
		protected slots:
			
			void wheelEvent(QWheelEvent* event) Q_DECL_OVERRIDE;
		};
	}// namespace Gui
}// namespace Tevian