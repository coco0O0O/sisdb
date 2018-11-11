
#include "compress.day.h"
#include "cllog.h"
#include "cltime.h"



/////////////////////////////////////////////////////////////////////////////////////////
//BITCODE
//	����	 �೤ ��� ����		���� ����ƫ��

static BITCODE dayTimeCode[] =		//�ɽ�ʱ��
{
	{ 0, 1, 0, 'E', 0, 0 },			// 0				= m_baseTime
	{ 0x2, 2, 4, 'B', 4, 1 },		// 10	+4Bit		= m_baseTime + 4Bit + 1 = 16
	{ 0x6, 3, 8, 'B', 8, 17 },		// 110	+8Bit		= m_baseTime + 8Bit + 17
	{ 0x7, 3, 32, 'D', 0, 0 },		// 111	+32Bit		= 32Bit Org
};
static BITCODE dayPriceDiffCode[] = //���ѹ���۸�
{
	{ 0, 1, 0, 'E', 0, 0 },			// 0				= ��׼
	{ 0x2, 2, 4, 'b', 4, 0 },			// 10	+4Bit		= ��׼+4Bit
	{ 0x6, 3, 6, 'b', 6, 8 },			// 110	+6Bit		= ��׼+6Bit+8
	{ 0xE, 4, 8, 'b', 8, 40 },			// 1110	+8Bit		= ��׼+8Bit+32+8
	{ 0x1E, 5, 12, 'b', 12, 168 },			// 11110+12Bit		= ��׼+12Bit+128+32+8
	{ 0x1F, 5, 32, 'D', 0, 0 },			// 11111+32Bit		= 32Bit Org
};

static BITCODE dayPriceCode[] =	//�����ѹ���۸�
{
	{ 0xE, 4, 0, 'E', 0, 0 },			// 1110				= ��׼
	{ 0x2, 2, 4, 'b', 4, 0 },			// 10	+4Bit		= ��׼+4Bit
	{ 0x0, 1, 6, 'b', 6, 8 },			// 0	+6Bit		= ��׼+6Bit+8
	{ 0x6, 3, 8, 'b', 8, 40 },			// 110	+8Bit		= ��׼+8Bit+32+8
	{ 0x1E, 5, 12, 'b', 12, 168 },			// 11110+12Bit		= ��׼+12Bit+128+32+8
	{ 0x1F, 5, 32, 'D', 0, 0 },			// 11111+32Bit		= 32Bit Org
};
static BITCODE dayVolCode[] =	//�����ѹ���ɽ���
{
	{ 0x6, 3, 12, 'B', 12, 0 },			// 110	+12Bit		= ��׼+12Bit
	{ 0x0, 1, 16, 'B', 16, 4096 },		// 0	+16Bit		= ��׼+16Bit+4096
	{ 0x2, 2, 24, 'B', 24, 69632 },		// 10	+24Bit		= ��׼+24Bit+65536+4096
	{ 0x7, 3, 32, 'M', 0, 0 },			// 111  +32Bit		= 32Bit Org
};

static BITCODE dayAmountCode[] =	//�����ѹ�� �Գɽ���*����*ÿ�ֹ���Ϊ��׼(��������)
{
	{ 0x6, 3, 12, 'b', 12, 0 },			// 110	+12Bit		= ��׼+12Bit
	{ 0x2, 2, 16, 'b', 16, 2048 },		// 10	+16Bit		= ��׼+16Bit+2048
	{ 0x0, 1, 24, 'b', 24, 34816 },		// 0	+24Bit		= ��׼+24Bit+32768+2048
	{ 0x7, 3, 32, 'M', 0, 0 },			// 111	+32Bit		= 32Bit float
};
/////////////////////////////
//  Tick
///////////////////////////////
static BITCODE tickTimeCode[] =
{
	{ 0, 1, 0, 'E', 0, 0 },			//0					= ��׼
	{ 2, 2, 8, 'B', 8, 1 },			//10	+8Bit		= ��׼ + 8Bit +1
	{ 6, 3, 16, 'B', 16, 257 },		//110	+16Bit		= ��׼ + 16Bit + 256 + 1
	{ 0xE, 4, 24, 'B', 24, 65793 },	//1110	+24Bit		= ��׼ + 24Bit + 65536 + 256 + 1
	{ 0xF, 4, 32, 'D', 0, 0 },		//1111	+32Bit		= 32Bit org
};
static BITCODE tickPriceCode[] = //�׼�¼
{
	{ 0x2, 2, 8, 'B', 8, 0 },			//10   +8Bit	= 8Bit
	{ 0x0, 1, 12, 'B', 12, 256 },		//0   +12Bit	= 12Bit+256
	{ 0x6, 3, 16, 'B', 16, 4352 },		//110 +16Bit	= 16Bit+4096+256
	{ 0x7, 3, 32, 'D', 0, 0 },			//111 +32Bit	= uint32
};

static BITCODE tickPriceDiffCode[] = //֮����ǰһ���۸�Ϊ��׼
{
	{ 0, 1, 0, 'E', 0, 0 },			//0				= ��׼
	{ 2, 2, 2, 'b', 2, 0 },			//10			= ��׼+2Bit; todo change to 100 +1, 101 -1
	{ 0x6, 3, 4, 'b', 4, 1 },			//110	+4Bit	= ��׼+4Bit+1
	{ 0xE, 4, 8, 'b', 8, 9 },			//1110  +8Bit	= ��׼+8Bit+9
	{ 0x1E, 5, 16, 'b', 16, 137 },		//11110 +16Bit	= ��׼+16Bit+128+9
	{ 0x1F, 5, 32, 'D', 0, 0 },			//11111 +32Bit	= uint32
};
static BITCODE tickVolDiffCode[] = //���ѹ���ɽ���
{
	{ 0x6, 3, 4, 'Z', 2, 1 },			// 110	+2Bit+2Bit	= ��׼+2Bit*10^N
	{ 0xE, 4, 6, 'Z', 4, 5 },			// 1110	+4Bit+2Bit	= ��׼+(4Bit+4)*10^N
	{ 0x0, 1, 6, 'B', 6, 0 },			// 0	+6Bit		= ��׼+6Bit
	{ 0x2, 2, 12, 'B', 12, 64 },		// 10	+12Bit		= ��׼+12Bit+64
	{ 0x1E, 5, 16, 'B', 16, 4160 },		// 11110+16Bit		= ��׼+16Bit+4096+64
	{ 0x1F, 5, 32, 'M', 0, 0 },			// 11111+32Bit		= 32Bit Org
};
////---------------------------------------------------/////
static BITCODE dynaID[] =	//stockID
{
	{ 0, 1, 0, 'E', 1, 0 },			// 0				˳��
	{ 0x2, 2, 4, 'B', 4, 0 },			// 10	+4Bit		˳��,+2+4Bit
	{ 0x3, 2, 16, 'D', 0, 0 },			// 11	+16Bit		ֱ�Ӵ��wStockID
};

static BITCODE dynaCode[] =		//����
{
	{ 0, 1, 0, 'E', 1, 0 },			//0					˳��
	{ 0x4, 3, 4, 'B', 4, 2 },			//100  +4Bit		˳��,+2+4Bit
	{ 0x5, 3, 8, 'B', 8, 18 },			//101  +8Bit		˳��+16+2+8Bit
	{ 0x6, 3, 32, 'D', 0, 0 },			//110 +32Bit		ֱ�ӵ�������Label
	{ 0x7, 3, 0, 's', 0, 0 },			//111 +�ַ���		������Label
};
static BITCODE mmpVolCode[] =	//�����ѹ����������
{
	{ 0x1, 2, 6, 'B', 6, 0 },			// 01	+6Bit		= 6Bit
	{ 0x0, 2, 8, 'B', 8, 64 },			// 00	+8Bit		= 8Bit+64
	{ 0x6, 3, 12, 'B', 12, 320 },		// 110	+12Bit		= 12Bit+256+64
	{ 0x2, 2, 16, 'B', 16, 4416 },		// 10	+16Bit		= 16Bit+4096+256+64
	{ 0xE, 4, 24, 'B', 24, 69952 },		// 1110 +24Bit		= 24Bit+65536+4096+256+64
	{ 0xF, 4, 32, 'D', 0, 0 },			// 1111 +32Bit		= 32Bit UINT32
};
////////////////////////////////////////////////////////////////
//
// ���������ߵ�ѹ���㷨
//
///////////////////////////////////////////////////////////////
void writehead(c_bit_stream &stream, CL_COMPRESS_TYPE cltype, int count, s_stock_static *info, bool isnow, bool islast)
{
	stream.MoveTo(0);
	stream.Put('R', 8);
	if (islast) stream.Put(1, 1);
	else  stream.Put(0, 1); //����
	stream.Put(cltype, 7);
	stream.Put(count, 8);
	///////////////////ͷ�ṹ/////////////////////////////////////////
	if (info->m_type_by == STK_STATIC::INDEX) stream.Put(1, 1);
	else stream.Put(0, 1);
	stream.Put(info->m_price_digit_by, 3);
	int volpower = cl_sqrt10(info->m_volunit_w);
	stream.Put(volpower, 3);
	if (isnow) stream.Put(1, 1);
	else  stream.Put(0, 1); 

}

