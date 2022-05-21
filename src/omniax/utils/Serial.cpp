#include "Serial.hpp"
#include <omniax/vendor/TermColor.hpp>
#include <omniax/utils/Utils.hpp>

namespace ox
{
	namespace serial
	{
		bool SerialIO::init(uint64_t size)
		{
			if (size < 1) return false;
			for (uint64_t i = 0; i < size; i++)
				m_data.push_back(0);
			return true;
		}

		bool SerialIO::r_QWord(StreamIndex addr, QWord& outVal)
		{
			if (!is_validAddr(addr, tTypeSize::QWORD)) return false;
			outVal =  (((QWord)m_data[addr + 7] << 56) & 0xFF00000000000000U)
					| (((QWord)m_data[addr + 6] << 48) & 0x00FF000000000000U)
					| (((QWord)m_data[addr + 5] << 40) & 0x0000FF0000000000U)
					| (((QWord)m_data[addr + 4] << 32) & 0x000000FF00000000U)
					| ((	   m_data[addr + 3] << 24) & 0x00000000FF000000U)
					| ((	   m_data[addr + 2] << 16) & 0x0000000000FF0000U)
					| ((	   m_data[addr + 1] <<  8) & 0x000000000000FF00U)
					| ( 	   m_data[addr + 0]        & 0x00000000000000FFU);
			return true;
		}
		
		bool SerialIO::r_DWord(StreamIndex addr, DWord& outVal)
		{
			if (!is_validAddr(addr, tTypeSize::DWORD)) return false;
			outVal =  ((m_data[addr + 3] << 24) & 0xFF000000U)
					| ((m_data[addr + 2] << 16) & 0x00FF0000U)
					| ((m_data[addr + 1] <<  8) & 0x0000FF00U)
					| ( m_data[addr + 0]        & 0x000000FFU);
			return true;
		}
		
		bool SerialIO::r_Word(StreamIndex addr, Word& outVal)
		{
			if (!is_validAddr(addr, tTypeSize::WORD)) return false;
			outVal =  ((m_data[addr + 1] <<  8) & 0xFF00U)
					| ( m_data[addr + 0]        & 0x00FFU);
			return true;
		}
		
		bool SerialIO::r_Byte(StreamIndex addr, Byte& outVal)
		{
			if (!is_validAddr(addr, tTypeSize::BYTE)) return false;
			outVal = m_data[addr];
			return true;
		}
		
		bool SerialIO::r_Addr(StreamIndex addr, StreamIndex& outVal)
		{
			if (!is_validAddr(addr, tTypeSize::ADDR)) return false;
			outVal =  ((m_data[addr + 3] << 24) & 0xFF000000U)
					| ((m_data[addr + 2] << 16) & 0x00FF0000U)
					| ((m_data[addr + 1] <<  8) & 0x0000FF00U)
					| ( m_data[addr + 0]        & 0x000000FFU);
			return true;
		}

		bool SerialIO::r_Float(StreamIndex addr, float& outVal)
		{
			if (!is_validAddr(addr, tTypeSize::FLOAT)) return false;
			__float_parser fp;
			fp.data[0] = m_data[addr + 3];
			fp.data[1] = m_data[addr + 2];
			fp.data[2] = m_data[addr + 1];
			fp.data[3] = m_data[addr + 0];
			outVal = fp.val;
			return true;
		}

		bool SerialIO::r_Double(StreamIndex addr, double& outVal)
		{
			if (!is_validAddr(addr, tTypeSize::DOUBLE)) return false;
			__double_parser dp;
			dp.data[0] = m_data[addr++];
			dp.data[1] = m_data[addr++];
			dp.data[2] = m_data[addr++];
			dp.data[3] = m_data[addr++];
			dp.data[4] = m_data[addr++];
			dp.data[5] = m_data[addr++];
			dp.data[6] = m_data[addr++];
			dp.data[7] = m_data[addr  ];
			outVal = dp.val;
			return true;
		}
		
		// bool SerialIO::r_Data(StreamIndex addr, ISerializable& outData) //TODO: Potentially dangerous
		// {
		// 	if (!is_validAddr(addr, outData.getMaxSerializedSize(), nullptr)) return false;
		// 	return outData.deserialize(m_data, addr);
		// }
		
