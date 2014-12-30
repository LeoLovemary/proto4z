﻿#pragma once
#ifndef _TEST_STREAM_H_
#define _TEST_STREAM_H_
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <list>
#include <deque>
#include <queue>
#include <typeinfo>
#include "../proto4z.h"
using namespace  std;
using namespace zsummer::proto4z;

struct tagTestData
{
	bool bl;
	char ch;
	unsigned char uch;
	short sh;
	unsigned short ush;
	int n;
	unsigned int u;
	long l;
	unsigned long ul;
	long long ll;
	unsigned long long ull;
	float f;
	double lf;
	std::string str;
	std::vector<int> vct;
	std::map<int, int> kv;
	std::set<int> st;
	std::multimap<int, int> mkv;
	std::multiset<int> mst;
	std::list<int> lt;
	std::deque<int> dq;
	std::queue<int> qu;
};



inline bool operator==(const tagTestData & tag1, const tagTestData &tag2)
{
	return tag1.bl == tag2.bl
		&& tag1.ch == tag2.ch
		&& tag1.uch == tag2.uch
		&& tag1.ush == tag2.ush
		&& tag1.n == tag2.n
		&& tag1.u == tag2.u
		&& tag1.l == tag2.l
		&& tag1.ul == tag2.ul
		&& tag1.ll == tag2.ll
		&& tag1.ull == tag2.ull
		&& tag1.f == tag2.f
		&& tag1.lf == tag2.lf
		&& tag1.str == tag2.str
		&& tag1.vct.size() == tag2.vct.size()
		&& tag1.kv.size() == tag2.kv.size()
		&& tag1.st.size() == tag2.st.size()
		&& tag1.mkv.size() == tag2.mkv.size()
		&& tag1.mst.size() == tag2.mst.size()
		&& tag1.lt.size() == tag2.lt.size()
		&& tag1.dq.size() == tag2.dq.size();
}
inline bool operator!=(const tagTestData & tag1, const tagTestData &tag2)
{
	return !(tag1 == tag2);
}
template<class StreamHeadTrait>
inline ReadStream<StreamHeadTrait> & operator >>(ReadStream<StreamHeadTrait> & rs, tagTestData & data)
{
	rs >> data.bl;
	rs >> data.ch;
	rs >> data.uch;
	rs >> data.sh;
	rs >> data.ush;
	rs >> data.n;
	rs >> data.u;
	rs >> data.l;
	rs >> data.ul;
	rs >> data.ll;
	rs >> data.ull;
	rs >> data.f;
	rs >> data.lf;
	rs >> data.str;
	rs >> data.vct;
	rs >> data.kv;
	rs >> data.st;
	rs >> data.mkv;
	rs >> data.mst;
	rs >> data.lt;
	rs >> data.dq;
	return rs;
}
template<class StreamHeadTrait>
inline WriteStream<StreamHeadTrait> & operator <<(WriteStream<StreamHeadTrait> & ws, const tagTestData & data)
{
	ws << data.bl;
	ws << data.ch;
	ws << data.uch;
	ws << data.sh;
	ws << data.ush;
	ws << data.n;
	ws << data.u;
	ws << data.l;
	ws << data.ul;
	ws << data.ll;
	ws << data.ull;
	ws << data.f;
	ws << data.lf;
	ws << data.str;
	ws << data.vct;
	ws << data.kv;
	ws << data.st;
	ws << data.mkv;
	ws << data.mst;
	ws << data.lt;
	ws << data.dq;
	return ws;
}





#pragma pack(push)
#pragma pack(1)
struct tagStreamHead
{
	unsigned int _protocolID;
	unsigned int _packLen;
	unsigned int _sessionID;
	unsigned int _agentID;
};
#pragma pack(pop)

struct UDStreamHeadTraits
{
	typedef unsigned int Integer;
	typedef unsigned int ProtoInteger;
	const static Integer MaxPackLen = 1024*1024*10;//10M
	const static bool PackLenIsContainHead = true;
	const static ZSummer_EndianType EndianType = LittleEndian;
	const static Integer HeadLen = sizeof(Integer); //Don't Touch.
};


template <class StreamHeadTrait>
class TestBase
{
public:
	TestBase()
	{
		_testData.bl = true;
		_testData.ch = 'a';
		_testData.uch = 200;
		_testData.ush = -1;
		_testData.n = 65000;
		_testData.u = -2;
		_testData.l = 333;
		_testData.ul = -3;
		_testData.ll = 111;
		_testData.ull = -4;
		_testData.f = (float)123.2;
		_testData.lf = 123.4;
		_testData.str = "1234567";
		_testData.vct.push_back(1);
		_testData.vct.push_back(2);
		_testData.kv[1] = 1;
		_testData.kv[100] = 100;
		_testData.st.insert(10);
		_testData.mkv.insert(std::make_pair(10, 10));
		_testData.mkv.insert(std::make_pair(10, 100));
		_testData.mst.insert(100);
		_testData.mst.insert(100);
		_testData.lt.push_back(10);
		_testData.dq.push_back(100);
		_packLen = 0;
		_bodyLen = 0;
	}

