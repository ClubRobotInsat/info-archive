//
// Created by terae on 11/11/18.
//

#include "Protocol.h"

namespace Communication {
	template <SerialProtocolType P>
	AbstractSerialProtocol<P>::AbstractSerialProtocol() : AbstractSerialProtocol(nullptr) {}

	template <SerialProtocolType P>
	AbstractSerialProtocol<P>::AbstractSerialProtocol(std::shared_ptr<Serial> serial)
	        : _serial(std::move(serial)), _delay(GLOBAL_CONSTANTS.get_default_communication_delay()), protocol(P) {}


	template <SerialProtocolType P>
	GlobalFrame AbstractSerialProtocol<P>::recv_frame_blocking(const std::atomic_bool& running_execution) {
		static uint8_t buf[GlobalFrame::DONNEES_TRAME_MAX];

		// Attente du début de la trame
		while(_serial->read_byte() != BYTE_BEGIN_FRAME_1 && running_execution) {
		}

		if(_serial->read_byte() == BYTE_BEGIN_FRAME_2) {
			if(_serial->read_byte() == BYTE_BEGIN_FRAME_3) {
				if(uint8_t frame_type = _serial->read_byte(); frame_type == BYTE_BEGIN_FRAME_4_NORMAL) {
					/*uint16_t msg_size;
					_serial->lireOctets(reinterpret_cast<uint8_t*>(&msg_size), 2);*/
					uint8_t msg_size = _serial->read_byte();

					_serial->read_bytes(buf, msg_size);
					GlobalFrame received_frame{msg_size, buf};
					/*if (_debug_active) {
					    logDebug("Communicator.RECV ", received_frame, "\ntime: ", _chrono.getElapsedTime(),
					             "\n");
					}*/
					return received_frame;
				} else if(frame_type == BYTE_BEGIN_FRAME_4_PING) {
					// TODO
					throw std::runtime_error("Ping not implemented yet.");
				}
			}
		}
		return GlobalFrame{};
	}

	template <SerialProtocolType P>
	AbstractSerialProtocol<P>::~AbstractSerialProtocol() = default;


	template <SerialProtocolType P>
	void AbstractSerialProtocol<P>::set_delay(Duration delay) {
		_delay = delay;
	}

	template <SerialProtocolType P>
	void AbstractSerialProtocol<P>::tempo_delay() const {
		sleep(_delay);
	}

	template <SerialProtocolType P>
	void AbstractSerialProtocol<P>::send_frame(const GlobalFrame& f) {
		std::lock_guard<std::mutex> lk(_mutex_write);

		uint16_t msg_size = f.getNbDonnees();
		GlobalFrame msg{BYTE_BEGIN_FRAME_1, BYTE_BEGIN_FRAME_2, BYTE_BEGIN_FRAME_3, BYTE_BEGIN_FRAME_4_NORMAL, static_cast<uint8_t>(msg_size)};
		msg += f;
		_serial->write_bytes(msg.getDonnees(), msg.getNbDonnees());

		sleep(_delay);
	}

	template <SerialProtocolType P>
	void AbstractSerialProtocol<P>::recv_frame(const std::atomic_bool& running_execution,
	                                           const std::function<void(const GlobalFrame&)>& handler) {
		while(running_execution) {
			GlobalFrame f = recv_frame_blocking(running_execution);
			if(!f.empty()) {
				handler(f);
			}
		}
		throw ReceptionAborted();
	}

	template <MultiSerialProtocolType M, SerialProtocolType P>
	AbstractMultiSerialProtocol<M, P>::AbstractMultiSerialProtocol() : protocol(M) {}

	template <MultiSerialProtocolType M, SerialProtocolType P>
	AbstractMultiSerialProtocol<M, P>::~AbstractMultiSerialProtocol() = default;

	template <MultiSerialProtocolType M, SerialProtocolType P>
	void AbstractMultiSerialProtocol<M, P>::send_frame(const GlobalFrame& f) {
		// Suppression de l'octet 0 qui correspond à l'ID ; envoi de tout le reste
		auto msg_size = static_cast<uint8_t>(f.getNbDonnees() - 1);
		uint8_t id = f.getDonnee(0);
		if(auto it = _serials.find(id); it != _serials.cend()) {
			it->second->send_frame(GlobalFrame{msg_size, f.getDonnees() + 1});
		}
	}

	template <MultiSerialProtocolType M, SerialProtocolType P>
	void AbstractMultiSerialProtocol<M, P>::recv_frame(const std::atomic_bool& running_execution,
	                                                   const std::function<void(const GlobalFrame&)>& handler) {
		const std::size_t nbr_serials = _serials.size();
		std::thread in[nbr_serials];
		std::size_t i = 0;

		for(auto& v : _serials) {
			in[i++] = std::thread([&]() { v.second->recv_frame(running_execution, handler); });
		}

		for(i = 0; i < nbr_serials; ++i) {
			in[i].join();
		}
	}
} // namespace Communication
