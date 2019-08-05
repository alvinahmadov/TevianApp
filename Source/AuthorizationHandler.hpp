/**
 *  Copyright (C) 2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 */
#pragma once


#include "BaseApi.hpp"
#include <QAuthenticator>


namespace Tevian
{
	namespace Client
	{
		class TEVIAN_API Authenticator : public QAuthenticator
		{
		public:
			Authenticator();
			
			QString type() const;
			
			void setType(const QString& type);
			
			QByteArray token() const;
			
			void setToken(const QByteArray& token);
		
		private:
			QByteArray m_token;
			
			QString m_type;
		};
		
		/**
		 * \author Alvin Ahmadov
		 * \namespace Tevian::Client
		 * \inherit FaceApi
		 *
		 * \brief Authorization class.
		 *
		 * \details class used to login or signing in
		 * to server.
		 * */
		class TEVIAN_API AuthorizationHandler : public BaseApi
		{
		Q_OBJECT
		public:
			
			/**
			 * ctor
			 *
			 * \param url Url to server's backend.
			 * \param path Option used in getting
			 * servers operations.
			 * \param header One of the \c HttpsHeader values
			 * to authorize.
			 * */
			AuthorizationHandler(const QString& url, QString path = QString(),
			                     HttpsHeader header = HttpsHeader::Authorization);
			
			~AuthorizationHandler();
			
			/**
			 * \brief Set to true if server requires
			 * authorization to use it.
			 * */
			bool requiresAuth() const;
			
			/**
			 * Sets authorization trigger
			 * */
			void setRequiresAuth(bool);
			
			/**
			 * \brief Sends login request to the server.
			 *
			 * \details By getting response it saves
			 * authorization data, thus enabling for child
			 * classes to login without load.
			 * */
			AuthorizationHandler&
			login(const QString& email, const QString& password, QString authType = QString("Bearer"));
			
			QByteArray
			getToken() const;
		
		protected:
			void authorize(QNetworkReply* = nullptr, QAuthenticator* = nullptr);
		
		private:
			bool m_requiresAuth;
			
			bool m_loggedIn;
			
			Authenticator* m_authenticator;
			
			QByteArray m_token;
			
			HttpsHeader m_header;
		};
	}// namespace Client
}// namespace Tevian