		bool SerialIO::r_ByteStream(StreamIndex addr, ByteStream& outStream)
		{
			if (!is_validAddr(addr, tTypeSize::ADDR)) return false;
			uint32_t stream_size;
			if (!r_Addr(addr, stream_size)) return false;
			addr += tTypeSize::ADDR;
			if (!is_validAddr(addr, stream_size)) return false;
			outStream.reserve(stream_size);
			for (StreamIndex j = addr; j < addr + stream_size; j++)
				outStream.push_back(m_data[j]);
			return true;
		}

		bool SerialIO::r_ByteStream(StreamIndex addr, ByteStream& outStream, uint32_t size)
		{
			if (!is_validAddr(addr, size)) return false;
			outStream.reserve(size);
			for (StreamIndex j = addr; j < addr + size; j++)
				outStream.push_back(m_data[j]);
			return true;
		}


		bool SerialIO::w_QWord(StreamIndex addr, QWord val)
		{
			if (!is_validAddr(addr, tTypeSize::QWORD)) return false;
			m_data[addr + 7] = (val >> 56) & 0xFF;
			m_data[addr + 6] = (val >> 48) & 0xFF;
			m_data[addr + 5] = (val >> 40) & 0xFF;
			m_data[addr + 4] = (val >> 32) & 0xFF;
			m_data[addr + 3] = (val >> 24) & 0xFF;
			m_data[addr + 2] = (val >> 16) & 0xFF;
			m_data[addr + 1] = (val >> 8) & 0xFF;
			m_data[addr + 0] = val & 0xFF;
			return true;
		}
		
		bool SerialIO::w_DWord(StreamIndex addr, DWord val)
		{
			if (!is_validAddr(addr, tTypeSize::DWORD)) return false;
			m_data[addr + 3] = (val >> 24) & 0xFF;
			m_data[addr + 2] = (val >> 16) & 0xFF;
			m_data[addr + 1] = (val >> 8) & 0xFF;
			m_data[addr + 0] = val & 0xFF;
			return true;
		}
		
		bool SerialIO::w_Word(StreamIndex addr, Word val)
		{
			if (!is_validAddr(addr, tTypeSize::WORD)) return false;
			m_data[addr + 1] = (val >> 8) & 0xFF;
			m_data[addr + 0] = val & 0xFF;
			return true;
		}
		
		bool SerialIO::w_Byte(StreamIndex addr, Byte val)
		{
			if (!is_validAddr(addr, tTypeSize::BYTE)) return false;
			m_data[addr] = val;
			return true;
		}
		
		bool SerialIO::w_Addr(StreamIndex addr, StreamIndex val)
		{
			if (!is_validAddr(addr, tTypeSize::ADDR)) return false;
			m_data[addr + 3] = (val >> 24) & 0xFF;
			m_data[addr + 2] = (val >> 16) & 0xFF;
			m_data[addr + 1] = (val >> 8) & 0xFF;
			m_data[addr + 0] = val & 0xFF;
			return true;
		}

		bool SerialIO::w_Float(StreamIndex addr, float val)
		{
			if (!is_validAddr(addr, tTypeSize::FLOAT)) return false;
			__float_parser fp;
			fp.val = val;
			m_data[addr + 3] = fp.data[0];
			m_data[addr + 2] = fp.data[1];
			m_data[addr + 1] = fp.data[2];
			m_data[addr + 0] = fp.data[3];
			return true;
		}

		bool SerialIO::w_Double(StreamIndex addr, double val)
		{
			if (!is_validAddr(addr, tTypeSize::DOUBLE)) return false;
			__double_parser dp;
			dp.val = val;
			m_data[addr++] = dp.data[0];
			m_data[addr++] = dp.data[1];
			m_data[addr++] = dp.data[2];
			m_data[addr++] = dp.data[3];
			m_data[addr++] = dp.data[4];
			m_data[addr++] = dp.data[5];
			m_data[addr++] = dp.data[6];
			m_data[addr  ] = dp.data[7];
			return true;
		}

		// bool SerialIO::w_Data(StreamIndex addr, const ISerializable& data)
		// {
		// 	if (!is_validAddr(addr, data.getMaxSerializedSize(), &memUsage)) return false;
		// 	auto ser = data.serialize();
		// 	if (ser.size() == 0 || !is_validAddr(addr, ser.size(), &memUsage)) return false;
		// 	for (uint32_t i = 0; i < ser.size(); i++)
		// 		m_data[addr + i] = ser[i];
		// 	return true;
		// }

