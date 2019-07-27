/**
 *  Copyright (C) 2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 */


#include "Gui/DetectionRenderer.hpp"

#include <QDebug>
#include <QPainter>
#include <QMouseEvent>
#include <QStaticText>

namespace Tevian
{
	namespace Gui
	{
		void drawLandmarkPath()
		{
		
		}
		
		void drawBboxPath()
		{
		
		}
		
		DetectionRenderer::DetectionRenderer(QImage* image, QWidget* parent, Qt::WindowFlags flags)
				: RenderWidget(image, parent),
				  m_label { Label() },
				  m_clear { false }
		{
			setWindowFlags(flags);
		}
		
		void DetectionRenderer::drawBox(QPainter* painter)
		{
			if (m_bounds.size() >= 3)
			{
				QPainterPath path;
				painter->setPen(Qt::NoPen);
				path.moveTo(m_bounds.at(0));
				
				if (m_pathMode == LineMode)
				{
					for (int i = 1; i < m_bounds.size(); ++i)
						path.lineTo(m_bounds.at(i));
				} else if (m_pathMode == CurveMode)
				{
					int i = 1;
					while (i + 2 < m_bounds.size())
					{
						path.cubicTo(m_bounds.at(i), m_bounds.at(i + 1), m_bounds.at(i + 2));
						i += 3;
					}
					while (i < m_bounds.size())
					{
						path.lineTo(m_bounds.at(i));
						++i;
					}
				} else
				{
					int i = 1;
					while (i + 2 < m_bounds.size())
					{
						path.quadTo(m_bounds.at(i), m_bounds.at(i + 1));
						i += 2;
					}
					while (i < m_bounds.size())
					{
						path.lineTo(m_bounds.at(i));
						++i;
					}
				}
				// Draw the getPath
				QColor lineColor = Qt::blue;
				
				// The "custom" pen
				if (m_penStyle == Qt::NoPen)
				{
					QPainterPathStroker stroker;
					stroker.setWidth(m_penWidth);
					stroker.setJoinStyle(m_joinStyle);
					stroker.setCapStyle(Qt::PenCapStyle::SquareCap);
					
					QVector<qreal> dashes;
					qreal space = 4;
					dashes << 1 << space
					       << 3 << space
					       << 9 << space
					       << 27 << space
					       << 9 << space
					       << 3 << space;
					stroker.setDashPattern(dashes);
					QPainterPath stroke = stroker.createStroke(path);
					painter->fillPath(stroke, lineColor);
					
				} else
				{
					QPen pen(lineColor, m_penWidth, m_penStyle, Qt::SquareCap, m_joinStyle);
					painter->strokePath(path, pen);
				}
			}
		}
		
		void DetectionRenderer::drawLandmarks(QPainter* painter)
		{
			if (!m_points.empty())
			{
				QColor pencolor = QColor(50, 100, 120, 200);
				QColor brushcolor = QColor(100, 100, 100, 120);
				painter->setPen(pencolor);
				painter->setBrush(brushcolor);
				painter->drawPoints(m_points);
				for (int i = 0; i < m_points.size(); ++i)
				{
					QPoint pos = m_points.at(i);
					painter->drawEllipse(pos.x(), pos.y(),
					                     m_pointSize, m_pointSize);
				}
				painter->setPen(Qt::NoPen);
				painter->setBrush(Qt::NoBrush);
			}
		}
		
		QSize DetectionRenderer::sizeHint() const
		{
			return image()->size();
		}
		
		QPoint DetectionRenderer::center() const
		{
			return QPoint(sizeHint().width() / 2, sizeHint().height() / 2);
		}
		
		qreal DetectionRenderer::area()
		{
			return sizeHint().width() * sizeHint().height();
		}
		
		qreal DetectionRenderer::realPenWidth() const
		{
			return m_penWidth;
		}
		
		void DetectionRenderer::setRealPenWidth(qreal penWidth)
		{
			m_penWidth = penWidth;
		}
		
		void DetectionRenderer::setPoints(const QVector<QPoint>& points)
		{
			if (!points.empty())
			{
				m_points = points;
				m_bounds = Math::minmax4D(m_points);        // Identify face box limits
				m_pointSize = 5;
				m_pointCount = points.size();
				m_currentPoint = -1;
				m_pathMode = LineMode;
				m_penWidth = 5;
				m_penStyle = Qt::SolidLine;
				m_clear = false;
				update();
			}
		}
		
		void DetectionRenderer::setBox(const std::array<int, 4>& bbox)
		{
			m_bbox1 = bbox;
		}
		
		void DetectionRenderer::setBox(const QRect& bbox)
		{
			m_bbox1 = {
					bbox.x(), bbox.y(), bbox.width(), bbox.height()
			};
		}
		
		bool DetectionRenderer::cleared()
		{
			return m_clear;
		}
		
		void DetectionRenderer::setCurveMode()
		{
			m_pathMode = CurveMode;
			update();
		}
		
		void DetectionRenderer::setLineMode()
		{
			m_pathMode = LineMode;
			update();
		}
		
		void DetectionRenderer::setSolidLine()
		{
			m_penStyle = Qt::SolidLine;
			update();
		}
		
		void DetectionRenderer::setDashLine()
		{
			m_penStyle = Qt::DashLine;
			update();
		}
		
		void DetectionRenderer::setDotLine()
		{
			m_penStyle = Qt::DotLine;
			update();
		}
		
		void DetectionRenderer::setDashDotLine()
		{
			m_penStyle = Qt::DashDotLine;
			update();
		}
		
		void DetectionRenderer::setDashDotDotLine()
		{
			m_penStyle = Qt::DashDotDotLine;
			update();
		}
		
		void DetectionRenderer::setCustomDashLine()
		{
			m_penStyle = Qt::NoPen;
			update();
		}
		
		void DetectionRenderer::clear(bool c)
		{
			m_clear = c;
			m_points.clear();
			m_bounds.clear();
			update();
		}
		
		void DetectionRenderer::paint(QPainter* painter)
		{
			if (!m_clear)
			{
				drawLandmarks(painter);
				drawBox(painter);
				clearText();
			}
		}
	}
}


#include <Gui/moc_DetectionRenderer.cpp>