listNode *cl_redis_compress_day(uint8 *in, int count_, s_stock_static *info)
{
	if (count_<1) return NULL;

	s_stock_day *lpCur = (s_stock_day*)in;

	uint8 out[CL_COMPRESS_MAX_RECS*sizeof(s_stock_day)];
	c_bit_stream stream(out, CL_COMPRESS_MAX_RECS*sizeof(s_stock_day));

	int nPage = (int)(count_ / CL_COMPRESS_MAX_RECS) + 1;
	cl_block_head head;
	//cl_last_info  last;

	listNode *result = NULL;
	try
	{
		int index = 0;
		int expand = cl_power10(info->m_price_digit_by);
		int outlen = 0;
		//////////////////��׼ֵ//////////////////////////////////////////
		uint32 tLastTime = 0;
		uint32 m_open_dw = 0;
		////////////////////////////////////////////////////////////
		for (int m = 0; m < nPage; m++)
		{
			stream.MoveTo(0);
			if (m == (nPage - 1)) head.count = count_ - m*CL_COMPRESS_MAX_RECS;
			else head.count = CL_COMPRESS_MAX_RECS;
			writehead(stream, CL_COMPRESS_DAY, head.count, info, false, false);
			//////////////////��Ʊ����//////////////////////////////////////////
			tLastTime = (uint32)lpCur[index].m_time_t;
			stream.Put(tLastTime, 32);
			m_open_dw = lpCur[index].m_open_dw;
			stream.SETBITCODE(dayPriceCode);
			stream.EncodeData(m_open_dw);
			//m_open_dw = lpCur[index].m_open_dw;
			//stream.Put(m_open_dw, 32);
			//////////////////��׼ֵ//////////////////////////////////////////
			for (int k = 0; k < head.count; k++, index++)
			{
				stream.SETBITCODE(dayTimeCode);
				stream.EncodeData((uint32)lpCur[index].m_time_t, (uint32*)&tLastTime);
				tLastTime = (uint32)lpCur[index].m_time_t;

				stream.SETBITCODE(dayPriceCode);
				stream.EncodeData(lpCur[index].m_open_dw, &m_open_dw);
				m_open_dw = lpCur[index].m_open_dw;

				stream.SETBITCODE(dayPriceDiffCode);
				stream.EncodeData(lpCur[index].m_high_dw, &m_open_dw);
				stream.EncodeData(lpCur[index].m_low_dw, &m_open_dw);
				stream.EncodeData(lpCur[index].m_new_dw, &m_open_dw);

				stream.SETBITCODE(dayVolCode);
				stream.EncodeData(lpCur[index].m_volume_z);

				stream.SETBITCODE(dayAmountCode);
				zint32 mAmountBase(0, 0);
				if (info->m_type_by != STK_STATIC::INDEX)
					mAmountBase = lpCur[index].m_volume_z*(int)(info->m_volunit_w * lpCur[index].m_new_dw / expand);
				stream.EncodeData(lpCur[index].m_amount_z, &mAmountBase);

				//stream.SETBITCODE(dayInnerVolCode);
				//stream.EncodeData(lpCur[i].m_DayStock.m_dwInnerVol);
			} //k
			outlen = (stream.GetCurPos() + 7) / 8;
			//һ����ѹ�����ˣ�����sds��
			result = sdsnode_push(result, out, outlen);
		}//m
/*		last.offset = outlen;
		last.bit = (stream.GetCurPos() % 8) > 0 ? 8 - (stream.GetCurPos() % 8) : 0; //offset*8 - bit  
		//�����Ҫ���ӻ�׼���ݿ�head.compress=1,block.count=0/1;
		writehead(stream, CL_COMPRESS_DAY, 0, info, false, true);
		stream.Put(last.offset, 16); 
		stream.Put(last.bit, 8);
		//��д���ݳ��ȵ���Ϣ,���һ�������������,��ʾʵ�����ݵ�ƫ��λ��
		//////////////////��Ʊ����//////////////////////////////////////////
		stream.Put(tLastTime, 32);
		stream.SETBITCODE(dayPriceCode);
		stream.EncodeData(m_open_dw);
		//////////////////��׼ֵ//////////////////////////////////////////
		outlen = (stream.GetCurPos() + 7) / 8;
		result = sdsnode_push(result, out, outlen);
*/
	}
	catch (...)
	{
		result = NULL;
	}
	return result;

}


s_memory_node * cl_redis_uncompress_day(uint8 *in, int inlen, OUT_COMPRESS_TYPE type)
{
	int i;
	char str[255];
	s_memory_node * result = malloc_memory();

	uint32 m_open_dw = 0;
	uint32 tLastTime = 0;
	cl_block_head head;
	//cl_last_info  last;
	c_bit_stream stream((uint8*)in, inlen);
	try
	{
		result->clear();
		head.hch=stream.Get(8);
		head.compress=stream.Get(1);
		head.type = stream.Get(7);
		head.count = stream.Get(8);
		if (head.count < 1) return result;
		///////////////////ͷ�ṹ/////////////////////////////////////////
		int isindex = stream.Get(1); //�ǲ���ָ��
		int expand = stream.Get(3); //
		expand = cl_power10(expand);
		int vol-unit = stream.Get(3);//
		vol-unit = cl_power10(vol-unit);
		stream.Get(1); //=0 ����ǰ���� =1 ������
		if (head.compress){
			//stream.Move(sizeof(cl_last_info));
			//last.offset = 
				stream.Get(16);
			//last.bit = 
				stream.Get(8);
		}
		//////////////////��Ʊ����//////////////////////////////////////////
		tLastTime = stream.Get(32);
		stream.SETBITCODE(dayPriceCode);
		m_open_dw = stream.DecodeData(&m_open_dw);
		//////////////////��׼ֵ//////////////////////////////////////////

		s_stock_day stkDay;
		for (i = 0; i<head.count; i++)
		{
			memset(&stkDay, 0, sizeof(s_stock_day));
			stream.SETBITCODE(dayTimeCode);
			stkDay.m_time_t = stream.DecodeData((uint32*)&tLastTime);
			tLastTime = (uint32)stkDay.m_time_t;

			stream.SETBITCODE(dayPriceCode);
			stkDay.m_open_dw = stream.DecodeData(&m_open_dw);
			m_open_dw = stkDay.m_open_dw;

			stream.SETBITCODE(dayPriceDiffCode);
			stkDay.m_high_dw = stream.DecodeData(&m_open_dw);
			stkDay.m_low_dw = stream.DecodeData(&m_open_dw);
			stkDay.m_new_dw = stream.DecodeData(&m_open_dw);

			stream.SETBITCODE(dayVolCode);
			stkDay.m_volume_z = stream.DecodeMWordData();

			stream.SETBITCODE(dayAmountCode);
			zint32 mAmountBase(0, 0);
			if (isindex == 0){
				mAmountBase = stkDay.m_volume_z*(int)(vol-unit*stkDay.m_new_dw / expand);
			}
			stkDay.m_amount_z = stream.DecodeMWordData(&mAmountBase);
			//stream.SETBITCODE(dayInnerVolCode);
			//stkDay.m_DayStock.m_dwInnerVol = stream.DecodeMWordData();
			if (type == OUT_COMPRESS_JSON)
			{
				sprintf(str, "[%d,%d,%d,%d,%d,"P64I","P64I"]", (uint32)stkDay.m_time_t, stkDay.m_open_dw, stkDay.m_high_dw,
					stkDay.m_low_dw, stkDay.m_new_dw, stkDay.m_volume_z.aslong(), stkDay.m_amount_z.aslong());
				*result += str;
				if (i < head.count - 1) *result += ",";
			}
			else
			{
				result->append((char *)&stkDay, sizeof(s_stock_day));
			}
		}
		//result += "]";
	}
	catch (...)
	{
		result->clear();
		if (type == OUT_COMPRESS_JSON){ *result = "[]"; }
	}
	return result;
}

