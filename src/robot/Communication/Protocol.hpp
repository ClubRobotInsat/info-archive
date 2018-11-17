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
	        : Protocol(), _serial(std::move(serial)), _delay(GLOBAL_CONSTANTS.get_default_communication_delay()), protocol(P) {}


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
	GlobalFrame AbstractSerialProtocol<P>::generate_header(const GlobalFrame& body) const {
		uint16_t msg_size = body.getNbDonnees();
		return {BYTE_BEGIN_FRAME_1, BYTE_BEGIN_FRAME_2, BYTE_BEGIN_FRAME_3, BYTE_BEGIN_FRAME_4_NORMAL, static_cast<uint8_t>(msg_size)};
	}

	template <SerialProtocolType P>
	void AbstractSerialProtocol<P>::send_frame(const GlobalFrame& f) {
		std::lock_guard<std::mutex> lk(_mutex_write);

		GlobalFrame msg = generate_header(f) + f;
		_serial->write_bytes(msg.getDonnees(), msg.getNbDonnees());

		sleep(_delay);
	}

	template <SerialProtocolType P>
	void AbstractSerialProtocol<P>::recv_frame(const std::atomic_bool& running_execution,
	                                           const std::function<void(const GlobalFrame&)>& handler) {
		auto create_recv_thread = [&running_execution, this]() -> std::future<GlobalFrame> {
			return std::async(std::launch::async, &AbstractSerialProtocol<P>::recv_frame_blocking, this, std::cref(running_execution));
		};

		auto frame = create_recv_thread();

		while(running_execution) {
			std::future_status status = frame.wait_for(_refresh_rate.toSystemDelay());
			switch(status) {
				case std::future_status::ready: {
					std::lock_guard<std::mutex> lk(_mutex);

					/** Ethernet `recv_frame` works without this block
					GlobalFrame f = frame.get();
					if (!f.empty()) {
					    handler(f);
					}
					frame = create_recv_thread();*/
				}
					[[fallthrough]];
				default: { continue; }
			}
		}

		std::cout
		    << "[AbstractSerialProtocol.recv_frame] exiting and throwing a ReceptionAborted exception for the protocol "
		    << P << "." << std::endl;
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
		} else {
			throw std::runtime_error("Impossible to send a frame to the module n°" + std::to_string(id) +
			                         ": the serial connexion doesn't exist.");
		}
	}

	template <MultiSerialProtocolType M, SerialProtocolType P>
	void AbstractMultiSerialProtocol<M, P>::recv_frame(const std::atomic_bool& running_execution,
	                                                   const std::function<void(const GlobalFrame&)>& handler) {
		/*std::map<uint8_t, std::future<void>> threads_in;

		auto create_async_thread = [&](uint8_t id, std::shared_ptr<AbstractSerialProtocol<P>> ptr) {
		    threads_in[id] = std::async(std::launch::async, [&, id]() {
		        // À la réception d'un message, on rajoute l'ID correspond devant la trame avant de la traiter
		        ptr->recv_frame(true, [&handler, id](const GlobalFrame &f) {
		            GlobalFrame header{id};
		            handler(header + f);
		        });
		    });
		};

		auto create_async_threads = [&]() {
		    for (auto &v : _serials) {
		        create_async_thread(v.first, v.second);
		    }
		};

		create_async_threads();
		while(running_execution) {
		    std::map<uint8_t, std::future_status> status;
		    for(auto &v : _serials) {
		        const uint8_t id = v.first;

		        status[id] = threads_in[id].wait_for(refresh_rate.toSystemDelay());
		    };
		    for(auto &v : _serials) {
		        const uint8_t id = v.first;

		        switch(status[id]) {
		            case std::future_status::ready: {
		                std::lock_guard<std::mutex> lk(mutex);
		                threads_in[id].get();

		                create_async_thread(id, v.second);
		            }
		            [[fallthrough]];
		            default: { continue; }
		        }
		    };
		}

		throw ReceptionAborted();*/
		std::cout << std::endl;
		/// async usage; don't work because we can't wait for multiple async for now (C++ 14 limitation)
		/// check for `when_any`
		/*auto create_recv_thread = [&running_execution, &handler](uint8_t id, std::shared_ptr<protocol_udp> ptr) ->
		std::future<void> { return std::async(std::launch::async, [&handler, &ptr, &running_execution, id]() { try { ptr->recv_frame(running_execution,
		[&handler, id](const GlobalFrame& f) { GlobalFrame header{id}; handler(header + f);
		            });
		        } catch(const ReceptionAborted&) {}
		    });
		};

		std::map<uint8_t, std::future<void>> threads_in;
		for(auto& v : _serials) {
		    const uint8_t id = v.first;

		    threads_in[id] = create_recv_thread(id, v.second);
		};

		while(running_execution) {
		    for (auto &v : _serials) {
		        const uint8_t id = v.first;

		        std::future_status status = threads_in[id].wait_for(_refresh_rate.toSystemDelay());
		        switch (status) {
		            case std::future_status::ready: {
		                std::lock_guard<std::mutex> lk(_mutex);
		                threads_in[id].get();
		                threads_in[id] = create_recv_thread(id, v.second);
		                break;
		            }
		            default: {
		                break;
		            }
		        }
		    }
		}*/


		std::map<uint8_t, std::thread> threads_in;

		for(auto& v : _serials) {
			const uint8_t id = v.first;
			auto udp = v.second;

			threads_in[id] = std::thread([&running_execution, &handler, udp, id] {
				try {
					std::cout << "before receiving from id " << (int)id << std::endl;
					udp->recv_frame(running_execution, [&handler, id](const GlobalFrame& f) {
						std::cout << "recved frame from id " << (int)id << std::endl;
						GlobalFrame header{id};
						handler(header + f);
					});
				} catch(const ReceptionAborted&) {
					std::cout << "ReceptionAborted caught for the thread " << (int)id << std::endl;
				}

				std::cout << "OK !! Abort of the thread " << (int)id << std::endl;
			});
		}

		// don't work, the program goes on infinite loop except with a breakpoint line 238
		/// ¯\_(ツ)_/¯
		for(auto& t : threads_in) {
			if(t.second.joinable()) {
				std::cout << "join of the thread " << (int)t.first << std::endl;
				t.second.join();
			} else {
				std::cout << "can't join the thread " << (int)t.first << std::endl;
			}
		}

		throw ReceptionAborted();
	}
} // namespace Communication
