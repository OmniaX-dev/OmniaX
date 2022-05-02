#include "Signals.hpp"

namespace ox
{
	void SignalRecieverObject::connectSignal(uint32_t signal_id)
	{
		SignalHandler::connect(*this, signal_id);
	}

	void SignalHandler::init(void)
	{
		SignalHandler::m_customRecievers.clear();
		SignalHandler::m_customRecievers.reserve(SignalHandler::__SIGNAL_BUFFER_START_SIZE);
		SignalHandler::m_mousePressedRecievers.clear();
		SignalHandler::m_mousePressedRecievers.reserve(SignalHandler::__SIGNAL_BUFFER_START_SIZE);
		SignalHandler::m_mouseReleasedRecievers.clear();
		SignalHandler::m_mouseReleasedRecievers.reserve(SignalHandler::__SIGNAL_BUFFER_START_SIZE);
		SignalHandler::m_keyPressedRecievers.clear();
		SignalHandler::m_keyPressedRecievers.reserve(SignalHandler::__SIGNAL_BUFFER_START_SIZE);
		SignalHandler::m_keyReleasedRecievers.clear();
		SignalHandler::m_keyReleasedRecievers.reserve(SignalHandler::__SIGNAL_BUFFER_START_SIZE);
		SignalHandler::m_mouseMovedRecievers.clear();
		SignalHandler::m_mouseMovedRecievers.reserve(SignalHandler::__SIGNAL_BUFFER_START_SIZE);
		SignalHandler::m_delegatedSignals.clear();
		SignalHandler::m_delegatedSignals.reserve(SignalHandler::__DELEGATED_SIGNALS_BUFFER_START_SIZE);
	}

	void SignalHandler::refresh(void)
	{
		for (auto& ds : SignalHandler::m_delegatedSignals)
			SignalHandler::emitSignal(ds.id, tSignalPriority::RealTime, ds.ud);
		SignalHandler::m_delegatedSignals.clear();
		//m_delegatedSignals.reserve(__DELEGATED_SIGNALS_BUFFER_START_SIZE);
	}

	void SignalHandler::emitSignal(uint32_t signal_id, uint8_t prio, BaseObject& userData)
	{
		if (prio == tSignalPriority::Normal)
		{
			SignalHandler::m_delegatedSignals.push_back({ signal_id, userData });
			return;
		}
		tSignal signal { signal_id, userData, prio };
		std::vector<tSignalObjPair>* sig_list = nullptr;
		if (signal_id == tBuiltinSignals::MousePressed)
			sig_list = &m_mousePressedRecievers;
		else if (signal_id == tBuiltinSignals::MouseReleased)
			sig_list = &m_mouseReleasedRecievers;
		else if (signal_id == tBuiltinSignals::KeyPressed)
			sig_list = &m_keyPressedRecievers;
		else if (signal_id == tBuiltinSignals::KeyReleased)
			sig_list = &m_keyReleasedRecievers;
		else if (signal_id == tBuiltinSignals::MouseMoved)
			sig_list = &m_mouseMovedRecievers;
		else if (signal_id > tBuiltinSignals::CustomSignalBase)
			sig_list = &m_customRecievers;
		if (sig_list == nullptr) return; //TODO: Warning, unknown signal
		for (auto& sop : *sig_list)
		{
			if (sop.signal_id == signal_id)
			{
				sop.object->handleSignal(signal);
				if (signal.handled) return;
			}
		}
	}

	void SignalHandler::connect(SignalRecieverObject& object, uint32_t signal_id)
	{
		if (signal_id == tBuiltinSignals::MousePressed)
			m_mousePressedRecievers.push_back({ &object, signal_id });
		else if (signal_id == tBuiltinSignals::MouseReleased)
			m_mouseReleasedRecievers.push_back({ &object, signal_id });
		else if (signal_id == tBuiltinSignals::KeyPressed)
			m_keyPressedRecievers.push_back({ &object, signal_id });
		else if (signal_id == tBuiltinSignals::KeyReleased)
			m_keyReleasedRecievers.push_back({ &object, signal_id });
		else if (signal_id == tBuiltinSignals::MouseMoved)
			m_mouseMovedRecievers.push_back({ &object, signal_id });
		else if (signal_id > tBuiltinSignals::CustomSignalBase)
			m_customRecievers.push_back({ &object, signal_id });
		//TODO: Warning, unknown signal
	}
}