	~TestBase()
	{

	}

	inline bool CheckLenght()
	{
		const char * className = typeid(StreamHeadTrait).name();
		WriteStream<StreamHeadTrait> ws(120);
		try
		{
			ws << _testData;
			_packLen = ws.getStreamLen();
			_bodyLen = ws.getStreamBodyLen();
			if (_packLen - _bodyLen != sizeof(typename StreamHeadTrait::Integer) + sizeof(typename StreamHeadTrait::ProtoInteger))
			{
				cout << "CheckLenght -> " << className 
				<< " HeadLen is error.  packLen=" << _packLen << ", bodyLen=" << _bodyLen << endl;
				return false;
			}
			if (120 != 
				streamToInteger<typename StreamHeadTrait::ProtoInteger, StreamHeadTrait>(ws.getStream() + sizeof(typename StreamHeadTrait::Integer)) )
			{
				cout << "CheckLenght -> " << className 
				<< " write protocol ID error.  packLen=" << _packLen << ", bodyLen=" << _bodyLen << endl;
				return false;
			}
		}
		catch (std::runtime_error e)
		{
			cout << "CheckLenght -> " << className << " write Fail. error msg=" << e.what() << endl;
			return false;
		}
		cout << "CheckLenght -> " << className << " _packLen=" << _packLen << ", _bodyLen=" << _bodyLen << endl;
		return true;
	}

	inline bool CheckProtocol(WriteStream<StreamHeadTrait> &ws, const char * desc);

	inline bool CheckAttachProtocol()
	{
		char * writeBuff = new char[_packLen];
		memset(writeBuff, 0, _packLen);
		WriteStream<StreamHeadTrait> ws(120, writeBuff, _packLen);
		std::string str = "Check Attach Write. protocolTrits=";
		str += typeid(StreamHeadTrait).name();
		str += ":  ";
		return CheckProtocol(ws, str.c_str());
	}

	inline bool CheckNoAttachProtocol()
	{
		WriteStream<StreamHeadTrait> ws(120, NULL, _packLen);
		std::string str = "Check NoAttach Write. protocolTrits=";
		str += typeid(StreamHeadTrait).name();
		str += ":  ";
		return CheckProtocol(ws, str.c_str());
	}

	inline bool CheckRouteProtocol()
	{
		const char * className = "TestBigStreamHeadTrait";

		try
		{
			//analog recv message buff.
			WriteStream<UDStreamHeadTraits> ws(120);
			unsigned int _roomID = 1;
			ws << _roomID << _testData;
			


			//analog recv msg
			std::pair<INTEGRITY_RET_TYPE, UDStreamHeadTraits::Integer> ret = checkBuffIntegrity<UDStreamHeadTraits>(ws.getStream(), ws.getStreamLen(), UDStreamHeadTraits::MaxPackLen);
			if (ret.first == IRT_CORRUPTION || ret.first == IRT_SHORTAGE)
			{
				cout << "CheckRouteProtocol -> " << className << "  checkBuffIntegrity error" << endl;
				return false;
			}

			//check
			ReadStream<UDStreamHeadTraits> rs(ws.getStream(), ws.getStreamLen());
			if (rs.getProtoID() != 120)
			{
				cout << "CheckRouteProtocol -> " << className << "  check Proto ID error" << endl;
				return false;
			}
			//check user data
			unsigned int roomID2 = 0;
			rs >> roomID2;
			if (roomID2 != _roomID)
			{
				cout << "CheckRouteProtocol -> " << className << "  check room ID error" << endl;
				return false;
			}
			//check route
			WriteStream<UDStreamHeadTraits> ws2(120);
			ws2.appendOriginalData(rs.getStreamUnread(), rs.getStreamUnreadLen());
			ReadStream<UDStreamHeadTraits> rs2(ws2.getStream(), ws2.getStreamLen());
			tagTestData testData;
			rs2 >> testData;
			if (testData != _testData)
			{
				cout << "CheckRouteProtocol -> " << className << "  check route error" << endl;
				return false;
			}
			cout << "CheckRouteProtocol -> " << className << "  check route OK" << endl;


			return true;
		}
		catch (std::runtime_error e)
		{
			cout << "CheckRouteProtocol -> " << className << "  check route Failed. msg=" << e.what() << endl;
			return false;
		}
		return true;
			
	}


