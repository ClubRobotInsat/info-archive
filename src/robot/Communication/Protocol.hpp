//
// Created by terae on 11/11/18.
//

#include "Protocol.h"
#include <future>

namespace Communication {

	template <SerialProtocolType P>
	AbstractSerialProtocol<P>::AbstractSerialProtocol() : AbstractSerialProtocol(nullptr) {}

	template <SerialProtocolType P>
	AbstractSerialProtocol<P>::AbstractSerialProtocol(std::shared_ptr<Serial> serial)
	        : Protocol(), _serial(std::move(serial)), _delay(GLOBAL_CONSTANTS().get_default_communication_delay()), protocol(P) {}

	template <SerialProtocolType P>
	GlobalFrame AbstractSerialProtocol<P>::recv_frame_blocking(const std::atomic_bool& running_execution) {
		static uint8_t buf[GlobalFrame::DONNEES_TRAME_MAX];

		// Permets de se remettre en attente si on a reçu `BYTE_BEGIN_FRAME_1` mais que la suite ne correspond pas
		while(running_execution) {
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
	GlobalFrame AbstractSerialProtocol<P>::generate_header(const GlobalFrame& body) const {
		uint16_t msg_size = body.getNbDonnees();
		return {BYTE_BEGIN_FRAME_1, BYTE_BEGIN_FRAME_2, BYTE_BEGIN_FRAME_3, BYTE_BEGIN_FRAME_4_NORMAL, static_cast<uint8_t>(msg_size)};
	}

	template <SerialProtocolType P>
	void AbstractSerialProtocol<P>::send_frame(const GlobalFrame& f) {
		std::lock_guard<std::mutex> lk(_mutex_write);

		GlobalFrame msg = generate_header(f) + f;
		_serial->write_bytes(msg.getDonnees(), msg.getNbDonnees());

		if(debug_active) {
			logInfo("\t", P, "::send_frame(): sent ", msg);
		}

		sleep(_delay);
	}

	template <SerialProtocolType P>
	void AbstractSerialProtocol<P>::recv_frame(const std::atomic_bool& running_execution,
	                                           const std::function<void(const GlobalFrame&)>& handler) {
		if(debug_active) {
			logInfo("\t", P, "::recv_frame(): begins.");
		}

		std::thread killer([&running_execution, this]() {
			while(running_execution) {
				sleep(_refresh_rate);
			}

			if(debug_active) {
				logInfo("\t\t", P, "::recv_frame(): ends -> killer sent an empty frame.");
			}

			// send_frame({});
		});

		auto create_recv_thread = [&running_execution, this]() -> std::future<GlobalFrame> {
			return std::async(std::launch::async, &AbstractSerialProtocol<P>::recv_frame_blocking, this, std::cref(running_execution));
		};

		auto frame = create_recv_thread();

		while(running_execution) {
			std::future_status status = frame.wait_for(_refresh_rate.toSystemDelay());
			if(status == std::future_status::ready) {
				std::lock_guard<std::mutex> lk(_mutex);

				GlobalFrame f = frame.get();

				if(debug_active) {
					logInfo("\t\t", P, "::recv_frame(): received ", f);
				}

				if(!f.empty()) {
					handler(f);
				}
				frame = create_recv_thread();
			}
		}

		if(debug_active) {
			logWarn("\t", P, "::recv_frame(): ends -> throwing a ReceptionAborted exception.");
		}

		killer.join();

		std::stringstream ss;
		ss << "end of the " << P << " communication";
		throw ReceptionAborted(ss.str());
	}

	template <MultiSerialProtocolType M, SerialProtocolType P>
	AbstractMultiSerialProtocol<M, P>::AbstractMultiSerialProtocol() : protocol(M) {}

	template <MultiSerialProtocolType M, SerialProtocolType P>
	AbstractMultiSerialProtocol<M, P>::~AbstractMultiSerialProtocol() = default;

	template <MultiSerialProtocolType M, SerialProtocolType P>
	void AbstractMultiSerialProtocol<M, P>::send_frame(const GlobalFrame& f) {
		// Suppression de l'octet 0 qui correspond à l'ID ; envoi de tout le reste
		auto msg_size = static_cast<uint16_t>(f.getNbDonnees() - 1);
		uint8_t id = f.getDonnee(0);
		if(auto it = _serials.find(id); it != _serials.cend()) {
			it->second->send_frame(GlobalFrame{msg_size, f.getDonnees() + 1});
		} else {
			throw std::runtime_error("Impossible to send a frame to the module n°" + std::to_string(id) +
			                         ": the serial connexion doesn't exist.");
		}
	}

	template <MultiSerialProtocolType M, SerialProtocolType P>
	void AbstractMultiSerialProtocol<M, P>::recv_frame(const std::atomic_bool& running_execution,
	                                                   const std::function<void(const GlobalFrame&)>& handler) {
		std::map<uint8_t, std::thread> threads_in;

		for(auto& v : _serials) {
			const uint8_t id = v.first;
			auto udp = v.second;

			threads_in[id] = std::thread([&running_execution, &handler, udp, id, this]() {
				try {
					udp->recv_frame(running_execution, [&handler, id, this](const GlobalFrame& f) {
						GlobalFrame header{id};
						handler(header + f);

						if(debug_active) {
							logInfo("\t\t", M, "::recv_frame(): received frame ", header + f);
						}
					});
				} catch(const ReceptionAborted&) {
					if(debug_active) {
						logInfo("\t", M, "::recv_frame: ends -> ReceptionAborted caught from ", P, "::thread n°", static_cast<int>(id));
					}
				}
			});
		}

		for(auto& t : threads_in) {
			if(t.second.joinable()) {
				if(debug_active) {
					logInfo("\t", M, "::recv_frame(): ends -> join the thread n°", static_cast<int>(t.first), ".");
				}
				t.second.join();
			}
		}

		std::stringstream ss;
		ss << "end of the " << M << " communication";
		throw ReceptionAborted(ss.str());
	}

} // namespace Communication