void write_compress_count(c_bit_stream *stream, int count)
{
	if (count <= CL_COMPRESS_MAX_RECS){
		stream->Put(count, 8);
	}
	else if (count <= 0xFFFF){
		stream->Put(253, 8);
		stream->Put(count, 16);
	}
	else {
		stream->Put(254, 8);
		stream->Put(count, 32);
	}
}
int read_compress_count(c_bit_stream *stream)
{
	int count = stream->Get(8);

	if (count == 253){
		count = stream->Get(16);
	}
	else {
		count = stream->Get(32);
	}
	return count;
}
listNode *cl_redis_compress_close(uint8 *in, int count_, s_stock_static *info)
{
	if (count_<1) return NULL;
	uint8 *out = (uint8 *)zmalloc(10000 * 32);
	c_bit_stream stream(out, 10000 * 32);

	listNode *result = NULL;
	s_stock_day *lpCur = (s_stock_day*)in;
	try
	{
		int index = 0;
		int outlen = 0;
		//////////////////��׼ֵ//////////////////////////////////////////
		uint32 m_time_t = 0;
		uint32 m_new_dw = 0;
		////////////////////////////////////////////////////////////
		stream.MoveTo(0);
		stream.Put('R', 8);
		stream.Put(1, 1);
		stream.Put(CL_COMPRESS_CLOSE, 7);
		write_compress_count(&stream, count_);
		///////////////////ͷ�ṹ/////////////////////////////////////////
		if (info->m_type_by == STK_STATIC::INDEX) stream.Put(1, 1);
		else stream.Put(0, 1);
		stream.Put(info->m_price_digit_by, 3);
		int volpower = cl_sqrt10(info->m_volunit_w);
		stream.Put(volpower, 3);
		stream.Put(0, 1); 
		
		m_time_t = (uint32)lpCur[index].m_time_t;
		stream.Put(m_time_t, 32);
		m_new_dw = lpCur[index].m_new_dw;
		stream.SETBITCODE(dayPriceCode);
		stream.EncodeData(m_new_dw);

		for (int k = 0; k < count_; k++)
		{
			stream.SETBITCODE(dayTimeCode);
			stream.EncodeData((uint32)lpCur[index].m_time_t, (uint32*)&m_time_t);
			m_time_t = (uint32)lpCur[index].m_time_t;

			stream.SETBITCODE(dayPriceCode);
			stream.EncodeData(lpCur[index].m_new_dw, &m_new_dw);
			m_new_dw = lpCur[index].m_new_dw;
		} //k
		outlen = (stream.GetCurPos() + 7) / 8;
		result = sdsnode_create(out, outlen);
	}
	catch (...)
	{
		result = NULL;
	}
	zfree(out);
	return result;

}
////////////////////////////////////////////////////////
//  ������ѹ�� ����now
////////////////////////////////////////////////////////
#define MINSEC 60 //��Ȼ�Ƿ����ߣ��Ǿ�һ����Ҫ����60
listNode *cl_redis_compress_min(uint8 *in, int count_, s_stock_static *info)
//info����������ָ��������ѹ������ɽ����
//isnow=0��ʾ��ʷ�ķ�����5,15,30,60,volΪÿ���ӳɽ���;�ж�������������仯����һ���ֶ�Ϊǰ������Ϣ
//isnow=1��ʾ����ķ�����5,15,30,60,volΪ���տ��к��ۼƳɽ���;��һ���ֶ�Ϊԭʼǰ������Ϣ
{
	if (count_<1) return NULL;
	s_stock_day *lpCur = (s_stock_day*)in;

	uint8 out[CL_COMPRESS_MAX_RECS*sizeof(s_stock_day)];
	c_bit_stream stream(out, CL_COMPRESS_MAX_RECS*sizeof(s_stock_day));

	int nPage = (int)(count_ / CL_COMPRESS_MAX_RECS) + 1;
	cl_block_head head;
	//cl_last_info  last;
	listNode *result = NULL;
	try
	{
		int index = 0;
		int expand = cl_power10(info->m_price_digit_by);
		int outlen = 0;
		//////////////////��׼ֵ//////////////////////////////////////////
		uint32 tLastTime = 0;
		uint32 m_open_dw = 0;
		////////////////////////////////////////////////////////////
		for (int m = 0; m < nPage; m++)
		{
			outlen = 0;
			stream.MoveTo(0); 
			if (m == (nPage - 1))
			{
				head.count = count_ - m*CL_COMPRESS_MAX_RECS;
			}
			else{
				head.count = CL_COMPRESS_MAX_RECS;
			}				
			writehead(stream, CL_COMPRESS_M5, head.count, info, false, false);

			//////////////////��Ʊ����//////////////////////////////////////////
			tLastTime = (uint32)(lpCur[index].m_time_t / MINSEC);  
			stream.Put(tLastTime, 32);
			m_open_dw = lpCur[index].m_open_dw;
			stream.SETBITCODE(dayPriceCode);
			stream.EncodeData(m_open_dw);
			//////////////////��׼ֵ//////////////////////////////////////////
			for (int k = 0; k < head.count; k++, index++)
			{
				stream.SETBITCODE(tickTimeCode);
				stream.EncodeData((uint32)(lpCur[index].m_time_t / MINSEC), (uint32*)&tLastTime);
				tLastTime = (uint32)(lpCur[index].m_time_t / MINSEC);

				stream.SETBITCODE(dayPriceCode);
				stream.EncodeData(lpCur[index].m_open_dw, &m_open_dw);
				m_open_dw = lpCur[index].m_open_dw;

				stream.SETBITCODE(dayPriceDiffCode);
				stream.EncodeData(lpCur[index].m_high_dw, &m_open_dw);
				stream.EncodeData(lpCur[index].m_low_dw, &m_open_dw);
				stream.EncodeData(lpCur[index].m_new_dw, &m_open_dw);

				stream.SETBITCODE(dayVolCode);
				stream.EncodeData(lpCur[index].m_volume_z);

				stream.SETBITCODE(dayAmountCode);
				zint32 mAmountBase(0, 0);
				if (info->m_type_by != STK_STATIC::INDEX)
					mAmountBase = lpCur[index].m_volume_z*(int)(info->m_volunit_w * lpCur[index].m_new_dw / expand);
				stream.EncodeData(lpCur[index].m_amount_z, &mAmountBase);

				//stream.SETBITCODE(dayInnerVolCode);
				//stream.EncodeData(lpCur[i].m_DayStock.m_dwInnerVol);
			} //k

			outlen = (stream.GetCurPos() + 7) / 8;
			//һ����ѹ�����ˣ�����sdsnode��
			result = sdsnode_push(result, out, outlen);
		}//m
	}
	catch (...)
	{
		result = NULL;
	}
	return result;
}
s_memory_node *cl_redis_uncompress_min(uint8 *in, int inlen, OUT_COMPRESS_TYPE type)
{
	int i;
	char str[255];
	s_memory_node * result = malloc_memory();;

	uint32 open_dw = 0;
	uint32 tLastTime = 0;
	zint32  volume_z(0, 0);
	cl_block_head head;
	//cl_last_info last;
	c_bit_stream stream((uint8*)in, inlen);
	try
	{
		result->clear();
		head.hch = stream.Get(8);
		head.compress = stream.Get(1);
		head.type = stream.Get(7);
		head.count = stream.Get(8);
		if (head.count < 1) return result;
		///////////////////ͷ�ṹ/////////////////////////////////////////
		int isindex = stream.Get(1); //�ǲ���ָ��
		int expand = stream.Get(3); //
		expand = cl_power10(expand);
		int vol-unit = stream.Get(3);//
		vol-unit = cl_power10(vol-unit);
		int isnow = stream.Get(1); //=0 ��ʷ���� =1 ʵʱ����
		if (head.compress){
			//last.offset = 
			stream.Get(16);
			//last.bit = 
			stream.Get(8);
		}
		//////////////////��Ʊ����//////////////////////////////////////////
		tLastTime = stream.Get(32);
		stream.SETBITCODE(dayPriceCode);
		open_dw = stream.DecodeData(&open_dw);
		if (isnow)
		{
			stream.SETBITCODE(dayVolCode);
			volume_z = stream.DecodeMWordData(&volume_z);
		}
		//////////////////��׼ֵ//////////////////////////////////////////
		s_stock_day stkDay;
		result->clear();
		for (i = 0; i<head.count; i++)
		{
			memset(&stkDay, 0, sizeof(s_stock_day));
			stream.SETBITCODE(tickTimeCode);
			stkDay.m_time_t = stream.DecodeData((uint32*)&tLastTime);
			tLastTime = (uint32)stkDay.m_time_t;
			stkDay.m_time_t = stkDay.m_time_t*MINSEC;

			stream.SETBITCODE(dayPriceCode);
			stkDay.m_open_dw = stream.DecodeData(&open_dw);
			open_dw = stkDay.m_open_dw;

			stream.SETBITCODE(dayPriceDiffCode);
			stkDay.m_high_dw = stream.DecodeData(&open_dw);
			stkDay.m_low_dw = stream.DecodeData(&open_dw);
			stkDay.m_new_dw = stream.DecodeData(&open_dw);

			if (isnow)
			{
				stream.SETBITCODE(tickVolDiffCode);
				stkDay.m_volume_z = stream.DecodeMWordData(&volume_z);
				volume_z = stkDay.m_volume_z;
			}
			else
			{
				stream.SETBITCODE(dayVolCode);
				stkDay.m_volume_z = stream.DecodeMWordData();
			}
			stream.SETBITCODE(dayAmountCode);
			zint32 mAmountBase(0, 0);
			if (isindex == 0){
				mAmountBase = stkDay.m_volume_z*(int)(vol-unit*stkDay.m_new_dw / expand);
			}
			stkDay.m_amount_z = stream.DecodeMWordData(&mAmountBase);

			//stream.SETBITCODE(dayInnerVolCode);
			//stkDay.m_DayStock.m_dwInnerVol = stream.DecodeMWordData();
			if (type == OUT_COMPRESS_JSON)
			{
				sprintf(str, "[%d,%d,%d,%d,%d,"P64I","P64I"]", (uint32)stkDay.m_time_t, stkDay.m_open_dw, stkDay.m_high_dw,
					stkDay.m_low_dw, stkDay.m_new_dw, stkDay.m_volume_z.aslong(), stkDay.m_amount_z.aslong());
				*result += str;
				if (i < head.count - 1) *result += ",";
			}
			else{
				result->append((char *)&stkDay, sizeof(s_stock_day));
			}
		}
		//result += "]";
	}
	catch (...)
	{
		result->clear();
		if (type == OUT_COMPRESS_JSON) *result = "[]";
	}
	return result;
}


