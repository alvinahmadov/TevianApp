/**
 *  Copyright (C) 2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 */


#include "Commons.hpp"
#include "BaseApi.hpp"

#include <QNetworkAccessManager>
#include <QEventLoop>
#include <QTimer>
#include <QException>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QHash>


namespace Tevian
{
	namespace Client
	{
		void fetchPath(BaseApi::PathMap& pathMap,
		               const QHash<QString, QVariant>& paths)
		{
			auto makeKey = [ ](QString path) -> QString
			{
				return path.remove(0, path.lastIndexOf('/') + 1);
			};
			
			for (const auto& path : paths.keys())
			{
				auto key = makeKey(path);
				if (!key.contains("{p_id}")
				    && !key.contains("{d_id}"))
				{
					pathMap[key] = path;
				}
			}
		}
		
		QUrl operator+(const QUrl& url, QString& path)
		{
			QUrl url1 = url.toString().append(path);
			return url1;
		}
		
		BaseApi::BaseApi(const QUrl& _url, QString path)
				: m_url(_url),
				  m_path { path },
				  m_httpManager { new QNetworkAccessManager(this) },
				  m_httpRequest { new QNetworkRequest() },
				  m_apiPathMap { PathMap() },
				  m_timeOut(3000)
		{
			/*
			 * We requested to fetch api paths from api listing
			 * e.g. http://${URL_PATH}/api.json
			 */
			fetchApi(m_url + m_path);
		}
		
		BaseApi&
		BaseApi::updateUrl(QString path)
		{
			m_path = path;
			return updateUrl(m_url + path);
		}
		
		BaseApi&
		BaseApi::updateUrl(const QUrl& url)
		{
			m_httpRequest->setUrl(url);
			return *this;
		}
		
		BaseApi&
		BaseApi::setHeader(RequestData::ContentType contentType, HttpHeader header)
		{
			m_requestData.setContent(contentType);
			m_httpRequest->setHeader(header, m_requestData.getContent());
			return *this;
		}
		
		BaseApi&
		BaseApi::setHeader(QByteArray header, QByteArray value)
		{
			m_requestData.setContent(RequestData::ContentType::NONE);
			m_httpRequest->setRawHeader(header, value);
			return *this;
		}
		
		QNetworkReply*
		BaseApi::post(const QByteArray& data)
		{
			auto response = m_httpManager->post(*m_httpRequest, data);
			handleResponse(response);
			return response;
		}
		
		QNetworkReply*
		BaseApi::post(QIODevice* data)
		{
			auto response = m_httpManager->post(*m_httpRequest, data);
			handleResponse(response);
			return response;
		}
		
		QNetworkReply*
		BaseApi::post(QHttpMultiPart* data)
		{
			auto response = m_httpManager->post(*m_httpRequest, data);
			handleResponse(response);
			return response;
		}
		
		QNetworkReply*
		BaseApi::get()
		{
			auto response = m_httpManager->get(*m_httpRequest);
			handleResponse(response);
			return response;
		}
		
		void BaseApi::fetchApi(QUrl apiPath)
		{
			m_httpRequest->setUrl(apiPath);
			auto response = setHeader(RequestData::JSON, HttpHeader::ContentTypeHeader).get();
			auto doc = QJsonDocument::fromJson(response->readAll());
			if (!doc.isEmpty())
			{
				if (doc.isObject())
				{
					auto paths = doc.object();
					if (!paths["paths"].isNull() && paths["paths"].isObject())
					{
						fetchPath(m_apiPathMap, paths["paths"].toObject().toVariantHash());
					}
				}
			}
		}
		
		QString
		BaseApi::getPath(QString key)
		{
			if (m_apiPathMap.contains(key))
			{
				return m_apiPathMap[key];
			}
		}
		
		QString
		BaseApi::getPath(Operation key)
		{
			QString skey;
			switch (key)
			{
				case Operation::Login: skey = QString("login");
					break;
				case Operation::Detect: skey = QString("detect");
					break;
				case Operation::Match: skey = QString("match");
					break;
				case Operation::Undefined: throw IllegalArgumentException("Argument Illegal");
				case Operation::Databases: throw NotImplementedException("Fetaure not implemented");
				case Operation::Photos: throw NotImplementedException("Fetaure not implemented");
				case Operation::Persons: throw NotImplementedException("Fetaure not implemented");
				case Operation::Users: throw NotImplementedException("Fetaure not implemented");
			}
			return getPath(skey);
		}
		
		void BaseApi::setTimeOut(int timeout)
		{
			m_timeOut = timeout;
		}
		
		QNetworkReply*
		BaseApi::handleResponse(QNetworkReply* response)
		{
			QTimer timer { };
			QEventLoop loop { };
			timer.setSingleShot(true);
			
			connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
			connect(m_httpManager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
			
			timer.start(m_timeOut);
			loop.exec();
			
			if (timer.isActive())
			{
				timer.stop();
				if (response->error() > 0)
				{
					auto reason = response->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();
					qWarning() << (__FUNCTION__) << response->url() << reason;
				}
			} else
			{
				disconnect(m_httpManager, SIGNAL(finished(QNetworkReply * )), &loop, SLOT(quit()));
				loop.quit();
			}
			return response;
		}
		
		QVariant BaseApi::content(RequestData::ContentType type)
		{
			return m_requestData.getContent(type);
		}
		
		QUrl
		BaseApi::getUrl()
		{
			return m_url + m_path;
		}
		
	}// namespace Client
}// namespace Tevian

#include <moc_BaseApi.cpp>