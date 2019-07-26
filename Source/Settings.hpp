/**
 *  Copyright (C) 2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 */
#pragma once


#include <memory>

#include <QSettings>
#include <QCryptographicHash>


namespace Tevian
{
	struct LoginData;
	
	/// As a singleton
	class Settings : public QSettings
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
		
		QString password();
		
		QByteArray token();
		
		void save();
	
	public slots:
		
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
	};
	
	struct LoginData
	{
		QString m_email;
		
		QString m_password;
		
		QByteArray m_token;
	};
	
	static Settings* g_settingsManager = Settings::getSettingsManager();
}// namespace Tevian
