/**
 *  Copyright (C) 2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 */
#pragma once


#include <tuple>

#include "AuthorizationHandler.hpp"
#include "FaceData.hpp"

#include <QRectF>
#include <QVariantList>
#include <QtCore/QJsonDocument>


class QJsonDocument;

namespace Tevian
{
	namespace Client
	{
		using Triple = QList<QPair<QString, QPair<QVariant::Type, QVariant>>>;
		
		class TEVIAN_API FaceApi : public AuthorizationHandler
		{
		Q_OBJECT
		public:
			enum class Mode
			{
				Detect,
				Match
			};
			
			FaceApi(const QString& url, QString path = QString());
			
			/**
			 * \brief Sends image file to rest server for
			 * image analysis.
			 *
			 *	\param image path to the image to be read.
			 *	\param document a json document in which
			 *	we store read data.
			 *
			 * \details Detect faces on the given image.
			 * Demographics, attributes or face landmarks
			 * information may be included. After logging
			 * in it saves token data to \c Authenticator
			 * for later use.
			 * */
			void detect(const QString& image,
			            QJsonDocument& document);
			
			/**
			 * \brief Validates that the same person is
			 * present on two images.
			 *
			 *	\param image1 path to the first image to be read.
			 *	\param image2 path to the second image to be read.
			 *	\param document a json document in which
			 *	we store read data.
			 *
			 * \note If given image has more than one face
			 * detected and face parameter isn't used, the
			 * largest face will be chosen. If there are
			 * multiple, one with the largest score is
			 * preferred
			 * */
			void match(const QString& image1, const QString& image2,
			           QJsonDocument& document);
		
		public slots:
			// setters
			/**
			 * \param min Minimal size of face that detector will search.
			 *
			 * \note Enabling face bounds disables detection.
			 * */
			void setMinSize(int min = 0);
			
			/**
			 * \param max Maximal size of face that detector will
			 * search. NOTE: zero value disables this constraint.
			 * */
			void setMaxSize(int max = 0);
			
			/**
			 * \param threshold Score threshold to be applied on found faces.
			 *
			 * \note Smaller values increase detector recall, but decrease precision.
			 * */
			void setThreshold(float threshold = 0.8);
			
			/**
			 * \param pos Position of the face on the image in format
			 * x, y, width, height.
			 *
			 * \note Usage of this parameter disables face detector functionality.
			 * */
			void setFace1(int x, int y, int w, int h);
			
			void setFace1(const FaceData::face_array& other);
			
			/**
			 * \param pos Position of the face on the image in format
			 * x, y, width, height.
			 *
			 * \note Usage of this parameter disables face detector functionality.
			 * */
			void setFace2(int x, int y, int w, int h);
			
			void setFace2(const FaceData::face_array& other);
			
			void setDemographics(bool enable);
			
			void setAttributes(bool enable);
			
			void setLandmarks(bool enable);
		
		public:
			// getters
			int getMinSize();
			
			int getMaxSize();
			
			float getThreshold();
			
			const FaceData::face_array&
			getFace();
			
			const FaceData::face_array&
			getFace2();
			
			bool getDemographics();
			
			bool getAttributes();
			
			bool getLandmarks();
		
		signals:
			
			void updated(bool);
			
		protected:
			void initKeys(Mode mode);
			
			void createPath(Mode mode, QUrl& url);
		
		private:
			Mode m_mode;
			
			DetectData m_detectData;
			
			MatchData m_matchData;
			
			Triple m_triple;
			
			Q_DISABLE_COPY(FaceApi)
			
		};
	}// namespace Client
}// namespace Tevian