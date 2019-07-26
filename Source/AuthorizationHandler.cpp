/**
 *  Copyright (C) 2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 */

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include "AuthorizationHandler.hpp"
#include <chrono>
#include <Settings.hpp>

namespace Tevian
{
	namespace Client
	{
		AuthorizationHandler::AuthorizationHandler(const QString& url, QString path, HttpsHeader header)
				: BaseApi(url, path),
				  m_loggedIn { false },
				  m_authenticator { new Authenticator() },
				  m_token { },
				  m_header { header }
		{
		}
		
		AuthorizationHandler::~AuthorizationHandler()
		{
			delete m_authenticator;
		}
		
		bool AuthorizationHandler::requiresAuth() const
		{
			return m_requiresAuth;
		}
		
		void AuthorizationHandler::setRequiresAuth(bool requires)
		{
			m_requiresAuth = requires;
		}
		
		AuthorizationHandler&
		AuthorizationHandler::login(LoginType loginType, QString authType)
		{
			if (loginType == LoginType::Token)
			{
				if (!g_settingsManager->token().isEmpty())
				{
					return login(g_settingsManager->token(), authType);
				} else
				{
					return login(LoginType::Account);
				}
			}
			if (loginType == LoginType::Account)
			{
				if (!g_settingsManager->email().isEmpty() &&
				    !g_settingsManager->password().isEmpty())
				{
					return login(g_settingsManager->email(), g_settingsManager->password(), authType);
				} else
					qWarning() << "Email or/and passwords are empty.";
			}
			return *this;
		}
		
		AuthorizationHandler&
		AuthorizationHandler::login(const QString& email, const QString& password, QString authType)
		{
			if (!m_loggedIn)
			{
				m_authenticator->setUser(email);
				m_authenticator->setPassword(password);
				
				auto loginData = QByteArray(QJsonDocument({
						                                          { "email",    QJsonValue(m_authenticator->user()) },
						                                          { "password", QJsonValue(m_authenticator->password()) }
				                                          }).toJson(QJsonDocument::Compact));
				
				updateUrl(getPath(Operation::Login));
				auto response = setHeader(RequestData::ContentType::JSON,
				                          HttpHeader::ContentTypeHeader)
						.post(loginData);
				
				auto _token = QJsonDocument::fromJson(response->readAll()).object().value("data")
				                                                          .toObject().value("access_token")
				                                                          .toString();
				
				m_authenticator->setType(authType);
				m_authenticator->setToken(_token.toLocal8Bit());
				
				if (m_token.isEmpty())
				{
					m_token = m_authenticator->token();
					m_token.prepend(' ').prepend(m_authenticator->type().toLocal8Bit());
				}
				if (g_settingsManager->token().isEmpty())
				{
					g_settingsManager->setToken(m_token);
				}
				
				setRequiresAuth(true);
				m_loggedIn = true;
			}
			return *this;
		}
		
		AuthorizationHandler&
		AuthorizationHandler::login(const QByteArray& token, QString authType)
		{
			if (!m_loggedIn)
			{
				m_authenticator->setType(authType);
				m_authenticator->setToken(token);
				
				if (m_token.isEmpty())
				{
					m_token = m_authenticator->token();
					m_token.prepend(' ').prepend(m_authenticator->type().toLocal8Bit());
				}
				
				setRequiresAuth(true);
				m_loggedIn = true;
			}
			return *this;
		}
		
		QByteArray
		AuthorizationHandler::getToken() const
		{
			return m_token;
		}
		
		void
		AuthorizationHandler::authorize(QNetworkReply*, QAuthenticator*)
		{
			QByteArray rawHeader;
			switch (m_header)
			{
				case BaseApi::HttpsHeader::WWWAuthenticate      : rawHeader = QByteArray("WWW-Authenticate");
					break;
				case BaseApi::HttpsHeader::Authorization        : rawHeader = QByteArray("Authorization");
					break;
				case BaseApi::HttpsHeader::ProxyAuthenticate    : rawHeader = QByteArray("Proxy-Authenticate");
					break;
				case BaseApi::HttpsHeader::ProxyAuthorization   : rawHeader = QByteArray("Proxy-Authorization");
					break;
			}
			
			setHeader(rawHeader, m_token);
		}
		
		Authenticator::Authenticator()
				: QAuthenticator()
		{ }
		
		QString Authenticator::type() const
		{
			return m_type;
		}
		
		void Authenticator::setType(const QString& type)
		{
			if (!type.isEmpty())
			{
				m_type = type;
			}
		}
		
		QByteArray Authenticator::token() const
		{
			return m_token;
		}
		
		void Authenticator::setToken(const QByteArray& token)
		{
			if (!token.isEmpty())
			{
				m_token = token;
			}
		}
	}// namespace Client
}// namespace Tevian

#include <moc_AuthorizationHandler.cpp>