listNode *cl_redis_compress_tmin(uint8 *in, int count_, s_stock_static *info)
//info����������ָ��������ѹ������ɽ����
{
	if (count_<1) return NULL;
	s_stock_day *lpCur = (s_stock_day*)in;

	uint8 out[CL_COMPRESS_MAX_RECS*sizeof(s_stock_day)];
	c_bit_stream stream(out, CL_COMPRESS_MAX_RECS*sizeof(s_stock_day));

	int nPage = (int)(count_ / CL_COMPRESS_MAX_RECS) + 1;
	cl_block_head head;
	cl_last_info  last;
	listNode *result = NULL;
	try
	{
		int index = 0;
		int expand = cl_power10(info->m_price_digit_by);
		int outlen = 0;
		//////////////////��׼ֵ//////////////////////////////////////////
		uint32 start_min = 0;
		uint32 m_open_dw = 0;
		zint32  m_volume_z(0, 0);
		////////////////////////////////////////////////////////////
		for (int m = 0; m < nPage; m++)
		{
			outlen = 0;
			stream.MoveTo(0);
			if (m == (nPage - 1))
			{
				head.count = count_ - m*CL_COMPRESS_MAX_RECS;
				head.count--;
			}
			else{
				head.count = CL_COMPRESS_MAX_RECS;
			}
			writehead(stream, CL_COMPRESS_TMIN, head.count, info, true, false); 

			//////////////////��Ʊ����//////////////////////////////////////////
			start_min = (uint32)lpCur[index].m_time_t;
			stream.SETBITCODE(dynaID);
			stream.EncodeData(start_min);
			m_open_dw = lpCur[index].m_open_dw;
			stream.SETBITCODE(dayPriceCode);
			stream.EncodeData(m_open_dw);
			m_volume_z = lpCur[index].m_volume_z;
			stream.SETBITCODE(dayVolCode);
			stream.EncodeData(m_volume_z);
			//////////////////��׼ֵ//////////////////////////////////////////
			for (int k = 0; k < head.count; k++, index++)
			{
				stream.SETBITCODE(dynaID);
				stream.EncodeData((uint32)lpCur[index].m_time_t, &start_min);
				start_min = (uint32)lpCur[index].m_time_t;

				stream.SETBITCODE(dayPriceCode);
				stream.EncodeData(lpCur[index].m_open_dw, &m_open_dw);
				m_open_dw = lpCur[index].m_open_dw;

				stream.SETBITCODE(dayPriceDiffCode);
				stream.EncodeData(lpCur[index].m_high_dw, &m_open_dw);
				stream.EncodeData(lpCur[index].m_low_dw, &m_open_dw);
				stream.EncodeData(lpCur[index].m_new_dw, &m_open_dw);

				stream.SETBITCODE(tickVolDiffCode);
				stream.EncodeData(lpCur[index].m_volume_z, &m_volume_z);
				m_volume_z = lpCur[index].m_volume_z;

				stream.SETBITCODE(dayAmountCode);
				zint32 mAmountBase(0, 0);
				if (info->m_type_by != STK_STATIC::INDEX)
					mAmountBase = lpCur[index].m_volume_z*(int)(info->m_volunit_w * lpCur[index].m_new_dw / expand);
				stream.EncodeData(lpCur[index].m_amount_z, &mAmountBase);

			} //k

			outlen = (stream.GetCurPos() + 7) / 8;
			//һ����ѹ�����ˣ�����sdsnode��
			result = sdsnode_push(result, out, outlen);
		}//m
		last.offset = outlen;
		last.bit = (stream.GetCurPos() % 8) > 0 ? 8 - (stream.GetCurPos() % 8) : 0; //offset*8 - bit  
		//�����Ҫ���ӻ�׼���ݿ�head.compress=1,block.count=0/1;
		writehead(stream, CL_COMPRESS_TMIN, 1, info, true, true); 
		//LOG(5)("com offset=%d,bit=%d\n", last.offset, last.bit);
		stream.Put(last.offset, 16);
		stream.Put(last.bit, 8);
		//////////////////��Ʊ����//////////////////////////////////////////
		stream.SETBITCODE(dynaID);
		stream.EncodeData(start_min);
		stream.SETBITCODE(dayPriceCode);
		stream.EncodeData(m_open_dw);

			stream.SETBITCODE(dayVolCode);
			stream.EncodeData(m_volume_z);
			//���ﱣ�����һ����¼
			index = count_ - 1;
			stream.SETBITCODE(dynaID);
			stream.EncodeData((uint32)lpCur[index].m_time_t, &start_min);
			stream.SETBITCODE(dayPriceCode);
			stream.EncodeData(lpCur[index].m_open_dw, &m_open_dw);
			m_open_dw = lpCur[index].m_open_dw;
			stream.SETBITCODE(dayPriceDiffCode);
			stream.EncodeData(lpCur[index].m_high_dw, &m_open_dw);
			stream.EncodeData(lpCur[index].m_low_dw, &m_open_dw);
			stream.EncodeData(lpCur[index].m_new_dw, &m_open_dw);
			stream.SETBITCODE(tickVolDiffCode);
			stream.EncodeData(lpCur[index].m_volume_z, &m_volume_z);
			stream.SETBITCODE(dayAmountCode);
			zint32 mAmountBase(0, 0);
			if (info->m_type_by != STK_STATIC::INDEX)
				mAmountBase = lpCur[index].m_volume_z*(int)(info->m_volunit_w * lpCur[index].m_new_dw / expand);
			stream.EncodeData(lpCur[index].m_amount_z, &mAmountBase);
		//////////////////��׼ֵ//////////////////////////////////////////
		outlen = (stream.GetCurPos() + 7) / 8;
		result = sdsnode_push(result, out, outlen);
	}
	catch (...)
	{
		result = NULL;
	}
	return result;
}

