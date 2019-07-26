/**
 *  Copyright (C) 2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 */

#include "Commons.hpp"
#include "AbstractApi.hpp"


namespace Tevian
{
	namespace Client
	{
		RequestData::RequestData()
				: m_contentType(ContentType::NONE),
				  m_content(QVariant())
		{ }
		
		RequestData&
		RequestData::setContent(RequestData::ContentType contentType)
		{
			m_contentType = contentType;
			content(m_contentType, m_content);
			return *this;
		}
		
		const QVariant&
		RequestData::getContent() const
		{
			return m_content;
		}
		
		QVariant
		RequestData::getContent(ContentType type)
		{
			QVariant value;
			content(type, value);
			return value;
		}
		
		void RequestData::content(ContentType type, QVariant& value)
		{
			switch (type)
			{
				case ContentType::JSON : value = "application/json";
					break;
				case ContentType::JPEG : value = "image/jpeg";
					break;
				case ContentType::FORMDATA : value = "multipart/form-data";
					break;
				case ContentType::HTML : value = "text/html; charset=utf-8";
					break;
				case ContentType::PNG : value = "image/png";
					break;
				default: break;
			}
		}
		
		RequestData::ContentType
		RequestData::getContentType() const
		{
			return m_contentType;
		}
	}
}