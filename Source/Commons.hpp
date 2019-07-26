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
		Q_CONSTEXPR inline
		bool operator<(const QPoint& p1, const QPoint& p2)
		{
			return (p1.x() < p2.x() && p2.x() < p2.y());
		}
		
		Q_CONSTEXPR inline
		bool operator<=(const QPoint& p1, const QPoint& p2)
		{
			return (p1.x() <= p2.x() && p2.x() <= p2.y());
		}
		
		Q_CONSTEXPR inline
		bool operator>(const QPoint& p1, const QPoint& p2)
		{
			return !operator<(p1, p2);
		}
		
		Q_CONSTEXPR inline
		bool operator>=(const QPoint& p1, const QPoint& p2)
		{
			return !operator<=(p1, p2);
		}
		
		Q_CONSTEXPR inline
		const QPoint&
		min(const QPoint& p1, const QPoint& p2)
		{
			if (p1 < p2)
			{
				return p1;
			} else
			{
				return p2;
			}
		}
		
		/**
		 * Find maximum and minimum points
		 * */
		const QVector<QPoint>
		minmax4D(QVector<QPoint>& points);
		
		Q_CONSTEXPR inline
		const QPoint&
		max(const QPoint& p1, const QPoint& p2)
		{
			if (p1 > p2)
			{
				return p1;
			} else
			{
				return p2;
			}
		}
		
		void pointSort(const QVector<QPoint>& in, QVector<QPoint>& out);
	}
	
	template<
			typename T,
			typename D = std::default_delete<T>
	        >
	using Unique = std::unique_ptr<T, D>;
	
	template<typename T>
	using Shared = std::shared_ptr<T>;
	
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
	
	struct MethodCallbackBase
	{
	};
	
	/**
	 * \author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
	 * \class MethodCallback
	 * \brief Allows holder structures to call its elements' methods.
	 * \tparam C Type of class of callback method.
	 * \tparam R Return type of method.
	 * \tparam Args Variable argument types
	 * */
	template<
			typename C,
			typename R,
			typename ... Args
	        >
	class TEVIAN_API MethodCallback : public MethodCallbackBase
	{
	public:
		using pointer_t   = C*;
		using reference_t = C&;
		
		typedef R (C::*method_t)(Args ... args);
	
	public:
		explicit MethodCallback(method_t methodPtr);
		
		explicit MethodCallback(method_t methodPtr, pointer_t objectPtr);
		
		~MethodCallback();
		
		R operator()(pointer_t objectPtr, Args&& ... args);
		
		R operator()(Args&& ... args);
	
	private:
		method_t m_methodPtr;
		
		pointer_t m_objectPtr;
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
	
	///MethodCallback
	template<
			class T,
			class Return,
			typename ... Args>
	MethodCallback<T, Return, Args...>::MethodCallback(MethodCallback::method_t methodPtr)
			: m_methodPtr { methodPtr },
			  m_objectPtr { nullptr }
	{
		
		QString m { };
	}
	
	template<
			class T,
			class Return,
			typename ... Args>
	MethodCallback<T, Return, Args...>::MethodCallback(MethodCallback::method_t methodPtr, MethodCallback::pointer_t objectPtr)
			: m_methodPtr { methodPtr },
			  m_objectPtr { objectPtr }
	{ }
	
	template<
			class T,
			class Return,
			typename ... Args>
	MethodCallback<T, Return, Args...>::~MethodCallback()
	{ }
	
	template<
			class T,
			class Return,
			typename ... Args>
	Return MethodCallback<T, Return, Args...>::operator()(MethodCallback::pointer_t objectPtr, Args&& ... args)
	{
		return (objectPtr->*m_methodPtr)(std::forward<Args>(args)...);
	}
	
	template<
			class T,
			class Return,
			typename ... Args>
	Return MethodCallback<T, Return, Args...>::operator()(Args&& ... args)
	{
		if (m_objectPtr != nullptr)
		{
			return (m_objectPtr->*m_methodPtr)(std::forward<Args>(args)...);
		}
	}
}