listNode *cl_redis_compress_tmin_new(listNode *src, uint8 *in, int count_, s_stock_static *info)
{
	if (src == NULL){
		return cl_redis_compress_tmin(in, count_, info);
	}
	int oCount = get_compress_node_count(src);
	//printf("count_=%d -> %d\n", oCount, count_);
	if (count_<1 || count_<oCount) return NULL;

	if ((count_ - oCount) > 1 || oCount <= 1){  
		//��ֹһ�δ��������¼,����ֻ����������2�Ľ�������ѹ��
		//������㷨�ܶ��ģ��п��ٸģ�
		//sdsnode_destroy(src);�����ͷţ������ֵ��޸�ʱҲ���ͷ�
		return cl_redis_compress_tmin(in, count_, info);
	}

	s_stock_day *lpCur = (s_stock_day*)in;

	//���nCount==oCount��ֻ�޸����һ����¼����������
	//���nCount>oCount,��ʾ���µ����ݵ��ˣ��õ����ڶ���ѹ���ŵ�1���У����һ��ѹ������β���У�

	listNode *lastnode = sdsnode_last_node(src);
	sds ls, s;
	int index;
	int outlen;
	uint32 start_min = 0;
	uint32 open_dw = 0;
	zint32  volume_z(0, 0);
	cl_last_info last;
	cl_block_head head;
	//��һ��,�����һ������ȡ��һ�ڵ��ƫ��λ�ú����յĻ�׼ֵ
	ls = (sds)(lastnode->value);
	c_bit_stream stream((uint8*)ls, sdslen(ls));
	head.hch = stream.Get(8);
	head.compress = stream.Get(1);
	head.type = stream.Get(7);
	head.count = stream.Get(8);
	//stream.MoveTo(sizeof(cl_block_head)* 8);
	//int isindex = 
	stream.Get(1); //�ǲ���ָ��
	int expand = stream.Get(3); //
	expand = cl_power10(expand);
	int vol-unit = stream.Get(3);//
	vol-unit = cl_power10(vol-unit);
	int isnow = stream.Get(1); //=0 ��ʷ���� =1 ʵʱ����
	if (head.count < 1 || head.compress != 1 || isnow != 1){
		LOG(5)("compress min error.[%d,%d,%d]\n", head.count ,head.compress , isnow);
		return NULL;
	}
	last.offset = stream.Get(16);
	last.bit = stream.Get(8);
	//LOG(5)("[%d]offset=%d,bit=%d\n", stream.GetCurPos(), last.offset, last.bit);

	stream.SETBITCODE(dynaID);
	start_min = stream.DecodeData();
	stream.SETBITCODE(dayPriceCode);
	open_dw = stream.DecodeData(&open_dw);
	stream.SETBITCODE(dayVolCode);
	volume_z = stream.DecodeMWordData(&volume_z);

	uint8 out[CL_COMPRESS_MAX_RECS*sizeof(s_stock_day)];
	if (count_>oCount)
	{
		listNode *node = lastnode->prev;
		//�ѵ����ڶ����������ӵ�node����¼��ƫ��λ�ã�count+1������ƫ��λ��
		//����lastnode�Ļ�׼ֵ��Ȼ������һ����¼д�룻
		s = (sds)(node->value);
		memmove(out, (uint8*)s, sdslen(s));
		c_bit_stream stream(out, CL_COMPRESS_MAX_RECS*sizeof(s_stock_day));
		//�޸�����
		stream.MoveTo(16);
		int num = stream.Get(8);
		stream.MoveTo(16);
		stream.Put(num + 1, 8);
		//LOG(5)("offset=%d,bit=%d\n", last.offset, last.bit);
		stream.MoveTo(last.offset * 8 - last.bit);

		index = count_ - 2;

		stream.SETBITCODE(dynaID);
		stream.EncodeData((uint32)lpCur[index].m_time_t, &start_min);
		start_min = (uint32)lpCur[index].m_time_t;

		stream.SETBITCODE(dayPriceCode);
		stream.EncodeData(lpCur[index].m_open_dw, &open_dw);
		open_dw = lpCur[index].m_open_dw;

		stream.SETBITCODE(dayPriceDiffCode);
		stream.EncodeData(lpCur[index].m_high_dw, &open_dw);
		stream.EncodeData(lpCur[index].m_low_dw, &open_dw);
		stream.EncodeData(lpCur[index].m_new_dw, &open_dw);

		stream.SETBITCODE(tickVolDiffCode);
		stream.EncodeData(lpCur[index].m_volume_z, &volume_z);
		volume_z = lpCur[index].m_volume_z;
		stream.SETBITCODE(dayAmountCode);
		zint32 mAmountBase(0, 0);
		if (info->m_type_by != STK_STATIC::INDEX)
			mAmountBase = lpCur[index].m_volume_z*(int)(info->m_volunit_w * lpCur[index].m_new_dw / expand);
		stream.EncodeData(lpCur[index].m_amount_z, &mAmountBase);

		outlen = (stream.GetCurPos() + 7) / 8;
		last.offset = outlen;
		last.bit = (stream.GetCurPos() % 8) > 0 ? 8 - (stream.GetCurPos() % 8) : 0; //offset*8 - bit  
		//��Ҫ��֤����ֵ��ͬ���������
		node = sdsnode_update(node, (char *)out, outlen);
	}
	//else  //������ô��,���һ����¼����Ҫ����
	{
		//�����޸����һ��������Ϣ��������������Ҫ��ѹ��׼ֵ��
		s = (sds)(lastnode->value);
		memmove(out, (uint8*)s, sdslen(s));
		c_bit_stream stream(out, CL_COMPRESS_MAX_RECS*sizeof(s_stock_day));
		stream.MoveTo(sizeof(cl_block_head)* 8 + 8);
		//�޸�һ��ƫ��λ��
		stream.Put(last.offset, 16);
		stream.Put(last.bit, 8);
		//////////////////��Ʊ����//////////////////////////////////////////
		stream.SETBITCODE(dynaID);
		stream.EncodeData(start_min);
		stream.SETBITCODE(dayPriceCode);
		stream.EncodeData(open_dw);
		stream.SETBITCODE(dayVolCode);
		stream.EncodeData(volume_z);
		/////////////////��ѹ��׼ֵ��Ȼ������д����ֵ////////////////////////
		index = count_ - 1;
		stream.SETBITCODE(dynaID);
		stream.EncodeData((uint32)lpCur[index].m_time_t, &start_min);
		stream.SETBITCODE(dayPriceCode);
		stream.EncodeData(lpCur[index].m_open_dw, &open_dw);
		open_dw = lpCur[index].m_open_dw;
		stream.SETBITCODE(dayPriceDiffCode);
		stream.EncodeData(lpCur[index].m_high_dw, &open_dw);
		stream.EncodeData(lpCur[index].m_low_dw, &open_dw);
		stream.EncodeData(lpCur[index].m_new_dw, &open_dw);
		stream.SETBITCODE(tickVolDiffCode);
		stream.EncodeData(lpCur[index].m_volume_z, &volume_z);
		stream.SETBITCODE(dayAmountCode);
		zint32 mAmountBase(0, 0);
		if (info->m_type_by != STK_STATIC::INDEX)
			mAmountBase = lpCur[index].m_volume_z*(int)(info->m_volunit_w * lpCur[index].m_new_dw / expand);
		stream.EncodeData(lpCur[index].m_amount_z, &mAmountBase);

		outlen = (stream.GetCurPos() + 7) / 8;
		//��Ҫ��֤����ֵ��ͬ���������
		lastnode = sdsnode_update(lastnode, (char *)out, outlen);
	}

	return src;
}

s_memory_node *cl_redis_uncompress_tmin(uint8 *in, int inlen, OUT_COMPRESS_TYPE type)
{
	int i;
	char str[255];
	s_memory_node * result = malloc_memory();;

	uint32 open_dw = 0;
	uint32 start_min = 0;
	zint32  volume_z(0, 0);
	cl_block_head head;
	c_bit_stream stream((uint8*)in, inlen);
	try
	{
		result->clear();
		head.hch = stream.Get(8);
		head.compress = stream.Get(1);
		head.type = stream.Get(7);
		head.count = stream.Get(8);
		if (head.count < 1) return result;
		///////////////////ͷ�ṹ/////////////////////////////////////////
		int isindex = stream.Get(1); //�ǲ���ָ��
		int expand = stream.Get(3); //
		expand = cl_power10(expand);
		int vol-unit = stream.Get(3);//
		vol-unit = cl_power10(vol-unit);
		int isnow = stream.Get(1); //=0 ��ʷ���� =1 ʵʱ����
		if (isnow == 0) return result;
		if (head.compress){
			//last.offset = 
			stream.Get(16);
			//last.bit = 
			stream.Get(8);
		}
		//////////////////��Ʊ����//////////////////////////////////////////
		stream.SETBITCODE(dynaID);
		start_min = stream.DecodeData();
		stream.SETBITCODE(dayPriceCode);
		open_dw = stream.DecodeData(&open_dw);
		stream.SETBITCODE(dayVolCode);
		volume_z = stream.DecodeMWordData(&volume_z);
		//////////////////��׼ֵ//////////////////////////////////////////
		s_stock_day stkDay;
		result->clear();
		for (i = 0; i<head.count; i++)
		{
			memset(&stkDay, 0, sizeof(s_stock_day));

			stream.SETBITCODE(dynaID);
			uint32 tt = 0;
			stream.GetNoMove(32, tt);
			printf("tt=%x\n", tt);
			stkDay.m_time_t = stream.DecodeData(&start_min);
			start_min = (uint32)stkDay.m_time_t;

			stream.SETBITCODE(dayPriceCode);
			stkDay.m_open_dw = stream.DecodeData(&open_dw);
			open_dw = stkDay.m_open_dw;

			stream.SETBITCODE(dayPriceDiffCode);
			stkDay.m_high_dw = stream.DecodeData(&open_dw);
			stkDay.m_low_dw = stream.DecodeData(&open_dw);
			stkDay.m_new_dw = stream.DecodeData(&open_dw);

			stream.SETBITCODE(tickVolDiffCode);
			stkDay.m_volume_z = stream.DecodeMWordData(&volume_z);
			volume_z = stkDay.m_volume_z;

			stream.SETBITCODE(dayAmountCode);
			zint32 mAmountBase(0, 0);
			if (isindex == 0){
				mAmountBase = stkDay.m_volume_z*(int)(vol-unit*stkDay.m_new_dw / expand);
			}
			stkDay.m_amount_z = stream.DecodeMWordData(&mAmountBase);
			if (type == OUT_COMPRESS_JSON)
			{
				sprintf(str, "[%d,%d,%d,%d,%d,"P64I","P64I"]", (uint32)stkDay.m_time_t, stkDay.m_open_dw, stkDay.m_high_dw,
					stkDay.m_low_dw, stkDay.m_new_dw, stkDay.m_volume_z.aslong(), stkDay.m_amount_z.aslong());
				*result += str;
				if (i < head.count - 1) *result += ",";
			}
			else{
				result->append((char *)&stkDay, sizeof(s_stock_day));
			}
		}
		//result += "]";
	}
	catch (...)
	{
		result->clear();
		if (type == OUT_COMPRESS_JSON) *result = "[]";
	}
	return result;
}