		bool SerialIO::w_ByteStream(StreamIndex addr, const ByteStream& stream)
		{
			uint32_t stream_size = stream.size();
			if (!is_validAddr(addr, tTypeSize::ADDR + stream_size)) return false;
			if (!w_Addr(addr, stream_size)) return false;
			addr += tTypeSize::ADDR;
			for (StreamIndex j = addr; j < addr + stream_size; j++)
				m_data[j] = stream[j - addr];
			return true;
		}


		bool SerialIO::is_validAddr(StreamIndex addr, StreamIndex offset)
		{
			if ((addr + offset - 1) >= m_data.size()) return false;
			return true;
		}


		void SerialIO::print(StreamIndex start, IOutputHandler& out)
		{
			uint32_t line_len = 32;
			uint64_t power = 1;
			while(power < size())
				power *= 2;
			Utils::printByteStream(m_data, start, line_len, power / line_len, out);
		}




		/*Byte ObjectField::getByte(String name)
		{
			return 0;
		}

		UByte ObjectField::getUByte(String name)
		{
			return 0;
		}
		
		Word ObjectField::getWord(String name)
		{
			return 0;
		}
		
		UWord ObjectField::getUWord(String name)
		{
			return 0;
		}
		
		DWord ObjectField::getDWord(String name)
		{
			return 0;
		}
		
		UDWord ObjectField::getUDWord(String name)
		{
			return 0;
		}
		
		QWord ObjectField::getQWord(String name)
		{
			return 0;
		}
		
		UQWord ObjectField::getUQWord(String name)
		{
			return 0;
		}
		
		float ObjectField::getFloat(String name)
		{
			return 0;
		}
		
		double ObjectField::getDouble(String name)
		{
			return 0;
		}
		
		ObjectField ObjectField::getObject(String name)
		{
			return *this;
		}



		ByteStream OXDBuilder::stringToStream(String str)
		{
			if (str.length() == 0) return { 0 };
			SerialIO sio(str.length() + tFieldType::QWord);
			sio.w_QWord(0, str.length());
			for (uint64_t i = 0; i < str.size(); i++)
			{
				char c = str[i];
				sio.w_Byte(tFieldType::QWord + i, c);
			}
			return sio.getData();
		}


		bool OXDBuilder::Object::create(String name, uint64_t oid)
		{
			if (StringEditor(name).trim().str() == "") return false;
			if (OXDBuilder::Object::s_open) return false;
			OXDBuilder::Object::s_open = true;
			OXDBuilder::Object::s_current = ObjectField();
		}

		ObjectField OXDBuilder::Object::get(void)
		{
			if (!OXDBuilder::Object::s_open) return ObjectField();
			ObjectField obj = s_current;
			OXDBuilder::Object::s_open = false;
			OXDBuilder::Object::s_current = ObjectField();
			return obj;
		}

		bool OXDBuilder::Object::newField_Byte(ObjectField& obj, String name, Byte value)
		{
			return false;
		}

		bool OXDBuilder::Object::newField_UByte(ObjectField& obj, String name, UByte value)
		{
			return false;
		}

		bool OXDBuilder::Object::newField_Word(ObjectField& obj, String name, Word value)
		{
			return false;
		}

		bool OXDBuilder::Object::newField_UWord(ObjectField& obj, String name, UWord value)
		{
			return false;
		}

		bool OXDBuilder::Object::newField_DWord(ObjectField& obj, String name, DWord value)
		{
			return false;
		}

		bool OXDBuilder::Object::newField_UDWord(ObjectField& obj, String name, UDWord value)
		{
			return false;
		}

		bool OXDBuilder::Object::newField_QWord(ObjectField& obj, String name, QWord value)
		{
			return false;
		}

		bool OXDBuilder::Object::newField_UQWord(ObjectField& obj, String name, UQWord value)
		{
			return false;
		}

		bool OXDBuilder::Object::newField_Float(ObjectField& obj, String name, float value)
		{
			return false;
		}

		bool OXDBuilder::Object::newField_Double(ObjectField& obj, String name, double value)
		{
			return false;
		}

		bool OXDBuilder::Object::newField_Object(ObjectField& obj, String name, ObjectField& value)
		{
			return false;
		}*/
		
	} // namespace serial
} // namesoace ox