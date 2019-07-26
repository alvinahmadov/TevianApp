/**
 *  Copyright (C) 2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 */
#pragma once


#include <memory>
#include <unordered_map>

#include "Commons.hpp"
#include "FaceApi.hpp"
#include "Settings.hpp"
#include <QDebug>

#include <QHash>
#include <QRect>
#include <QPoint>
#include <QString>
#include <QVector>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>


namespace Tevian
{
	namespace Details
	{
		/// FaceDetector parameters
		
		/**
		 * Ethnicity parameter for
		 * demographics option.
		 */
		enum class Ethnicity
		{
			Unknown,
			Caucasian,
			Mongoloid,
			Negroid,
			EastAsian
		};
		
		QString ethnicityToStr(Ethnicity ethnicity);
		
		Ethnicity stringToEthnicity(QString ethnicity);
		
		/**
		 * Gender parameter for
		 * demographics option.
		 */
		enum class Gender
		{
			Male,
			Female,
			Unknown
		};
		
		QString genderToStr(Gender gender);
		
		Gender stringToGender(QString ethnicity);
		
		
		/**
		 * \brief Used to save parameters like the
		 * number and the values of landmark keys,
		 * face bounding box coordinates.
		 * */
		struct FaceParameters
		{
			//! Number of landmark points
			int landmarkSize;
			
			//! Rectangle of face coordinates
			FaceData::face_array face_bounds;
			
			//! Vector of face points
			QVector<QPoint> landmarks;
			
			FaceParameters() = default;
		};
		
		struct Demographics
		{
			explicit Demographics(qreal mean = 0.0, qreal variance = 0.0,
			                      Gender gender = Gender::Unknown,
			                      Ethnicity ethnicity = Ethnicity::Unknown) Q_DECL_NOEXCEPT;
			
			//! Ethnicity of person
			Ethnicity _ethnicity;
			
			//! Gender of person
			Gender _gender;
			
			//! id number of person.
			//! Must be unique
			static quint16 _id;
			
			//! Age of detected person
			union Age
			{
				qreal mean;
				qreal variance;
			} _age;
			
			inline QString getEthnicity() const
			{
				return ethnicityToStr(_ethnicity);
			}
			
			inline QString getGender() const
			{
				return genderToStr(_gender);
			}
		};
		
		using Attributes = std::unordered_map<std::string, std::string>;
		
	}
	
	class JsonReader;
	
	using VariantMultiMap = std::multimap<QString, QVariant>;
	
	/**
	 * \author Alvin Ahmadov
	 * \namespace Tevian
	 * \ingroup Base
	 *
	 * \brief Controller class to bind read data from rest server
	 * to projects Gui module to do image rendering.
	 * To implement classes that read data from server like
	 * /api/persons, /api/databases etc. may be needed to derive
	 * from this class.
	 * Maybe needed to do a base class for Annotator?
	 * */
	class TEVIAN_API FaceDetector : public QObject
	{
	Q_OBJECT
	public:
		using Mode = Client::FaceApi::Mode;
		
		/**
		 * \param file Image file to be detected
		 * \param api REST service helper. If null creates new one
		 * */
		explicit FaceDetector(const QString& file, Client::FaceApi* api = nullptr);
		
		~FaceDetector();
		
		/**
		 * \brief Reads detection data from server.
		 *
		 * \returns true if data is read.
		 *
		 * \details If data updated from GUI Controls
		 * it will fetch them again, deleting old
		 * value.
		 *
		 * \note Must be called before run
		 * */
		bool fetch();
		
		/**
		 * \brief Reads all enabled values.
		 *
		 * \details Values are enabled from \c FaceApi's interfaces.
		 * Available models include: face's bounding rectangle,
		 * landmark points and attributes. Data will.
		 * Wanted to implement face match, it's interesting.
		 * */
		void run();
		
		/**
		 * \returns Face landmarks.
		 *
		 * \note May return empty data, if option
		 * landmarks disabled or fetch not called first.
		 * */
		const QVector<QPoint>&
		getLandmarks();
		
		const std::array<int, 4>&
		getBox() const;
		
		const Details::Attributes&
		getAttributes() const;
		
		const Details::Demographics&
		getDemographics() const;
	
	private:
		void init(Client::FaceApi* api = nullptr);
		
		/**
		 * \brief reads and stores person
		 * attributes like dress, glasses,
		 * hair color etc.
		 * */
		void readAttributes();
		
		/**
		 * \brief reads and stores person
		 * face bounding rectangle.
		 * */
		void readBoundingBox();
		
		/**
		 * \brief reads and stores person
		 * face landmarks and their number.
		 *
		 * \note
		 * */
		void readFacelandmarks();
		
		void readDemographics();
	
	public slots:
		
		/**
		 * \note Called when control changes current values
		 * of \c FaceApi, then class resets old data and updates
		 * with new ones.
		 * */
		void refetch(bool, const ControlData& = ControlData());
	
	private:
		QString m_file;
		
		Mode m_mode;
		
		//! Parser
		JsonReader* m_reader;
		
		//! Face parameters
		Details::FaceParameters m_face;
		
		//! Demographics information
		Details::Demographics m_demographics;
		
		Details::Attributes m_attributes;
		
		Client::FaceApi* m_faceApi;
		
		bool m_fetched;
	};
	
	/**
	 * \author Alvin Ahmadov
	 *
	 * \brief Helper class for \c Annotator.
	 * Parses data read from rest server.
	 * */
	class JsonReader
	{
	public:
		JsonReader();
		
		void setDocument(const QJsonDocument& document);
		
		/**
		 * \brief Checks if data set.
		 *
		 * \returns true if document is not
		 * empty, else false otherwise.
		 * */
		bool isReady();
		
		/**
		 * \brief read json array.
		 *
		 * \param jsonKey \see JsonReader::read
		 *
		 * \returns Returns map of read data.
		 *
		 * \note If there's no data, result will be empty map.
		 * It is caller's responsibility to check valid data.
		 * */
		VariantMultiMap
		readArray(QString jsonKey);
		
		/**
		 * \brief read json object
		 *
		 * \param jsonKey \see JsonReader::read
		 *
		 * \returns Returns map of read data.
		 *
		 * \note If there's no data, result will be empty map.
		 * It is caller's responsibility to check valid data.
		 * */
		VariantMultiMap
		readObject(QString jsonKey);
	
	private:
		/**
		 * \brief read document and call corresponding
		 * method to read data.
		 *
		 * \param jsonKey key of data to read.
		 * */
		void read(QString jsonKey, QJsonValue& value);
	
	private:
		QJsonValue m_jsonValue;
		
		QJsonDocument m_document;
	};
}

