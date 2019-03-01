//
// Created by paul on 24/04/18.
//

#include "Callback.h"
#include "PinMode.h"
#include "PinName.h"

#ifndef ROOT_INTERRUPTIN_H
#define ROOT_INTERRUPTIN_H

class InterruptIn {
public:
	/** Create an InterruptIn connected to the specified pin
	 *
	 *  @param pin InterruptIn pin to connect to
	 */
	InterruptIn(PinName pin){};
	virtual ~InterruptIn(){};

	/** Read the input, represented as 0 or 1 (int)
	 *
	 *  @returns
	 *    An integer representing the state of the input pin,
	 *    0 for logical 0, 1 for logical 1
	 */
	int read() {
		return 0;
	};

	/** An operator shorthand for read()
	 */
	operator int() {
		return 0;
	};

	/** Attach a function to call when a rising edge occurs on the input
	 *
	 *  @param func A pointer to a void function, or 0 to set as none
	 */
	void rise(Callback<void()> func){};
	/** Attach a function to call when a falling edge occurs on the input
	 *
	 *  @param func A pointer to a void function, or 0 to set as none
	 */
	void fall(Callback<void()> func){};
	/** Set the input pin mode
	 *
	 *  @param pull PullUp, PullDown, PullNone
	 */
	void mode(PinMode pull){};

	/** Enable IRQ. This method depends on hw implementation, might enable one
	 *  port interrupts. For further information, check gpio_irq_enable().
	 */
	void enable_irq(){};

	/** Disable IRQ. This method depends on hw implementation, might disable one
	 *  port interrupts. For further information, check gpio_irq_disable().
	 */
	void disable_irq(){};

protected:
	Callback<void()> _rise;
	Callback<void()> _fall;
};

#endif // ROOT_INTERRUPTIN_H
