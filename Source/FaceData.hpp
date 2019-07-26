/**
 *  Copyright (C) 2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 */
#pragma once


#include <array>

#include <QRect>


namespace Tevian
{
	struct FaceData
	{
		using face_array = std::array<int, 4>;
		
		inline void set(int x = 0, int y = 0, int w = 0, int h = 0)
		{
			_face = { x, y, w, h };
		}
		
		inline void set(const face_array& other)
		{
			_face = { other };
		}
		
		inline QRect asRect()
		{
			return QRect(_face[0], _face[1], _face[2], _face[3]);
		}
		
		inline static bool isFaceEmpty(face_array faceArray)
		{
			int v { 0 };
			
			for (int i : faceArray)
			{
				v += i;
			}
			
			return v == 0;
		}
		
		inline bool isFaceEmpty()
		{
			return isFaceEmpty(_face);
		}
		
		inline bool operator()() const
		{
			return isFaceEmpty(_face);
		}
		
		// members
		face_array _face = face_array();
		
		bool _attributes;
		
		bool _landmarks;
		
		bool _demographics;
	};
	
	struct DetectData : public FaceData
	{
		int _min_size = 0;
		int _max_size = 0;
		float _threshold = 0.0;
	};
	
	struct MatchData : public FaceData
	{
		inline bool emptyFace2() const
		{
			return _face2.empty();
		}
		
		inline void set2(int x = 0, int y = 0, int w = 0, int h = 0)
		{
			_face2 = { x, y, w, h };
		}
		
		inline QRect asRect2()
		{
			return QRect(_face2[0], _face2[1], _face2[2], _face2[3]);
		}
		
		face_array _face2 = face_array();
	};
	
	struct ControlData : public DetectData
	{
		face_array _face2 { };
	};
}


