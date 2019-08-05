/**
 *  Copyright (C) 2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 */



#include "Commons.hpp"

#include <iostream>

#include <QString>


namespace Tevian
{
	namespace Math
	{
		
		void pointSort(const QVector<QPoint>& in, QVector<QPoint>& out)
		{
			auto pmin = [ ](const QPoint& p1, const QPoint& p2)
			{
				auto x1 = p1.manhattanLength();
				auto x2 = p2.manhattanLength();
				
				return x1 < x2 ? p1 : p2;
			};
			
			for (int i = 0; i < in.size() - 1; ++i)
			{
				out.push_back(pmin(in.at(i), in.at(i + 1)));
			}
			
		}
		
		QPointF& operator-=(QPointF& p, int i)
		{
			p.setX(p.x() - i);
			p.setY(p.y() - i);
			return p;
		}
		
		QPointF& operator-=(QPointF& p, float f)
		{
			p.setX(p.x() - f);
			p.setY(p.y() - f);
			return p;
		}
		
		QPointF& operator+=(QPointF& p, int i)
		{
			p.setX(p.x() + i);
			p.setY(p.y() + i);
			return p;
		}
		
		QPointF& operator+=(QPointF& p, float f)
		{
			p.setX(p.x() + f);
			p.setY(p.y() + f);
			return p;
		}
		
		QVector<QPointF>
		minmax4D(const QVector<QPointF>& points)
		{
			auto start = points.at(points.size() - 1);
			// Vertical and horizontal limits (y0 -> ymax and x0 -> xmax)
			QPointF vMin = start, vMax = start,
					hMin = start, hMax = start;
			QVector<QPointF> range { };
			
			auto minmaxPoint = [ & ](const QPointF& p1, const QPointF p2)
			{
				// Test for x
				if (p1.x() > p2.x())
				{
					if (hMax.x() < p1.x())
					{
						hMax = p1;
					}
					if (hMin.x() > p2.x())
					{ hMin = p2; }
				}
				
				// Test for y
				if (p1.y() > p2.y())
				{
					if (vMax.y() < p1.y())
					{ vMax = p1; }
					if (vMin.y() > p2.y())
					{ vMin = p2; }
				}
			};
			
			for (int i = 0; i < points.size() - 1; ++i)
			{
				minmaxPoint(points.at(i), points.at(i + 1));
			}
			
			range.push_back(QPointF(hMin.x(), vMin.y()));    // topleft
			range.push_back(QPointF(hMax.x(), vMin.y()));    // topright
			range.push_back(QPointF(hMax.x(), vMin.y()));    // bottomright
			range.push_back(QPointF(hMax.x(), vMax.y()));    // bottomleft
			range.push_back(QPointF(hMax.x(), vMax.y()));    // topleft
			range.push_back(QPointF(hMin.x(), vMax.y()));
			range.push_back(QPointF(hMin.x(), vMax.y()));
			range.push_back(QPointF(hMin.x(), vMin.y()));
			
			return range;
		}
	}
	
	Exception::Exception() Q_DECL_NOEXCEPT
			: m_message { }
	{ }
	
	Exception::Exception(const char* message) Q_DECL_NOEXCEPT
			: m_message { message }
	{ }
	
	Exception::Exception(const QString& message) Q_DECL_NOEXCEPT
			: m_message { message }
	{ }
	
	Exception::~Exception() Q_DECL_NOEXCEPT
	{ }
	
	inline const QString
	Exception::getMessage() const Q_DECL_NOEXCEPT
	{
		return m_message;
	}
	
	#ifndef DEF_DETAILED_EXCEPTION
	#define DEF_DETAILED_EXCEPTION(classname) DEF_BASE_DETAILED_EXCEPTION(Exception, classname)
	
	DEF_DETAILED_EXCEPTION(IllegalArgumentException)
	
	DEF_DETAILED_EXCEPTION(NullPointerException)
	
	DEF_DETAILED_EXCEPTION(NotImplementedException)
	
	DEF_DETAILED_EXCEPTION(NotFoundException)
	
	DEF_DETAILED_EXCEPTION(AuthException)
	
	DEF_DETAILED_EXCEPTION(DataException)
	
	DEF_DETAILED_EXCEPTION(RangeException)
	
	DEF_DETAILED_EXCEPTION(IOException)
	
	#endif
}
