/**
 *  Copyright (C) 2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 */
#pragma once


#include "Defines.hpp"
#include <memory>

#include <QSettings>
#include <QCryptographicHash>


namespace Tevian
{
	struct LoginData;
	struct BackendData;
	
	/// As a singleton
	class TEVIAN_API Settings : public QSettings
	{
	Q_OBJECT
	private:
		Settings(QObject* parent = nullptr);
	
	public:
		using Value = QVariant;
		
		enum Key
		{
			BackgroundColour,
			ForegroundColour,
			WindowWidth,
			WindowHeight,
			CachePath,
			SavePath,
			LoadPath,
			ApiUrl,
			ApiPath,
			Email,
			Password,
			Token
		};
		
		~Settings() Q_DECL_OVERRIDE;
		
		static Settings*
		getSettingsManager(QObject* parent = nullptr);
		
		/**
		 * \brief Set value by enum key
		 * To eliminate typos
		 * */
		void set(Key key, const Value& value);
		
		/**
		 * \brief Get value by enum key.
		 * To eliminate typos
		 * */
		Value get(Key key);
		
		QString email();
		
		QString url();
		
		QString path();
		
		QString password();
		
		QByteArray token();
		
		void save();
	
	public slots:
		
		void setBackendUrl(const QString& backend);
		
		void setApiPath(const QString& path);
		
		void setEmail(const QString& email);
		
		void setPassword(const QString& password);
		
		/**
		 * \brief Sets ready token value.
		 *
		 * If this data set then email and
		 * password maybe ignored.
		 * */
		void setToken(const QString& token);
	
	private:
		static
		std::unique_ptr<Settings> m_instance;
		
		LoginData* m_loginData;
		
		BackendData* m_backendData;
	};
	
	struct LoginData
	{
		QString m_email;
		
		QString m_password;
		
		QByteArray m_token;
	};
	
	struct BackendData
	{
		QString m_url;
		QString m_apiPath;
	};
	
	static Settings* g_settingsManager = Settings::getSettingsManager();
}// namespace Tevian
