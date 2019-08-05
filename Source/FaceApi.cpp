/**
 *  Copyright (C) 2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 */

#include <memory>
#include "FaceApi.hpp"
#include "Commons.hpp"

#include <QJsonDocument>
#include <QFile>
#include <QString>
#include <QDir>


namespace Tevian
{
	namespace Client
	{
		QRect faceArray(const FaceData::face_array& faceArray)
		{
			return QRect(faceArray[0], faceArray[1], faceArray[2], faceArray[3]);
		}
		
		QString qRectToString(QString str, QRect val)
		{
			return str.arg(val.x())
			          .arg(val.y())
			          .arg(val.width())
			          .arg(val.height());
		}
		
		QString booleanToStr(bool value)
		{
			return value ? "true" : "false";
		}
		
		FaceApi::FaceApi(const QString& url, QString path)
				: AuthorizationHandler(url, path, HttpsHeader::Authorization),
				  m_mode { Mode::Detect },
				  m_detectData { },
				  m_matchData { },
				  m_triple { }
		{ }
		
		void FaceApi::detect(const QString& image,
		                     QJsonDocument& document)
		{
			try
			{
				updateUrl(getPath(Operation::Detect));      // change current url to backend (json)
				// to one of the available paths.
				auto url = getUrl();
				auto file = new QFile(image);               // read contents of image
				
				file->open(QFile::ReadOnly);
				if (!file->isOpen())
				{
					throw IOException { "File %s can not be opened.", image };
				}
				
				createPath(Mode::Detect, url);              // creates options for selected path
				
				setHeader(RequestData::JSON, HttpHeader::ContentTypeHeader);
				if (requiresAuth())
				{
					authorize();                            // set login token
				}
				// send data
				auto response = setHeader(RequestData::JPEG, HttpHeader::ContentTypeHeader).post(file);
				// read response data
				document = QJsonDocument::fromJson(response->readAll());
				delete file;
				
				emit updated(!document.isEmpty());
			} catch (Exception& e)
			{
				qErrnoWarning(e.what());
				emit updated(false);
			}
			
		}
		
		void FaceApi::match(const QString& image1, const QString& image2,
		                    QJsonDocument& document)
		{
			try
			{
				updateUrl(getPath(Operation::Match));
				auto url = getUrl();
				
				createPath(Mode::Match, url);
				
				auto multiPart = std::shared_ptr<QHttpMultiPart>(new QHttpMultiPart(QHttpMultiPart::FormDataType));
				QHttpPart imagePart1;
				QHttpPart imagePart2;
				
				imagePart1.setHeader(HttpHeader::ContentTypeHeader, content(RequestData::JPEG));
				imagePart1.setHeader(QNetworkRequest::ContentDispositionHeader,
				                     QVariant("form-data; name=\"image1\""));
				
				auto imageFile1 = new QFile(image1, multiPart.get());
				imageFile1->open(QFile::ReadOnly);
				imagePart1.setBodyDevice(imageFile1);
				
				imagePart2.setHeader(HttpHeader::ContentTypeHeader, content(RequestData::JPEG));
				imagePart2.setHeader(QNetworkRequest::ContentDispositionHeader,
				                     QVariant("form-data; name=\"image2\""));
				
				auto imageFile2 = new QFile(image2, multiPart.get());
				imageFile2->open(QFile::ReadOnly);
				imagePart2.setBodyDevice(imageFile2);
				
				multiPart->append(imagePart1);
				multiPart->append(imagePart2);
				
				auto response = post(multiPart.get());
				multiPart->setParent(response);
				imageFile1->setParent(multiPart.get());
				imageFile2->setParent(multiPart.get());
				
				if (requiresAuth())
				{
					authorize();
				}
				
				auto resp = setHeader(RequestData::FORMDATA,
				                      HttpHeader::ContentTypeHeader)
						.post(multiPart.get());
				
				document = QJsonDocument::fromJson(resp->readAll());
			} catch (Exception& e)
			{
				qErrnoWarning(e.what());
			}
		}
		
		void FaceApi::setMinSize(int min)
		{
			if (m_mode == Mode::Detect)
			{
				if (m_detectData.isFaceEmpty())
				{
					m_detectData._min_size = min;
				}
			}
		}
		
		void FaceApi::setMaxSize(int max)
		{
			if (m_mode == Mode::Detect)
			{
				if (m_detectData.isFaceEmpty())
				{
					m_detectData._max_size = max;
				}
			}
		}
		
		void FaceApi::setThreshold(float threshold)
		{
			if (m_mode == Mode::Detect)
			{
				if (m_detectData.isFaceEmpty())
				{
					m_detectData._threshold = threshold;
				}
			}
		}
		
		void FaceApi::setFace1(int x, int y, int w, int h)
		{
			if (m_mode == Mode::Detect)
			{
				if (!m_detectData._min_size
				    && !m_detectData._max_size)
				{
					m_detectData.set(x, y, w, h);
				}
			} else
			{
				m_matchData.set(x, y, w, h);
			}
		}
		
		void FaceApi::setFace1(const FaceData::face_array& other)
		{
			m_detectData.set(other);
		}
		
