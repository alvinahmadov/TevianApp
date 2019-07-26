/**
 *  Copyright (C) 2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 */
#pragma once


#include <memory>
#include <any>

#include "Commons.hpp"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QHttpMultiPart>


namespace Tevian
{
	namespace Client
	{
		enum class Operation
		{
			Login,
			Detect,
			Match,
			Databases,
			Persons,
			Photos,
			Users,
			Undefined
		};
		
		class TEVIAN_API RequestData
		{
		public:
			enum ContentType
			{
				NONE,
				JSON,       // application/json
				JPEG,       // image/jpeg
				FORMDATA,   // multipart/form-data
				
				// Not implemented
						PNG,        // image/png
				SVG,        // image/svg+xml
				PLAIN,      // text/plain
				HTML,       // text/html
				CSV         // text/csv
			};
		
		public:
			RequestData();
			
			RequestData&
			setContent(ContentType contentType);
			
			ContentType
			getContentType() const;
			
			const QVariant&
			getContent() const;
			
			QVariant
			getContent(ContentType type);
		
		private:
			void content(ContentType type, QVariant& value);
		
		private:
			ContentType m_contentType;
			
			std::shared_ptr<std::any> m_data;
			
			QVariant m_content;
		};
		
		/**
		 * \addtogroup Client
		 * \brief An abstract API class defining common operations to interact
		 * with REST server.
		 * */
		class TEVIAN_API AbstractApi : public QObject
		{
		public:
			using HttpHeader = QNetworkRequest::KnownHeaders;
			
			enum HttpsHeader
			{
				WWWAuthenticate,
				Authorization,
				ProxyAuthenticate,
				ProxyAuthorization
			};
			
			enum HttpsStatusCodes
			{
				Unauthorized = 401,
				Forbidden = 403,
				ProxyAuthenticationRequired = 407
			};
			
			/**
			 * Must be called after \c setHeader()
			 */
			virtual QNetworkReply*
			post(const QByteArray& data) = 0;
			
			/**
			 * Must be called after \c setHeader()
			 */
			virtual QNetworkReply*
			post(QIODevice* data) = 0;
			
			/**
			 * Must be called after \c setHeader()
			 */
			virtual QNetworkReply*
			post(QHttpMultiPart* data) = 0;
			
			virtual QNetworkReply*
			get() = 0;
		};
	}// namespace Client
}// namespace Tevian