////////////////////////////////////////////////////////
//  day - tick  ѹ��
////////////////////////////////////////////////////////
listNode *cl_redis_compress_tick(uint8 *in, int count_, s_stock_static *info, bool isnow)
{

	if (count_<1) return NULL;
	s_stock_day *lpCur = (s_stock_day*)in;

	uint8 out[CL_COMPRESS_MAX_RECS*sizeof(s_stock_day)];
	c_bit_stream stream(out, CL_COMPRESS_MAX_RECS*sizeof(s_stock_day));

	int nPage = (int)(count_ / CL_COMPRESS_MAX_RECS) + 1;
	cl_block_head head;
	listNode *result = NULL;
	try
	{
		int index = 0;
		int outlen = 0;
		//int expand = cl_power10(info->m_wPriceDigit);
		//////////////////��׼ֵ//////////////////////////////////////////
		uint32 tLastTime = 0;
		uint32 new_dw = 0;
		zint32  volume_z(0, 0);
		////////////////////////////////////////////////////////////
		for (int m = 0; m < nPage; m++)
		{
			if (m == (nPage - 1)) head.count = count_ - m*CL_COMPRESS_MAX_RECS;
			else head.count = CL_COMPRESS_MAX_RECS;

			if (isnow) writehead(stream, CL_COMPRESS_TICK, head.count, info, true, false);
			else writehead(stream, CL_COMPRESS_D5, head.count, info, false, false);
			//////////////////��Ʊ����//////////////////////////////////////////
			if (!isnow){ //5����
				tLastTime = (uint32)(lpCur[index].m_time_t / MINSEC);
			}
			else{
				tLastTime = (uint32)(lpCur[index].m_time_t);
			}
			stream.Put(tLastTime, 32);
			new_dw = lpCur[index].m_new_dw;
			stream.SETBITCODE(tickPriceCode); /********/
			stream.EncodeData(new_dw);
			if (isnow)
			{
				volume_z = lpCur[index].m_volume_z;
				stream.SETBITCODE(dayVolCode);
				stream.EncodeData(volume_z);
			}
			//////////////////��׼ֵ//////////////////////////////////////////
			for (int k = 0; k < head.count; k++, index++)
			{
				stream.SETBITCODE(tickTimeCode);
				if (!isnow){ //5����
					stream.EncodeData((uint32)(lpCur[index].m_time_t / MINSEC), (uint32*)&tLastTime);
					tLastTime = (uint32)(lpCur[index].m_time_t / MINSEC);
				}
				else{
					stream.EncodeData((uint32)(lpCur[index].m_time_t), (uint32*)&tLastTime);
					tLastTime = (uint32)(lpCur[index].m_time_t);
				}

				stream.SETBITCODE(tickPriceDiffCode);
				stream.EncodeData(lpCur[index].m_new_dw, &new_dw);
				new_dw = lpCur[index].m_new_dw;

				if (isnow)
				{
					stream.SETBITCODE(tickVolDiffCode);
					stream.EncodeData(lpCur[index].m_volume_z, &volume_z);
					volume_z = lpCur[index].m_volume_z;
				}
				else
				{
					stream.SETBITCODE(dayVolCode);
					stream.EncodeData(lpCur[index].m_volume_z);
				}

				/*stream.SETBITCODE(dayAmountCode);
				zint32 mAmountBase(0, 0);
				if (info->m_wType != STK_STATIC::INDEX)
					mAmountBase = lpCur[index].m_volume_z*(int)(info->m_wVolUnit * lpCur[index].m_new_dw / expand);
				stream.EncodeData(lpCur[index].m_amount_z, &mAmountBase);*/
				//stream.SETBITCODE(dayInnerVolCode);
				//stream.EncodeData(lpCur[i].m_DayStock.m_dwInnerVol);
			} //k
			outlen = (stream.GetCurPos() + 7) / 8;
			//һ����ѹ�����ˣ�����sds��
			//printf("2.1.1 %d\n", outlen);
			result = sdsnode_push(result, out, outlen);
		}//m
		//�����Ҫ���ӻ�׼���ݿ�head.compress=1,block.count=0/1;
	}
	catch (...)
	{
		result = NULL;
	}
	return result;
}

////////////////////////////////////////////////////////
//  tick - tick  ѹ��
////////////////////////////////////////////////////////
listNode *cl_redis_compress_tick_to_tick(uint8 *in, int count_, s_stock_static *info, bool isnow)
{

	if (count_<1) return NULL;
	s_stock_tick *lpCur = (s_stock_tick*)in;

	uint8 out[CL_COMPRESS_MAX_RECS*sizeof(s_stock_tick)];
	c_bit_stream stream(out, CL_COMPRESS_MAX_RECS*sizeof(s_stock_tick));

	int nPage = (int)(count_ / CL_COMPRESS_MAX_RECS) + 1;
	cl_block_head head;
	listNode *result = NULL;
	try
	{
		int index = 0;
		int outlen = 0;
		//////////////////��׼ֵ//////////////////////////////////////////
		uint32 tLastTime = 0;
		uint32 new_dw = 0;
		zint32  volume_z(0, 0);
		////////////////////////////////////////////////////////////
		for (int m = 0; m < nPage; m++)
		{
			if (m == (nPage - 1)) head.count = count_ - m*CL_COMPRESS_MAX_RECS;
			else head.count = CL_COMPRESS_MAX_RECS;

			if (isnow) writehead(stream, CL_COMPRESS_TICK, head.count, info, true, false);
			else writehead(stream, CL_COMPRESS_D5, head.count, info, false, false);
			//////////////////��Ʊ����//////////////////////////////////////////
			if (!isnow){ //5����
				tLastTime = (uint32)(lpCur[index].m_time_t / MINSEC);
			}
			else{
				tLastTime = (uint32)(lpCur[index].m_time_t);
			}
			stream.Put(tLastTime, 32);
			new_dw = lpCur[index].m_new_dw;
			stream.SETBITCODE(tickPriceCode); /********/
			stream.EncodeData(new_dw);
			if (isnow)
			{
				volume_z = lpCur[index].m_volume_z;
				stream.SETBITCODE(dayVolCode);
				stream.EncodeData(volume_z);
			}
			//////////////////��׼ֵ//////////////////////////////////////////
			for (int k = 0; k < head.count; k++, index++)
			{
				stream.SETBITCODE(tickTimeCode);
				if (!isnow){ //5����
					stream.EncodeData((uint32)(lpCur[index].m_time_t / MINSEC), (uint32*)&tLastTime);
					tLastTime = (uint32)(lpCur[index].m_time_t / MINSEC);
				}
				else{
					stream.EncodeData((uint32)(lpCur[index].m_time_t), (uint32*)&tLastTime);
					tLastTime = (uint32)(lpCur[index].m_time_t);
				}

				stream.SETBITCODE(tickPriceDiffCode);
				stream.EncodeData(lpCur[index].m_new_dw, &new_dw);
				new_dw = lpCur[index].m_new_dw;

				if (isnow)
				{
					stream.SETBITCODE(tickVolDiffCode);
					stream.EncodeData(lpCur[index].m_volume_z, &volume_z);
					volume_z = lpCur[index].m_volume_z;
				}
				else
				{
					stream.SETBITCODE(dayVolCode);
					stream.EncodeData(lpCur[index].m_volume_z);
				}

				/*stream.SETBITCODE(dayAmountCode);
				zint32 mAmountBase(0, 0);
				if (info->m_wType != STK_STATIC::INDEX)
				mAmountBase = lpCur[index].m_volume_z*(int)(info->m_wVolUnit * lpCur[index].m_new_dw / expand);
				stream.EncodeData(lpCur[index].m_amount_z, &mAmountBase);*/
				//stream.SETBITCODE(dayInnerVolCode);
				//stream.EncodeData(lpCur[i].m_DayStock.m_dwInnerVol);
			} //k
			outlen = (stream.GetCurPos() + 7) / 8;
			//һ����ѹ�����ˣ�����sds��
			result = sdsnode_push(result, out, outlen);
		}//m
	}
	catch (...)
	{
		result = NULL;
	}
	return result;
}