		void FaceApi::setFace2(int x, int y, int w, int h)
		{
			if (m_mode == Mode::Match)
			{
				m_matchData.set2(x, y, w, h);
			}
		}
		
		void FaceApi::setFace2(const FaceData::face_array& other)
		{
			m_matchData.set(other);
		}
		
		void FaceApi::setDemographics(bool enable)
		{
			m_detectData._demographics =
			m_matchData._demographics = enable;
		}
		
		void FaceApi::setAttributes(bool enable)
		{
			m_matchData._attributes =
			m_detectData._attributes = enable;
		}
		
		void FaceApi::setLandmarks(bool enable)
		{
			m_matchData._landmarks =
			m_detectData._landmarks = enable;
		}
		
		int FaceApi::getMinSize()
		{
			return m_detectData._min_size;
		}
		
		int FaceApi::getMaxSize()
		{
			return m_detectData._max_size;
		}
		
		float FaceApi::getThreshold()
		{
			return m_detectData._threshold;
		}
		
		const FaceData::face_array&
		FaceApi::getFace()
		{
			return m_mode == Mode::Detect ?
			       m_detectData._face :
			       m_matchData._face;
		}
		
		const FaceData::face_array&
		FaceApi::getFace2()
		{
			return m_matchData._face2;
		}
		
		bool FaceApi::getDemographics()
		{
			return m_mode == Mode::Detect ?
			       m_detectData._demographics :
			       m_matchData._demographics;
		}
		
		bool FaceApi::getAttributes()
		{
			return m_mode == Mode::Detect ?
			       m_detectData._attributes :
			       m_matchData._attributes;
		}
		
		bool FaceApi::getLandmarks()
		{
			return m_mode == Mode::Detect ?
			       m_detectData._landmarks :
			       m_matchData._landmarks;
		}
		
		void FaceApi::initKeys(Mode mode)
		{
			if (mode == Mode::Detect)
			{
				m_triple.push_back({ "fd_min_size=%1", { QVariant::Int,
				                                         m_detectData._min_size } });
				m_triple.push_back({ "fd_max_size=%1", { QVariant::Int,
				                                         m_detectData._max_size } });
				m_triple.push_back({ "fd_threshold=%1", { QVariant::Double,
				                                          m_detectData._threshold } });
				m_triple.push_back({ "face=%1,%2,%3,%4", { QVariant::Rect,
				                                           m_detectData.asRect() } });
				m_triple.push_back({ "demographics=%1", { QVariant::Bool,
				                                          m_detectData._demographics } });
				m_triple.push_back({ "attributes=%1", { QVariant::Bool,
				                                        m_detectData._attributes } });
				m_triple.push_back({ "landmarks=%1", { QVariant::Bool,
				                                       m_detectData._landmarks } });
			} else
			{
				m_triple.push_back({ "m_face1=%1,%2,%3,%4", { QVariant::Rect,
				                                              m_matchData.asRect() } });
				m_triple.push_back({ "m_face2=%1,%2,%3,%4", { QVariant::Rect,
				                                              m_matchData.asRect2() } });
				m_triple.push_back({ "demographics=%1", { QVariant::Bool,
				                                          m_matchData._demographics } });
				m_triple.push_back({ "attributes=%1", { QVariant::Bool,
				                                        m_matchData._attributes } });
				m_triple.push_back({ "landmarks=%1", { QVariant::Bool,
				                                       m_matchData._landmarks } });
			}
		}
		
		void FaceApi::createPath(Mode mode, QUrl& url)
		{
			if (!m_triple.size())
			{
				initKeys(mode);
			}
			QString path { "?" };
			
			for (int i = 0; i < m_triple.size(); ++i)
			{
				static bool firstMatch { };
				auto key = m_triple.at(i).first;
				auto value = m_triple.at(i).second.second;
				auto tp = m_triple.at(i).second.first;
				
				auto builder = [ & ](QString& requestUrl)
				{
					path.append(requestUrl);
					if (i < m_triple.size() - 1)
					{
						path.append('&');
					}
				};
				
				switch (tp)
				{
					case QVariant::Double :
						if (value.toFloat() != 0)
						{
							key = key.arg(value.toFloat());
							builder(key);
							break;
						}
						continue;
					case QVariant::Int :
						if (value.toInt() != 0)
						{
							key = key.arg(value.toInt());
							builder(key);
							break;
						}
						continue;
					case QVariant::Bool : key = key.arg(value.toBool() ? "true" : "false");
						builder(key);
						break;
					case QVariant::Type::Rect :
						if (mode == Mode::Detect)
						{
							if (!firstMatch && !m_detectData())
							{
								key = qRectToString(key, faceArray(m_detectData._face));
								builder(key);
								firstMatch = !firstMatch;
								break;
							}
						} else
						{
							if (!firstMatch && !m_matchData())
							{
								key = qRectToString(key, faceArray(m_matchData._face));
								builder(key);
								firstMatch = !firstMatch;
							}
							if (firstMatch && !m_matchData.emptyFace2())
							{
								key = qRectToString(key, faceArray(m_matchData._face2));
								builder(key);
							}
							break;
						}
						break;
					default: break;
				}
			}
			
			url = url + path;
			updateUrl(url);
			m_triple.clear();
		}
		
	}// namespace Client
}// namespace Tevian