//
// Created by paul on 24/04/18.
//

#ifndef ROOT_PINNAME_H
#define ROOT_PINNAME_H

#include <string>

class PinName {
public:
	PinName(std::string name) : _name(name) {}


	friend std::ostream& operator<<(std::ostream& stream, const PinName& pin) {
		return (stream << pin._name);
	}

	friend bool operator!=(PinName a, const PinName b) {
		return a._name != b._name;
	}


private:
	std::string _name;
};

const PinName PA_0("PA_0");
const PinName PA_1("PA_1");
const PinName PA_2("PA_2");
const PinName PA_3("PA_3");
const PinName PA_4("PA_4");
const PinName PA_5("PA_5");
const PinName PA_6("PA_6");
const PinName PA_7("PA_7");
const PinName PA_8("PA_8");
const PinName PA_9("PA_9");
const PinName PA_10("PA_10");
const PinName PA_11("PA_11");
const PinName PA_12("PA_12");
const PinName PA_13("PA_13");
const PinName PA_14("PA_14");
const PinName PA_15("PA_15");

const PinName PB_0("PB_0");
const PinName PB_1("PB_1");
const PinName PB_2("PB_2");
const PinName PB_3("PB_3");
const PinName PB_4("PB_4");
const PinName PB_5("PB_5");
const PinName PB_6("PB_6");
const PinName PB_7("PB_7");
const PinName PB_8("PB_8");
const PinName PB_9("PB_9");
const PinName PB_10("PB_10");
const PinName PB_11("PB_11");
const PinName PB_12("PB_12");
const PinName PB_13("PB_13");
const PinName PB_14("PB_14");
const PinName PB_15("PB_15");

const PinName PC_0("PC_0");
const PinName PC_1("PC_1");
const PinName PC_2("PC_2");
const PinName PC_3("PC_3");
const PinName PC_4("PC_4");
const PinName PC_5("PC_5");
const PinName PC_6("PC_6");
const PinName PC_7("PC_7");
const PinName PC_8("PC_8");
const PinName PC_9("PC_9");
const PinName PC_10("PC_10");
const PinName PC_11("PC_11");
const PinName PC_12("PC_12");
const PinName PC_13("PC_13");
const PinName PC_14("PC_14");
const PinName PC_15("PC_15");

const PinName NC("NC");
const PinName USBTX("USBTX");
const PinName USBRX("USBRX");
const PinName A0("A0");
const PinName A1("A1");

#endif // ROOT_PINNAME_H