s_memory_node * cl_redis_uncompress_tick(uint8 *in, int inlen, OUT_COMPRESS_TYPE type)
//isnow=0��ʾ��ʷ��tick��Ҫ�ṩ5���ߵ�����,volΪÿ���ӳɽ���;
//isnow=1��ʾ�����tick��������1�����ߺͷֱʳɽ�������,volΪ���տ��к��ۼƳɽ���;
{
	int i;
	char str[255];
	s_memory_node * result = malloc_memory();;

	uint32 new_dw = 0;
	uint32 tLastTime = 0;
	zint32  volume_z(0, 0);
	cl_block_head head;
	c_bit_stream stream((uint8*)in, inlen);
	try
	{
		result->clear();
		head.hch = stream.Get(8);
		head.compress = stream.Get(1);
		head.type = stream.Get(7);
		head.count = stream.Get(8);
		if (head.count < 1) return result;
		///////////////////ͷ�ṹ/////////////////////////////////////////
		//int isindex = 
		stream.Get(1); //�ǲ���ָ��
		int expand = stream.Get(3); //
		expand = cl_power10(expand);
		int vol-unit = stream.Get(3);//
		vol-unit = cl_power10(vol-unit);
		int isnow = stream.Get(1); //=0 ��ʷ���� =1 ʵʱ����
		if (head.compress){
			//last.offset = 
				stream.Get(16);
			//last.bit = 
				stream.Get(8);
		}
		//////////////////��Ʊ����//////////////////////////////////////////
		tLastTime = stream.Get(32);
		stream.SETBITCODE(tickPriceCode);
		new_dw = stream.DecodeData(&new_dw);
		if (isnow)
		{
			stream.SETBITCODE(dayVolCode);
			volume_z = stream.DecodeMWordData(&volume_z);
		}
		//////////////////��׼ֵ//////////////////////////////////////////
		//s_stock_day stkDay;
		s_stock_tick stkTick;
		result->clear();
		for (i = 0; i<head.count; i++)
		{
			//memset(&stkDay, 0, sizeof(s_stock_day));
			memset(&stkTick, 0, sizeof(s_stock_tick));
			stream.SETBITCODE(tickTimeCode);
			stkTick.m_time_t = stream.DecodeData((uint32*)&tLastTime);
			tLastTime = (uint32)stkTick.m_time_t;
			if (!isnow)
			{
				stkTick.m_time_t = stkTick.m_time_t*MINSEC;
			}
			stream.SETBITCODE(tickPriceDiffCode);
			stkTick.m_new_dw = stream.DecodeData(&new_dw);
			new_dw = stkTick.m_new_dw;

			if (isnow)
			{
				stream.SETBITCODE(tickVolDiffCode);
				stkTick.m_volume_z = stream.DecodeMWordData(&volume_z);
				volume_z = stkTick.m_volume_z;
			}
			else
			{
				stream.SETBITCODE(dayVolCode);
				stkTick.m_volume_z = stream.DecodeMWordData();
			}
			if (type == OUT_COMPRESS_JSON)
			{
				sprintf(str, "[%d,%d,"P64I"]", (uint32)stkTick.m_time_t, stkTick.m_new_dw, stkTick.m_volume_z.aslong());
				*result += str;
				if (i < head.count - 1) *result += ",";
			}
			else{
				//result->append((char *)&stkTick, sizeof(s_stock_day));
				result->append((char *)&stkTick, sizeof(s_stock_tick));
			}
		}
	}
	catch (...)
	{
		result->clear();
		if (type == OUT_COMPRESS_JSON) *result = "[]";
	}
	return result;
}
s_memory_node * cl_redis_uncompress_tick_to_day(uint8 *in, int inlen, OUT_COMPRESS_TYPE type)
{
	int i;
	char str[255];
	s_memory_node * result = malloc_memory();

	uint32 new_dw = 0;
	uint32 tLastTime = 0;
	zint32  volume_z(0, 0);
	cl_block_head head;
	c_bit_stream stream((uint8*)in, inlen);
	try
	{
		result->clear();
		head.hch = stream.Get(8);
		head.compress = stream.Get(1);
		head.type = stream.Get(7);
		head.count = stream.Get(8);
		if (head.count < 1) return result;
		///////////////////ͷ�ṹ/////////////////////////////////////////
		//int isindex = 
		stream.Get(1); //�ǲ���ָ��
		int expand = stream.Get(3); //
		expand = cl_power10(expand);
		int vol-unit = stream.Get(3);//
		vol-unit = cl_power10(vol-unit);
		int isnow = stream.Get(1); //=0 ��ʷ���� =1 ʵʱ����
		if (head.compress){
			//last.offset = 
			stream.Get(16);
			//last.bit = 
			stream.Get(8);
		}
		//////////////////��Ʊ����//////////////////////////////////////////
		tLastTime = stream.Get(32);
		stream.SETBITCODE(tickPriceCode);
		new_dw = stream.DecodeData(&new_dw);
		if (isnow)
		{
			stream.SETBITCODE(dayVolCode);
			volume_z = stream.DecodeMWordData(&volume_z);
		}
		//////////////////��׼ֵ//////////////////////////////////////////
		s_stock_day stkDay;
		result->clear();
		for (i = 0; i<head.count; i++)
		{
			memset(&stkDay, 0, sizeof(s_stock_day));
			stream.SETBITCODE(tickTimeCode);
			stkDay.m_time_t = stream.DecodeData((uint32*)&tLastTime);
			tLastTime = (uint32)stkDay.m_time_t;
			if (!isnow)
			{
				stkDay.m_time_t = stkDay.m_time_t*MINSEC;
			}
			stream.SETBITCODE(tickPriceDiffCode);
			stkDay.m_new_dw = stream.DecodeData(&new_dw);
			new_dw = stkDay.m_new_dw;

			if (isnow)
			{
				stream.SETBITCODE(tickVolDiffCode);
				stkDay.m_volume_z = stream.DecodeMWordData(&volume_z);
				volume_z = stkDay.m_volume_z;
			}
			else
			{
				stream.SETBITCODE(dayVolCode);
				stkDay.m_volume_z = stream.DecodeMWordData();
			}
			if (type == OUT_COMPRESS_JSON)
			{
				sprintf(str, "[%d,%d,"P64I"]", (uint32)stkDay.m_time_t, stkDay.m_new_dw, stkDay.m_volume_z.aslong());
				*result += str;
				if (i < head.count - 1) *result += ",";
			}
			else{
				result->append((char *)&stkDay, sizeof(s_stock_day));
			}
		}
	}
	catch (...)
	{
		result->clear();
		if (type == OUT_COMPRESS_JSON) *result = "[]";
	}
	return result;

}


