/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 */
#pragma once


#include "AbstractApi.hpp"

#include <QEventLoop>
#include <QTimer>


namespace Tevian
{
	namespace Client
	{
		
		QUrl operator+(const QUrl& url, QString& path);
		
		class BaseApi : public AbstractApi
		{
		Q_OBJECT
		public:
			using PathMap = QHash<QString, QString>;
		public:
			BaseApi(const QUrl& url, QString path = QString());
			
			/**
			 * \brief Updates current request url path, which built on base url.
			 * */
			BaseApi&
			updateUrl(QString path);
			
			/**
			 * \brief \see updateUrl(const QString& path)
			 * */
			BaseApi&
			updateUrl(const QUrl& path);
			
			BaseApi&
			setHeader(RequestData::ContentType contentType,
			          HttpHeader header = HttpHeader::ContentTypeHeader);
			
			/**
			 * \brief Sends an HTTP POST request to the destination.
			 * Interface method, which automatically handles requests.
			 *
			 * \param data Device data thay will be uploaded to the server.
			 * \param rawHeaderData Optional header to allow handling authorisation
			 * */
			virtual
			QNetworkReply*
			post(const QByteArray& data) Q_DECL_OVERRIDE;
			
			virtual
			QNetworkReply*
			post(QIODevice* data) Q_DECL_OVERRIDE;
			
			virtual
			QNetworkReply*
			post(QHttpMultiPart* data) Q_DECL_OVERRIDE;
			
			virtual
			QNetworkReply*
			get() Q_DECL_OVERRIDE;
			
			void setTimeOut(int timeout);
			
			QVariant content(RequestData::ContentType type);
			
			/**
			 * \brief Returns api endpoint url
			 * */
			QUrl getUrl();
			
			QString
			getPath(Operation key);
			
			QNetworkReply*
			handleResponse(QNetworkReply* response);
		
		protected:
			BaseApi&
			setHeader(QByteArray header, QByteArray value);
			
			QNetworkRequest* request()
			{
				return m_httpRequest;
			}
			
			QNetworkAccessManager* manager()
			{
				return m_httpManager;
			}
		
		private:
			/**
			 * \brief Sets hashmap of the available api paths.
			 *
			 * */
			void fetchApi(QUrl apiPath);
			
			QString
			getPath(QString key);
		
		private:
			// Top url to api
			QUrl m_url;
			
			// Path to api description
			QString m_path;
			
			QNetworkAccessManager* m_httpManager;
			
			QNetworkRequest* m_httpRequest;
			
			RequestData m_requestData;
			
			PathMap m_apiPathMap;
			
			int m_timeOut;
			
		};
	}// namespace Client
}// namespace Tevian