	typename StreamHeadTrait::Integer _packLen;
	typename StreamHeadTrait::Integer _bodyLen;
	tagTestData _testData;
};







template < class StreamHeadTrait>
bool TestBase<StreamHeadTrait>::CheckProtocol(WriteStream<StreamHeadTrait> &ws, const char * desc)
{
	//check write
	{
		try
		{
			ws << _testData;
			cout << desc << " write data OK." << endl;
			if (ws.getStreamLen() == _packLen)
			{
				cout << desc << " write data  WriteStreamLen OK." << endl;
			}
			else
			{
				cout << desc << " write data  WriteStreamLen Failed. getStreamLen()="
					<< ws.getStreamLen() << ", _packLen=" << _packLen << endl;
				return false;
			}
			if (ws.getStreamBodyLen() == _bodyLen)
			{
				cout << desc << " write data WriteStreamBodyLen OK." << endl;
			}
			else
			{
				cout << desc << " write data WriteStreamBodyLen Failed. getStreamBodyLen()="
					<< ws.getStreamBodyLen() << ", _bodyLen=" << _bodyLen << endl;
				return false;
			}
		}
		catch (std::runtime_error e)
		{
			cout << desc << " write data catch error msg=" << e.what() << endl;
			return false;
		}
		try
		{
			ws << 'c';
			cout << desc << " write data check bound Failed. getStreamBodyLen()="
				<< ws.getStreamBodyLen() << ", _bodyLen=" << _bodyLen << endl;
			return false;
		}
		catch (std::runtime_error e)
		{
			cout << desc << " write data check bound OK" << endl;
		}
	}


	//check checkBuffIntegrity
	{
		size_t headLen = sizeof(typename StreamHeadTrait::Integer) + sizeof(typename StreamHeadTrait::ProtoInteger);
		std::pair<INTEGRITY_RET_TYPE, typename StreamHeadTrait::Integer> ret = checkBuffIntegrity<StreamHeadTrait>(ws.getStream(), 1, _packLen);
		if (ret.first == IRT_SHORTAGE && ret.second == headLen - 1)
		{
			cout << desc << " checkBuffIntegrity check header len OK" << endl;
		}
		else
		{
			cout << desc << " checkBuffIntegrity check header len failed" << endl;
			return false;
		}
		ret = checkBuffIntegrity<StreamHeadTrait>(ws.getStream(), ws.getStreamLen(), ws.getStreamLen());
		if (ret.first == IRT_SUCCESS && ret.second == _packLen)
		{
			cout << desc << " checkBuffIntegrity check integrity  OK" << endl;
		}
		else
		{
			cout << desc << " checkBuffIntegrity check integrity  failed" << endl;
			return false;
		}
	}
	

	//check read
	{
		tagTestData readTestData;
		ReadStream<StreamHeadTrait> rs(ws.getStream(), ws.getStreamLen());

		try
		{
			rs >> readTestData;
			cout << desc << " check Read all data  OK" << endl;
		}
		catch (std::runtime_error e)
		{
			cout << desc << " check Read all data  failed" << e.what() << endl;
			return false;
		}

		if (readTestData == _testData)
		{
			cout << desc << "consistency check OK" << endl;
		}
		else
		{
			cout << desc << "consistency check failed" << endl;
			return false;
		}

		try
		{
			char ch = 'a';
			rs >> ch;
			cout << desc << "read bounds check  ReadStream failed" << endl;
			return false;
		}
		catch (std::runtime_error e)
		{
			cout << desc << "read bounds check  ReadStream OK." << endl;
		}
		cout << desc << "check OK." << endl;


		ReadStream<StreamHeadTrait> rs2(rs.getStreamBody(), rs.getStreamBodyLen(), false);
		tagTestData readTestData2;
		try
		{

			rs2 >> readTestData2;
			cout << desc << " check Read all data  OK" << endl;
		}
		catch (std::runtime_error e)
		{
			cout << desc << " check Read all data  failed" << e.what() << endl;
			return false;
		}

		if (readTestData2 == _testData)
		{
			cout << desc << "consistency check OK" << endl;
		}
		else
		{
			cout << desc << "consistency check failed" << endl;
			return false;
		}

		try
		{
			char ch = 'a';
			rs2 >> ch;
			cout << desc << "read bounds check  ReadStream failed" << endl;
			return false;
		}
		catch (std::runtime_error e)
		{
			cout << desc << "read bounds check  ReadStream OK." << endl;
		}	
	}
	
	cout << desc << "check OK." << endl;
	return true;
}



#endif


