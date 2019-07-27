/**
 *  Copyright (C) 2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 */
#pragma once


#include <memory>
#include <unordered_map>

#include "Commons.hpp"
#include "Gui/RenderWidget.hpp"
#include "FaceDetector.hpp"

#include <QBrush>
#include <QLabel>
#include <QPixmap>
#include <QtGui/QStaticText>


class QImage;

namespace Tevian
{
	namespace Gui
	{
		/**
		 * \author Alvin Ahmadov
		 * \namespace Tevian::Gui
		 * \brief Draws face detection lines on input image.
		 *
		 * \details
 		 * \c FaceMarkRenderer draws and renders found face landmarks on input images.
 		 * Builds face landmark path starting from upper left pair of points and
 		 * finishing on bottom-right point pair.
 		 * All point calculation process made in \class Annotator, after finishing
 		 * the class sends signal to this class to draw and render image.
		 * */
		class TEVIAN_API DetectionRenderer : public RenderWidget
		{
		Q_OBJECT
		public:
			/**
			 * \brief Drawing mode.
			 * */
			enum PathMode
			{
				CurveMode, LineMode
			};
			
			using Label = std::unordered_map<std::string, QVariant>;
			
		public:
			///ctor.
			DetectionRenderer(QImage* image, QWidget* paernt = nullptr,
			                  Qt::WindowFlags wFlags = Qt::WindowFlags());
			
			QSize sizeHint() const Q_DECL_OVERRIDE;
			
			QPoint center() const;
			
			qreal area();
			
			qreal realPenWidth() const;
			
			void setRealPenWidth(qreal penWidth);
			
			void setPoints(const QVector<QPoint>& points);
			
			void setBox(const std::array<int, 4>& bbox);
			
			void setBox(const QRect& bbox);
			
			bool cleared();
		
		signals:
			
			void clicked();
		
		public slots:
			
			/// Slots to change line type
			void setCurveMode();
			
			void setLineMode();
			
			void setSolidLine();
			
			void setDashLine();
			
			void setDotLine();
			
			void setDashDotLine();
			
			void setDashDotDotLine();
			
			void setCustomDashLine();
			
			void clear(bool c);
		
		private slots:
			
			/**
			 * \brief Paints line
			 *
			 * \details The method is inherited from the base
			 * class. In base class it's pure virtual, only
			 * called on paint event of FacelineRenderer to
			 * draw points and lines on background image
			 * */
			void paint(QPainter*) Q_DECL_OVERRIDE;
		
		private:
			/**
			 * \details It listens for finish signal sent from \c Annotator to start drawing
			 * and called repeatedly if paint event runs.
			 * Draws face bounding box according to landmark points
			 * */
			void drawBox(QPainter* painter);
			
			/**
			 * \brief Draws ponts
			 * */
			void drawLandmarks(QPainter* painter);
		
		private:
			
			QVector<QPoint> m_bounds;
			
			PathMode m_pathMode;
			
			qreal m_penWidth;
			
			int m_pointCount;
			
			int m_pointSize;
			
			std::array<int, 4> m_bbox1;
			
			std::array<int, 4> m_bbox2;
			
			int m_currentPoint;
			
			QVector<QPoint> m_points;
			
			QRect m_boundingBox;
			
			Qt::PenJoinStyle m_joinStyle;
			
			Qt::PenStyle m_penStyle;
			
			bool m_clear;
		};
	}   // namespace Gui
}       // namespace Tevian