/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 */
#include "Settings.hpp"
#include <memory>
#include <QDebug>


namespace Tevian
{
	std::unique_ptr<Settings>
			Settings::m_instance { };
	
	Settings::Settings(QObject* parent)
			: QSettings(parent),
			  m_loginData { new LoginData },
			  m_backendData { new BackendData }
	{
	}
	
	Settings::~Settings()
	{
		delete m_loginData;
		delete m_backendData;
	}
	
	Settings*
	Settings::getSettingsManager(QObject* parent)
	{
		if (!m_instance)
		{
			m_instance = std::unique_ptr<Settings>(new Settings(parent));
		}
		
		return m_instance.get();
	}
	
	void Settings::set(Key key, const Value& value)
	{
		switch (key)
		{
			case Key::BackgroundColour  : m_instance->setValue("window/background-colour", value);
				break;
			case Key::ForegroundColour  : m_instance->setValue("window/foreground-colour", value);
				break;
			case Key::WindowWidth       : m_instance->setValue("window/width", value);
				break;
			case Key::WindowHeight      : m_instance->setValue("window/height", value);
				break;
			case Key::CachePath         : m_instance->setValue("data/cache-dir", value);
				break;
			case Key::SavePath          : m_instance->setValue("data/save-dir", value);
				break;
			case Key::LoadPath          : m_instance->setValue("data/load-dir", value);
				break;
			case Key::ApiUrl            : m_instance->setValue("backend/url", value);
				break;
			case Key::ApiPath           : m_instance->setValue("backend/path", value);
				break;
			case Key::Email             : m_instance->setValue("login/email", value);
				break;
			case Key::Password          : m_instance->setValue("login/password", value);
				break;
			case Key::Token             : m_instance->setValue("login/token", value);
				break;
		}
	}
	
	Settings::Value
	Settings::get(Key key)
	{
		switch (key)
		{
			case Key::BackgroundColour  : return m_instance->value("window/background-colour");
			case Key::ForegroundColour  : return m_instance->value("window/foreground-colour");
			case Key::WindowWidth       : return m_instance->value("window/width");
			case Key::WindowHeight      : return m_instance->value("window/height");
			case Key::CachePath         : return m_instance->value("data/cache-dir");
			case Key::SavePath          : return m_instance->value("data/save-dir");
			case Key::LoadPath          : return m_instance->value("data/load-dir");
			case Key::ApiUrl            : return m_instance->value("backend/url");
			case Key::ApiPath           : return m_instance->value("backend/path");
			case Key::Email             : return m_instance->value("login/email");
			case Key::Password          : return m_instance->value("login/password");
			case Key::Token             : return m_instance->value("login/token");
			default: break;
		}
	}
	
	void Settings::setBackendUrl(const QString& backend)
	{
		m_backendData->m_url = backend;
	}
	
	void Settings::setApiPath(const QString& path)
	{
		m_backendData->m_apiPath = path;
	}
	
	void Settings::setEmail(const QString& email)
	{
		m_loginData->m_email = email;
	}
	
	void Settings::setPassword(const QString& password)
	{
		m_loginData->m_password = password;
	}
	
	void Settings::setToken(const QString& token)
	{
		m_loginData->m_token = token.toLocal8Bit();
	}
	
	QString
	Settings::email()
	{
		if (m_loginData->m_email.isEmpty())
		{
			m_loginData->m_email = get(Key::Email).toString();
		}
		return m_loginData->m_email;
	}
	
	QString Settings::url()
	{
		if (m_backendData->m_url.isEmpty())
		{
			m_backendData->m_url = get(Key::ApiUrl).toString();
		}
		return m_backendData->m_url;
	}
	
	QString Settings::path()
	{
		
		if (m_backendData->m_apiPath.isEmpty())
		{
			m_backendData->m_apiPath = get(Key::ApiPath).toString();
		}
		return m_backendData->m_apiPath;
	}
	
	QString
	Settings::password()
	{
		if (m_loginData->m_password.isEmpty())
		{
			m_loginData->m_password = get(Key::Password).toString();
		}
		return m_loginData->m_password;
	}
	
	QByteArray Settings::token()
	{
		if (m_loginData->m_token.isEmpty())
		{
			m_loginData->m_token = get(Key::Token).toByteArray();
		}
		return m_loginData->m_token;
	}
	
	void Settings::save()
	{
		if (!m_loginData->m_email.isEmpty())
		{
			set(Key::Email, m_loginData->m_email);
		}
		
		if (!m_loginData->m_password.isEmpty())
		{
			set(Key::Password, m_loginData->m_password);
		}
		
		if (!m_loginData->m_token.isEmpty())
		{
			set(Key::Token, m_loginData->m_token);
		}
		
		if (!m_backendData->m_url.isEmpty())
		{
			set(Key::ApiUrl, m_backendData->m_url);
		}
		
		if (!m_backendData->m_apiPath.isEmpty())
		{
			set(Key::ApiPath, m_backendData->m_apiPath);
		}
		
		sync();
	}
	
}


#include <moc_Settings.cpp>