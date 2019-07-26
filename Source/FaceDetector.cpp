/**
 *  Copyright (C) 2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 */

#include "FaceDetector.hpp"
#include <QtGui/QMatrix>


namespace Tevian
{
	using Client::FaceApi;
	using Details::Ethnicity;
	using Details::Gender;
	
	Details::Demographics::Demographics(qreal mean, qreal variance,
	                                    Gender gender, Ethnicity ethnicity) Q_DECL_NOEXCEPT
			: _ethnicity { ethnicity },
			  _gender { gender }
	{
		++_id;
		_age.mean = mean;
		_age.variance = variance;
	}
	
	quint16 Details::Demographics::_id = 0;
	
	QString Details::ethnicityToStr(Ethnicity ethnicity)
	{
		switch (ethnicity)
		{
			case Ethnicity::Caucasian : return "caucasian";
			case Ethnicity::EastAsian : return "east_indian";
			case Ethnicity::Mongoloid : return "asian";
			case Ethnicity::Negroid : return "black";
			case Ethnicity::Unknown : return "unknown";
		}
	}
	
	QString Details::genderToStr(Gender gender)
	{
		switch (gender)
		{
			case Gender::Male : return "male";
			case Gender::Female : return "female";
			case Gender::Unknown:
			default: return "unknown";
		}
	}
	
	Ethnicity Details::stringToEthnicity(QString ethnicity)
	{
		if (ethnicity.compare("caucasian"))
		{
			return Ethnicity::Caucasian;
		}
		if (ethnicity.compare("east_indian"))
		{
			return Ethnicity::EastAsian;
		}
		if (ethnicity.compare("asian"))
		{
			return Ethnicity::Mongoloid;
		}
		if (ethnicity.compare("black"))
		{
			return Ethnicity::Negroid;
		}
		if (ethnicity.compare("unknown"))
		{
			return Ethnicity::Unknown;
		}
	}
	
	Gender Details::stringToGender(QString ethnicity)
	{
		if (ethnicity.compare("male"))
		{
			return Gender::Male;
		}
		if (ethnicity.compare("female"))
		{
			return Gender::Female;
		}
		if (ethnicity.compare("unknown"))
		{
			return Gender::Unknown;
		}
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
	FaceDetector::getLandmarks()
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
			m_faceApi = new Client::FaceApi(QString(TBACKEND), QString(API_PATH));
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
					//item.second.toJsonValue().toObject()
					m_demographics._age.mean = item.second.toJsonValue().toObject().
							value("mean").toDouble();
					m_demographics._age.variance = item.second.toJsonValue().toObject().
							value("variance").toDouble();
				}
				if (item.first.compare("ethcnicity") == 0)
				{
					m_demographics._ethnicity =
							Details::stringToEthnicity(item.second.toString());
				}
				if (item.first.compare("gender") == 0)
				{
					m_demographics._ethnicity =
							Details::stringToEthnicity(item.second.toString());
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
