/**
 *  Copyright (C) 2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 */

#include "FaceDetector.hpp"
#include <QMatrix>
#include <QMessageBox>


namespace Tevian
{
	using Client::FaceApi;
	
	Details::Demographics::Demographics(qreal mean, qreal variance,
	                                    QString gender, QString ethnicity) Q_DECL_NOEXCEPT
			: _ethnicity { gender },
			  _gender { ethnicity },
			  _age(Age(mean, variance))
	{
		++_id;
	}
	
	quint16 Details::Demographics::_id = 0;
	
	QString Details::Demographics::getAsText() const
	{
		QString info = QString("Age mean %1, \nAge variance %2, \nEthnicity %3, \nGender %4")
				.arg(_age._mean)
				.arg(_age._variance)
				.arg(_ethnicity)
				.arg(_gender);
		return info;
	}
	
	/// FaceDetector
	FaceDetector::FaceDetector(const QString& file, Client::FaceApi* api)
			: m_file { file },
			  m_reader { new JsonReader },
			  m_face { },
			  m_demographics { Details::Demographics() },
			  m_attributes { Details::Attributes() }
	{
		init(api);
	}
	
	FaceDetector::~FaceDetector()
	{
		delete m_reader;
	}
	
	bool FaceDetector::fetch()
	{
		/// Avoid multiple calls if no data in \c FaceApi set.
		
		if (!m_fetched)
		{
			QJsonDocument document;
			m_faceApi->detect(m_file, document);
			m_reader->setDocument(document);
			m_fetched = true;
		}
		return m_fetched;
	}
	
	void FaceDetector::run()
	{
		if (m_fetched)
		{
			// check if requested values are valid
			if (m_faceApi->getAttributes())
			{
				readAttributes();
			}
			
			if (!FaceData::isFaceEmpty(m_faceApi->getFace()))
			{
				readBoundingBox();
			}
			
			if (m_faceApi->getLandmarks())
			{
				readFacelandmarks();
			}
			
			if (m_faceApi->getDemographics())
			{
				readDemographics();
			}
		}
		return;
	}
	
	const QVector<QPoint>&
	FaceDetector::getLandmarks() const
	{
		return m_face.landmarks;
	}
	
	const std::array<int, 4>&
	FaceDetector::getBox() const
	{
		return m_face.face_bounds;
	}
	
	const Details::Attributes&
	FaceDetector::getAttributes() const
	{
		return m_attributes;
	}
	
	const Details::Demographics&
	FaceDetector::getDemographics() const
	{
		return m_demographics;
	}
	
	void FaceDetector::init(Tevian::Client::FaceApi* api)
	{
		if (!api)
		{
			m_faceApi = api;
		} else
		{
			if (g_settingsManager->url().isEmpty() || g_settingsManager->path().isEmpty())
			{
				QMessageBox::information(reinterpret_cast<QWidget*>(this),
				                         QString("Backend"),
				                         QString("Please set backend and openapi.json path"),
				                         QMessageBox::StandardButton::Ok);
			}
			m_faceApi = new Client::FaceApi(QString(g_settingsManager->url()),
			                                QString(g_settingsManager->path()));
		}
		
		m_faceApi->login(FaceApi::Token);
		m_faceApi->setParent(this);
		m_fetched = false;
	}
	
	void FaceDetector::readAttributes()
	{
		VariantMultiMap data { };
		
		if (m_faceApi->getAttributes())
		{
			data = m_reader->readObject(QString("attributes"));
			for (const auto& item : data)
			{
				m_attributes.emplace(item.first.toStdString(),
				                     item.second.toString().toStdString());
			}
		}
		
	}
	
	void FaceDetector::readBoundingBox()
	{
		VariantMultiMap data { };
		
		if (FaceData::isFaceEmpty(m_faceApi->getFace()))
		{
			data = m_reader->readObject(QString("bbox"));
			int i { };
			for (const auto& item : data)
			{
				m_face.face_bounds[i++] = item.second.toInt();
			}
		}
	}
	
