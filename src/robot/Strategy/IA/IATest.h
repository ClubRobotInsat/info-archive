//
// Created by terae on 09/05/19.
//

#ifndef ROOT_IATEST_H
#define ROOT_IATEST_H

#include "../AbstractStrategy.h"

namespace Strategy {
	class IATest final : public AbstractStrategy {
	public:
		IATest();

	private:
		void execute() override;

		void funny_action() override;
	};
} // namespace Strategy

#endif // ROOT_IATEST_H
