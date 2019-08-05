/**
 *  Copyright (C) 2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 */
#pragma once


#include <exception>
#include <memory>
#include <array>

#include "Defines.hpp"

#include <QString>
#include <QPoint>
#include <QVector>


namespace Tevian
{
	namespace Math
	{
		/**
		 * Find maximum and minimum points
		 * */
		QVector<QPointF>
		minmax4D(const QVector<QPointF>& points);
		
		inline qreal
		rectArea(const QVector<QPoint>& points)
		{
			auto width = qAbs(points.at(1).x() - points.at(0).x());
			auto height = qAbs(points.at(1).y() - points.at(3).y());
			return width * height;
		}
		
		inline qreal
		rectHeight(const QVector<QPoint>& points)
		{
			auto width = qAbs(points.at(1).x() - points.at(0).x());
			return width;
		}
		
		inline qreal
		rectWidth(const QVector<QPoint>& points)
		{
			auto height = qAbs(points.at(1).y() - points.at(3).y());
			return height;
		}
		
		void pointSort(const QVector<QPoint>& in, QVector<QPoint>& out);
	}
	
	class TEVIAN_API Exception : public std::exception
	{
	public:
		Exception() Q_DECL_NOEXCEPT;
		
		explicit Exception(const char* message) Q_DECL_NOEXCEPT;
		
		explicit Exception(const QString& message) Q_DECL_NOEXCEPT;
		
		virtual ~Exception() Q_DECL_NOEXCEPT Q_DECL_OVERRIDE;
		
		virtual const QString getMessage() const Q_DECL_NOEXCEPT;
	
	protected:
		QString m_message;
	};
	
}

///Implementation
namespace Tevian
{
	///Exception
	
	#ifndef DECL_DETAILED_EXCEPTION
	#define DECL_DETAILED_EXCEPTION(classname) DECL_BASE_DETAILED_EXCEPTION(Exception, classname)
	#endif
	
	DECL_DETAILED_EXCEPTION(IllegalArgumentException)
	
	DECL_DETAILED_EXCEPTION(NotImplementedException)
	
	DECL_DETAILED_EXCEPTION(NullPointerException)
	
	DECL_DETAILED_EXCEPTION(NotFoundException)
	
	DECL_DETAILED_EXCEPTION(AuthException)
	
	DECL_DETAILED_EXCEPTION(DataException)
	
	DECL_DETAILED_EXCEPTION(RangeException)
	
	DECL_DETAILED_EXCEPTION(IOException)
	
}