listNode *cl_redis_compress_dyna(uint8 *in, int count_, s_stock_static *info_, bool ismap)
{
	if (count_<1) return NULL;
	s_stock_dynamic *lpCur = (s_stock_dynamic*)in;

	uint8 out[CL_COMPRESS_MAX_RECS*sizeof(s_stock_dynamic)];
	c_bit_stream stream(out, CL_COMPRESS_MAX_RECS*sizeof(s_stock_dynamic));

	int nPage = (int)(count_ / CL_COMPRESS_MAX_RECS) + 1;
	listNode *result = NULL;
	try
	{
		int index = 0;
		int outlen = 0;
		int count = 0;
		uint32 start_index = 0;
		//////////////////��׼ֵ//////////////////////////////////////////
		uint32 tLastTime = 0;
		////////////////////////////////////////////////////////////
		for (int m = 0; m < nPage; m++)
		{
			if (m == (nPage - 1)) count = count_ - m*CL_COMPRESS_MAX_RECS;
			else count = CL_COMPRESS_MAX_RECS;

			stream.MoveTo(0);
			stream.Put('R', 8);
			if (ismap) stream.Put(1, 1);
			else stream.Put(0, 1); 
			stream.Put(CL_COMPRESS_DYNA, 7);
			stream.Put(count, 8);
			//////////////////��Ʊ����//////////////////////////////////////////
			tLastTime = (uint32)(lpCur[index].m_time_t);
			stream.Put(tLastTime, 32);
			if (ismap){
				stream.SETBITCODE(dynaID);
				start_index = lpCur[index].m_id_w;
				stream.EncodeData(start_index);
			}
			//////////////////��׼ֵ//////////////////////////////////////////
			//printf("[%d] %d  %d\n", index, count, count_);
			for (int k = 0; k < count; k++, index++)
			{
				uint32  price_dw = 0;

				s_stock_static *info = info_ + lpCur[index].m_id_w;
				//printf("[%d] %d  %d\n", index, count,lpCur[index].m_id_w);
				if (info->m_type_by == STK_STATIC::INDEX) stream.Put(1, 1);
				else stream.Put(0, 1); //����ָ����
				stream.Put(info->m_price_digit_by, 3);
				int volpower = cl_sqrt10(info->m_volunit_w);
				stream.Put(volpower, 3);
				//if (isnow) stream.Put(1, 1);
				stream.Put(0, 1);//����
				int expand = cl_power10(info->m_price_digit_by);

				if (!ismap){
					stream.SETBITCODE(dynaID);
					stream.EncodeData(lpCur[index].m_id_w, &start_index);
					start_index = lpCur[index].m_id_w;
				} //map��ȫ�����飬˳���ţ�����Ҫid

				stream.SETBITCODE(tickTimeCode);
				stream.EncodeData((uint32)(lpCur[index].m_time_t), (uint32*)&tLastTime);

				price_dw = lpCur[index].m_new_dw;
				stream.SETBITCODE(tickPriceCode); 
				stream.EncodeData(price_dw);

				stream.SETBITCODE(dayVolCode);
				stream.EncodeData(lpCur[index].m_volume_z);
				
				stream.SETBITCODE(dayAmountCode);
				zint32 mAmountBase(0, 0);
				if (info->m_type_by != STK_STATIC::INDEX)
					mAmountBase = lpCur[index].m_volume_z*(int)(info->m_volunit_w * lpCur[index].m_new_dw / expand);
				stream.EncodeData(lpCur[index].m_amount_z, &mAmountBase);

				stream.SETBITCODE(tickPriceDiffCode);
				stream.EncodeData(lpCur[index].m_open_dw, &price_dw);
				stream.EncodeData(lpCur[index].m_high_dw, &price_dw);
				stream.EncodeData(lpCur[index].m_low_dw, &price_dw);

				for (int nn = 0; nn < 5; nn++){
					if (info->m_type_by == STK_STATIC::INDEX){
						stream.SETBITCODE(dynaID);
						stream.EncodeData(lpCur[index].m_buy_price_dw[nn]);
						stream.EncodeData(lpCur[index].m_sell_price_dw[nn]);
					}
					else{
						stream.SETBITCODE(tickPriceDiffCode);
						stream.EncodeData(lpCur[index].m_buy_price_dw[nn], &price_dw);
						stream.EncodeData(lpCur[index].m_sell_price_dw[nn], &price_dw);
					}
				}
				stream.SETBITCODE(mmpVolCode);
				for (int nn = 0; nn < 5; nn++){
					stream.EncodeData(lpCur[index].m_buy_vol_dw[nn]);
					stream.EncodeData(lpCur[index].m_sell_vol_dw[nn]);
				}

			} //k
			outlen = (stream.GetCurPos() + 7) / 8;
			//printf("[%s] outlen=%d\n", outlen);
			//һ����ѹ�����ˣ�����sds��
			result = sdsnode_push(result, out, outlen);
		}//m
	}
	catch (...)
	{
		result = NULL;
	}
	return result;
}
s_memory_node *cl_redis_uncompress_dyna(uint8 *in, int inlen, OUT_COMPRESS_TYPE type){
	int i;
	char str[255];
	s_memory_node * result = malloc_memory();;

	cl_block_head head;
	uint32 tLastTime = 0;
	c_bit_stream stream((uint8*)in, inlen);
	try
	{
		result->clear();
		head.hch = stream.Get(8);
		head.compress = stream.Get(1); //ismap
		head.type = stream.Get(7);
		head.count = stream.Get(8);
		if (head.count < 1) return result;
		///////////////////ͷ�ṹ/////////////////////////////////////////
		tLastTime = stream.Get(32);
		stream.SETBITCODE(dynaID);
		uint32 index = 0;
		if (head.compress)
		{
			index = stream.DecodeData();
		}
		//////////////////��Ʊ����//////////////////////////////////////////
		s_stock_dynamic dyna;
		result->clear();
		for (i = 0; i<head.count; i++)
		{
			//uint32  id_dw = 0;
			uint32  price_dw = 0;
	
			int isindex = stream.Get(1);//STK_STATIC::INDEX
			int expand = cl_power10(stream.Get(3));
			int vol-unit = cl_power10(stream.Get(3));
			stream.Get(1);

			memset(&dyna, 0, sizeof(s_stock_dynamic));
			if (!head.compress){
				stream.SETBITCODE(dynaID);
				dyna.m_id_w = stream.DecodeData(&index);
				//printf("====%d,--%d\n", index, dyna.m_id_w);
				index = dyna.m_id_w;
			}
			else{ //map��ȫ�����飬˳���ţ�����Ҫid
				dyna.m_id_w = index;
				index++;
			}
			stream.SETBITCODE(tickTimeCode);
			dyna.m_time_t = stream.DecodeData((uint32*)&tLastTime);

			stream.SETBITCODE(tickPriceCode);
			dyna.m_new_dw = stream.DecodeData();
			price_dw = dyna.m_new_dw;

			stream.SETBITCODE(dayVolCode);
			dyna.m_volume_z = stream.DecodeMWordData();

			stream.SETBITCODE(dayAmountCode);
			zint32 mAmountBase(0, 0);
			if (isindex == 0){
				mAmountBase = dyna.m_volume_z*(int)(vol-unit*dyna.m_new_dw / expand);
			}
			dyna.m_amount_z = stream.DecodeMWordData(&mAmountBase);

			stream.SETBITCODE(tickPriceDiffCode);
			dyna.m_open_dw = stream.DecodeData(&price_dw);
			dyna.m_high_dw = stream.DecodeData(&price_dw);
			dyna.m_low_dw = stream.DecodeData(&price_dw);

			for (int nn = 0; nn < 5; nn++){
				if (isindex == 1){
					stream.SETBITCODE(dynaID);
					dyna.m_buy_price_dw[nn] = stream.DecodeData();
					dyna.m_sell_price_dw[nn] = stream.DecodeData();
				}
				else{
					stream.SETBITCODE(tickPriceDiffCode);
					dyna.m_buy_price_dw[nn] = stream.DecodeData(&price_dw);
					dyna.m_sell_price_dw[nn] = stream.DecodeData(&price_dw);
				}
			}
			stream.SETBITCODE(mmpVolCode);
			for (int nn = 0; nn < 5; nn++){
				dyna.m_buy_vol_dw[nn] = stream.DecodeData();
				dyna.m_sell_vol_dw[nn] = stream.DecodeData();
			}
			if (type == OUT_COMPRESS_JSON)
			{
				sprintf(str, "[%d,%d,%d,%d,%d,%d,"P64I","P64I",%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d]",
					dyna.m_id_w, (uint32)dyna.m_time_t, dyna.m_open_dw, dyna.m_high_dw, dyna.m_low_dw,
					dyna.m_new_dw, dyna.m_volume_z.aslong(), dyna.m_amount_z.aslong(),
					dyna.m_buy_price_dw[0], dyna.m_buy_vol_dw[0], dyna.m_sell_price_dw[0], dyna.m_sell_vol_dw[0],
					dyna.m_buy_price_dw[1], dyna.m_buy_vol_dw[1], dyna.m_sell_price_dw[1], dyna.m_sell_vol_dw[1],
					dyna.m_buy_price_dw[2], dyna.m_buy_vol_dw[2], dyna.m_sell_price_dw[2], dyna.m_sell_vol_dw[2],
					dyna.m_buy_price_dw[3], dyna.m_buy_vol_dw[3], dyna.m_sell_price_dw[3], dyna.m_sell_vol_dw[3],
					dyna.m_buy_price_dw[4], dyna.m_buy_vol_dw[4], dyna.m_sell_price_dw[4], dyna.m_sell_vol_dw[4]);			
				*result += str;
				if (i < head.count - 1) *result += ",";
			}
			else{
				result->append((char *)&dyna, sizeof(s_stock_dynamic));
			}
		}
	}
	catch (...)
	{
		result->clear();
		if (type == OUT_COMPRESS_JSON) *result = "[]";
	}
	return result;

}