	void FaceDetector::readFacelandmarks()
	{
		VariantMultiMap data { };
		size_t size = 0;
		if (m_faceApi->getLandmarks())
		{
			data = m_reader->readArray(QString("landmarks"));
			size = m_face.landmarkSize = data.size();
			// distance = (x,y)/2 -> map (x, valx), map(x + dist == y, valy)
			const int distance = size / 2;
			int x, y;
			
			auto xiter = data.begin();
			auto yiter = data.begin();
			
			// xiter + dist == y
			std::advance(yiter, distance);
			
			// yiter[dist, end)
			for (; yiter != data.end(); ++yiter, ++xiter)
			{
				x = xiter->second.toInt(),
						y = yiter->second.toInt();
				
				m_face.landmarks.push_back(QPoint(x, y));
			}
		}
	}
	
	void FaceDetector::readDemographics()
	{
		VariantMultiMap data { };
		if (m_faceApi->getDemographics())
		{
			data = m_reader->readObject(QString("demographics"));
			for (const auto& item : data)
			{
				
				if (item.first.compare("age") == 0)
				{
					m_demographics._age._mean = item.second.toJsonValue().toObject().
							value("mean").toDouble();
					m_demographics._age._variance = item.second.toJsonValue().toObject().
							value("variance").toDouble();
				}
				if (item.first.compare("ethnicity") == 0)
				{
					m_demographics._ethnicity = item.second.toString();
				}
				if (item.first.compare("gender") == 0)
				{
					m_demographics._gender = item.second.toString();
				}
			}
		}
	}
	
	void FaceDetector::refetch(bool refetch, const ControlData& controlData)
	{
		m_fetched = !refetch;                                   // Values changed, try to send new request
		m_faceApi->setDemographics(controlData._demographics);  // Get from \c Controls class : checked?
		m_faceApi->setAttributes(controlData._attributes);      // Get from \c Controls class : checked?
		m_faceApi->setLandmarks(controlData._landmarks);        // Get from \c Controls class : checked?
		m_faceApi->setFace1(controlData._face);                 // if enabled, minsize and maxsize automatically
		// will be disabled (API)
		m_faceApi->setMinSize(controlData._min_size);           // empty if face != NULL
		m_faceApi->setMaxSize(controlData._max_size);           // empty if face != NULL
		m_faceApi->setThreshold(controlData._threshold);        // empty if face != NULL
		
		fetch();                                                // update values with new ones,
		// called from parent
	}
	
	/// JsonReader
	JsonReader::JsonReader()
			: m_document { }
	{ }
	
	void JsonReader::setDocument(const QJsonDocument& document)
	{
		m_document = document;
	}
	
	bool JsonReader::isReady()
	{
		return !m_document.isEmpty();
	}
	
	void JsonReader::read(QString jsonKey, QJsonValue& value)
	{
		if (!m_document.isEmpty())
		{
			for (const auto& valueRef : m_document.object())
			{
				for (const auto& d2 : valueRef.toArray())
				{
					if (!d2.toObject().value(jsonKey).isUndefined())
					{
						value = d2.toObject().value(jsonKey);
						
					} else
					{
						// Inform readObject and readArray that key doesn't exist
						value = QJsonValue(QJsonValue::Undefined);
					}
				}
			}
		}
	}
	
	VariantMultiMap
	JsonReader::readArray(QString jsonKey)
	{
		QJsonValue value;
		QJsonArray jsonArray;
		read(jsonKey, value);
		jsonArray = value.toArray();
		
		VariantMultiMap arrayData { };
		
		// if key exists fill data
		if (!value.isUndefined())
		{
			for (const auto& jsonValue : jsonArray)
			{
				auto jsonObject = jsonValue.toObject();
				auto keys = jsonObject.keys();
				
				for (int i = 0; i < jsonObject.size(); ++i)
				{
					auto key = keys.at(i);
					arrayData.emplace(key, jsonObject.value(key));
				}
			}
		} // else return empty map.
		return arrayData;
	}
	
	VariantMultiMap
	JsonReader::readObject(QString jsonKey)
	{
		QJsonValue value;
		read(jsonKey, value);
		QJsonObject jsonObject;
		VariantMultiMap objectData { };
		
		// if key exists fill data
		if (!value.isUndefined())
		{
			jsonObject = value.toObject();
			const auto keys = jsonObject.keys();
			
			for (int i = 0; i < jsonObject.size(); ++i)
			{
				auto key = keys.at(i);
				objectData.emplace(key, jsonObject.value(key));
			}
		} // else return empty map.
		return objectData;
	}
}


#include <moc_FaceDetector.cpp>
